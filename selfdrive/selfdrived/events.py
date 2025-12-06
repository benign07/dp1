#!/usr/bin/env python3
import bisect
import math
import os
from enum import IntEnum
from collections.abc import Callable

from cereal import log, car
import cereal.messaging as messaging
from openpilot.common.constants import CV
from openpilot.common.git import get_short_branch
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.locationd.calibrationd import MIN_SPEED_FILTER
from openpilot.system.micd import SAMPLE_RATE, SAMPLE_BUFFER
from openpilot.selfdrive.ui.feedback.feedbackd import FEEDBACK_MAX_DURATION
from openpilot.system.hardware import HARDWARE

AlertSize = log.SelfdriveState.AlertSize
AlertStatus = log.SelfdriveState.AlertStatus
VisualAlert = car.CarControl.HUDControl.VisualAlert
AudibleAlert = car.CarControl.HUDControl.AudibleAlert
EventName = log.OnroadEvent.EventName


# 警报优先级
class Priority(IntEnum):
  LOWEST = 0
  LOWER = 1
  LOW = 2
  MID = 3
  HIGH = 4
  HIGHEST = 5


# 事件类型
class ET:
  ENABLE = 'enable'               # 启用
  PRE_ENABLE = 'preEnable'       # 预启用
  OVERRIDE_LATERAL = 'overrideLateral'  # 横向接管
  OVERRIDE_LONGITUDINAL = 'overrideLongitudinal'  # 纵向接管
  NO_ENTRY = 'noEntry'           # 无法启用
  WARNING = 'warning'            # 警告
  USER_DISABLE = 'userDisable'   # 用户手动退出
  SOFT_DISABLE = 'softDisable'   # 软退出
  IMMEDIATE_DISABLE = 'immediateDisable'  # 立即退出
  PERMANENT = 'permanent'        # 持续显示


# 从枚举获取事件名称
EVENT_NAME = {v: k for k, v in EventName.schema.enumerants.items()}


class Events:
  def __init__(self):
    self.events: list[int] = []
    self.static_events: list[int] = []
    self.event_counters = dict.fromkeys(EVENTS.keys(), 0)

  @property
  def names(self) -> list[int]:
    return self.events

  def __len__(self) -> int:
    return len(self.events)

  def add(self, event_name: int, static: bool=False) -> None:
    if static:
      bisect.insort(self.static_events, event_name)
    bisect.insort(self.events, event_name)

  def clear(self) -> None:
    self.event_counters = {k: (v + 1 if k in self.events else 0) for k, v in self.event_counters.items()}
    self.events = self.static_events.copy()

  def contains(self, event_type: str) -> bool:
    return any(event_type in EVENTS.get(e, {}) for e in self.events)

  def create_alerts(self, event_types: list[str], callback_args=None):
    if callback_args is None:
      callback_args = []

    ret = []
    for e in self.events:
      types = EVENTS[e].keys()
      for et in event_types:
        if et in types:
          alert = EVENTS[e][et]
          if not isinstance(alert, Alert):
            alert = alert(*callback_args)

          if DT_CTRL * (self.event_counters[e] + 1) >= alert.creation_delay:
            alert.alert_type = f"{EVENT_NAME[e]}/{et}"
            alert.event_type = et
            ret.append(alert)
    return ret

  def add_from_msg(self, events):
    for e in events:
      bisect.insort(self.events, e.name.raw)

  def to_msg(self):
    ret = []
    for event_name in self.events:
      event = log.OnroadEvent.new_message()
      event.name = event_name
      for event_type in EVENTS.get(event_name, {}):
        setattr(event, event_type, True)
      ret.append(event)
    return ret


class Alert:
  def __init__(self,
               alert_text_1: str,
               alert_text_2: str,
               alert_status: log.SelfdriveState.AlertStatus,
               alert_size: log.SelfdriveState.AlertSize,
               priority: Priority,
               visual_alert: car.CarControl.HUDControl.VisualAlert,
               audible_alert: car.CarControl.HUDControl.AudibleAlert,
               duration: float,
               creation_delay: float = 0.):

    self.alert_text_1 = alert_text_1
    self.alert_text_2 = alert_text_2
    self.alert_status = alert_status
    self.alert_size = alert_size
    self.priority = priority
    self.visual_alert = visual_alert
    self.audible_alert = audible_alert

    self.duration = int(duration / DT_CTRL)

    self.creation_delay = creation_delay

    self.alert_type = ""
    self.event_type: str | None = None

  def __str__(self) -> str:
    return f"{self.alert_text_1}/{self.alert_text_2} {self.priority} {self.visual_alert} {self.audible_alert}"

  def __gt__(self, alert2) -> bool:
    if not isinstance(alert2, Alert):
      return False
    return self.priority > alert2.priority

# 空警报
EmptyAlert = Alert("" , "", AlertStatus.normal, AlertSize.none, Priority.LOWEST,
                   VisualAlert.none, AudibleAlert.none, 0)

# 无法启用警报
class NoEntryAlert(Alert):
  def __init__(self, alert_text_2: str,
               alert_text_1: str = "openpilot 不可用",
               visual_alert: car.CarControl.HUDControl.VisualAlert=VisualAlert.none):
    if HARDWARE.get_device_type() == 'mici':
      alert_text_1, alert_text_2 = alert_text_2, alert_text_1
    super().__init__(alert_text_1, alert_text_2, AlertStatus.normal,
                     AlertSize.mid, Priority.LOW, visual_alert,
                     AudibleAlert.refuse, 3.)

# 软退出警报
class SoftDisableAlert(Alert):
  def __init__(self, alert_text_2: str):
    super().__init__("立即接管车辆", alert_text_2,
                     AlertStatus.userPrompt, AlertSize.full,
                     Priority.MID, VisualAlert.steerRequired,
                     AudibleAlert.warningSoft, 2.)

# 用户触发的软退出（更温和版本）
class UserSoftDisableAlert(SoftDisableAlert):
  def __init__(self, alert_text_2: str):
    super().__init__(alert_text_2)
    self.alert_text_1 = "openpilot 即将退出"

# 立即退出警报
class ImmediateDisableAlert(Alert):
  def __init__(self, alert_text_2: str):
    super().__init__("立即接管车辆", alert_text_2,
                     AlertStatus.critical, AlertSize.full,
                     Priority.HIGHEST, VisualAlert.steerRequired,
                     AudibleAlert.warningImmediate, 4.)

# 启用提示警报
class EngagementAlert(Alert):
  def __init__(self, audible_alert: car.CarControl.HUDControl.AudibleAlert):
    super().__init__("", "",
                     AlertStatus.normal, AlertSize.none,
                     Priority.MID, VisualAlert.none,
                     audible_alert, .2)

# 持续显示的普通警报
class NormalPermanentAlert(Alert):
  def __init__(self, alert_text_1: str, alert_text_2: str = "", duration: float = 0.2, priority: Priority = Priority.LOWER, creation_delay: float = 0.):
    super().__init__(alert_text_1, alert_text_2,
                     AlertStatus.normal, AlertSize.mid if len(alert_text_2) else AlertSize.small,
                     priority, VisualAlert.none, AudibleAlert.none, duration, creation_delay=creation_delay)

# 启动提示警报
class StartupAlert(Alert):
  def __init__(self, alert_text_1: str, alert_text_2: str = "注意安全", alert_status=AlertStatus.normal):
    alert_size = AlertSize.mid
    if HARDWARE.get_device_type() == 'mici':
      if alert_text_2 == "注意安全":
        alert_text_2 = ""
      alert_size = AlertSize.small
    super().__init__(alert_text_1, alert_text_2,
                     alert_status, alert_size,
                     Priority.LOWER, VisualAlert.none, AudibleAlert.none, 5.)

# ********** 辅助函数 **********
def get_display_speed(speed_ms: float, metric: bool) -> str:
  """获取带单位的显示速度"""
  speed = int(round(speed_ms * (CV.MS_TO_KPH if metric else CV.MS_TO_MPH)))
  unit = 'KM/小时' if metric else '英里/小时'
  return f"{speed} {unit}"

# ********** 警报回调函数 **********
AlertCallbackType = Callable[[car.CarParams, car.CarState, messaging.SubMaster, bool, int, log.ControlsState], Alert]

def soft_disable_alert(alert_text_2: str) -> AlertCallbackType:
  """软退出警报回调"""
  def func(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
    if soft_disable_time < int(0.5 / DT_CTRL):
      return ImmediateDisableAlert(alert_text_2)
    return SoftDisableAlert(alert_text_2)
  return func

def user_soft_disable_alert(alert_text_2: str) -> AlertCallbackType:
  """用户触发的软退出警报回调"""
  def func(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
    if soft_disable_time < int(0.5 / DT_CTRL):
      return ImmediateDisableAlert(alert_text_2)
    return UserSoftDisableAlert(alert_text_2)
  return func

def startup_master_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """主启动警报"""
  branch = get_short_branch()  # 确保分支名称缓存，避免启动时卡顿
  if "REPLAY" in os.environ:
    branch = "回放模式"

  return StartupAlert("警告：当前分支未经测试", branch, alert_status=AlertStatus.userPrompt)

def below_engage_speed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """车速低于启用速度警报"""
  return NoEntryAlert(f"车速需高于 {get_display_speed(CP.minEnableSpeed, metric)} 才能启用")

def below_steer_speed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """车速低于转向辅助速度警报"""
  return Alert(
    f"车速低于 {get_display_speed(CP.minSteerSpeed, metric)} 时转向辅助不可用",
    "",
    AlertStatus.userPrompt, AlertSize.small,
    Priority.LOW, VisualAlert.none, AudibleAlert.prompt, 0.4)

def steer_saturated_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """转向饱和警报"""
  steer_text2 = "向左打方向" if sm['carControl'].actuators.torque > 0 else "向右打方向"
  return Alert(
    "接管车辆",
    steer_text2,
    AlertStatus.userPrompt, AlertSize.mid,
    Priority.LOW, VisualAlert.steerRequired, AudibleAlert.promptRepeat, 2.)

def calibration_incomplete_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """标定未完成警报"""
  first_word = '重新标定中' if sm['liveCalibration'].calStatus == log.LiveCalibrationData.Status.recalibrating else '标定中'
  return Alert(
    f"{first_word}：{sm['liveCalibration'].calPerc:.0f}%",
    f"车速需高于 {get_display_speed(MIN_SPEED_FILTER, metric)}",
    AlertStatus.normal, AlertSize.mid,
    Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .2)

def audio_feedback_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """音频反馈录制警报"""
  duration = FEEDBACK_MAX_DURATION - ((sm['audioFeedback'].blockNum + 1) * SAMPLE_BUFFER / SAMPLE_RATE)
  return NormalPermanentAlert(
    "正在录制音频反馈",
    f"剩余 {round(duration)} 秒{'钟' if round(duration) != 1 else ''}。再次点击可提前保存。",
    priority=Priority.LOW)

# *** 调试警报 ***
def out_of_space_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """存储空间不足警报"""
  full_perc = round(100. - sm['deviceState'].freeSpacePercent)
  return NormalPermanentAlert("存储空间不足", f"已使用 {full_perc}%")

def posenet_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """Posenet速度无效警报"""
  mdl = sm['modelV2'].velocity.x[0] if len(sm['modelV2'].velocity.x) else math.nan
  err = CS.vEgo - mdl
  msg = f"速度误差：{err:.1f} 米/秒"
  return NoEntryAlert(msg, alert_text_1="Posenet速度无效")

def process_not_running_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """进程未运行警报"""
  not_running = [p.name for p in sm['managerState'].processes if not p.running and p.shouldBeRunning]
  msg = ', '.join(not_running)
  return NoEntryAlert(msg, alert_text_1="进程未运行")

def comm_issue_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """进程间通信异常警报"""
  bs = [s for s in sm.data.keys() if not sm.all_checks([s, ])]
  msg = ', '.join(bs[:4])  # 一行显示不下太多内容
  return NoEntryAlert(msg, alert_text_1="进程间通信异常")

def camera_malfunction_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """摄像头故障警报"""
  all_cams = ('roadCameraState', 'driverCameraState', 'wideRoadCameraState')
  bad_cams = [s.replace('State', '') for s in all_cams if s in sm.data.keys() and not sm.all_checks([s, ])]
  bad_cams_cn = {
    'roadCamera': '前视摄像头',
    'driverCamera': '驾驶员摄像头',
    'wideRoadCamera': '广角摄像头'
  }
  bad_cams_display = [bad_cams_cn.get(cam, cam) for cam in bad_cams]
  return NormalPermanentAlert("摄像头故障", ', '.join(bad_cams_display))

def calibration_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """标定无效警报"""
  rpy = sm['liveCalibration'].rpyCalib
  yaw = math.degrees(rpy[2] if len(rpy) == 3 else math.nan)
  pitch = math.degrees(rpy[1] if len(rpy) == 3 else math.nan)
  angles = f"重新安装设备（俯仰角：{pitch:.1f}°，偏航角：{yaw:.1f}°）"
  return NormalPermanentAlert("标定无效", angles)

def paramsd_invalid_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """参数标定无效警报"""
  if not sm['liveParameters'].angleOffsetValid:
    angle_offset_deg = sm['liveParameters'].angleOffsetDeg
    title = "检测到方向盘未对准"
    text = f"角度偏移过大（偏移量：{angle_offset_deg:.1f}°）"
  elif not sm['liveParameters'].steerRatioValid:
    steer_ratio = sm['liveParameters'].steerRatio
    title = "转向比不匹配"
    text = f"转向齿条几何参数可能异常（比例：{steer_ratio:.1f}）"
  elif not sm['liveParameters'].stiffnessFactorValid:
    stiffness_factor = sm['liveParameters'].stiffnessFactor
    title = "轮胎刚度异常"
    text = f"请检查轮胎、胎压或四轮定位（系数：{stiffness_factor:.1f}）"
  else:
    return NoEntryAlert("paramsd 临时错误")

  return NoEntryAlert(alert_text_1=title, alert_text_2=text)

def overheat_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """系统过热警报"""
  cpu = max(sm['deviceState'].cpuTempC, default=0.)
  gpu = max(sm['deviceState'].gpuTempC, default=0.)
  temp = max((cpu, gpu, sm['deviceState'].memoryTempC))
  return NormalPermanentAlert("系统过热", f"{temp:.0f} °C")

def low_memory_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """内存不足警报"""
  return NormalPermanentAlert("内存不足", f"已使用 {sm['deviceState'].memoryUsagePercent}%")

def high_cpu_usage_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """CPU使用率过高警报"""
  x = max(sm['deviceState'].cpuUsagePercent, default=0.)
  return NormalPermanentAlert("CPU使用率过高", f"已使用 {x}%")

def modeld_lagging_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """模型处理延迟警报"""
  return NormalPermanentAlert("驾驶模型处理延迟", f"丢帧率：{sm['modelV2'].frameDropPerc:.1f}%")

def wrong_car_mode_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """车辆模式错误警报"""
  text = "请启用自适应巡航以启用openpilot"
  if CP.brand == "honda":
    text = "请启用主开关以启用openpilot"
  return NoEntryAlert(text)

def joystick_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """摇杆模式警报"""
  gb = sm['carControl'].actuators.accel / 4.
  steer = sm['carControl'].actuators.torque
  vals = f"油门：{round(gb * 100.)}%，转向：{round(steer * 100.)}%"
  return NormalPermanentAlert("摇杆模式", vals)

def longitudinal_maneuver_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """纵向控制模式警报"""
  ad = sm['alertDebug']
  audible_alert = AudibleAlert.prompt if 'Active' in ad.alertText1 else AudibleAlert.none
  alert_status = AlertStatus.userPrompt if 'Active' in ad.alertText1 else AlertStatus.normal
  alert_size = AlertSize.mid if ad.alertText2 else AlertSize.small
  return Alert(ad.alertText1, ad.alertText2,
               alert_status, alert_size,
               Priority.LOW, VisualAlert.none, audible_alert, 0.2)

def personality_changed_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """驾驶风格变更警报"""
  personality_map = {
    log.LongitudinalPersonality.standard: "标准",
    log.LongitudinalPersonality.relaxed: "舒适",
    log.LongitudinalPersonality.aggressive: "激进"
  }
  personality_cn = personality_map.get(personality, str(personality))
  return NormalPermanentAlert(f"驾驶风格：{personality_cn}", duration=1.5)

def invalid_lkas_setting_alert(CP: car.CarParams, CS: car.CarState, sm: messaging.SubMaster, metric: bool, soft_disable_time: int, personality) -> Alert:
  """LKAS设置无效警报"""
  text = "请开启或关闭原厂LKAS以启用"
  if CP.brand == "tesla":
    text = "请切换至交通感知巡航控制以启用"
  elif CP.brand == "mazda":
    text = "请启用车辆原厂LKAS以启用"
  elif CP.brand == "nissan":
    text = "请关闭车辆原厂LKAS以启用"
  return NormalPermanentAlert("LKAS设置无效", text)

# 事件与警报映射表
EVENTS: dict[int, dict[str, Alert | AlertCallbackType]] = {
  # ********** 无警报的事件 **********
  EventName.stockFcw: {},
  EventName.actuatorsApiUnavailable: {},

  # ********** 所有状态下都显示的警报 **********
  EventName.joystickDebug: {
    ET.WARNING: joystick_alert,
    ET.PERMANENT: NormalPermanentAlert("摇杆模式"),
  },

  EventName.longitudinalManeuver: {
    ET.WARNING: longitudinal_maneuver_alert,
    ET.PERMANENT: NormalPermanentAlert("纵向控制模式",
                                       "请确保前方道路畅通"),
  },

  EventName.selfdriveInitializing: {
    ET.NO_ENTRY: NoEntryAlert("系统初始化中"),
  },

  EventName.startup: {
    ET.PERMANENT: StartupAlert("这不是自动驾驶")
  },

  EventName.startupMaster: {
    ET.PERMANENT: startup_master_alert,
  },

  EventName.startupNoControl: {
    ET.PERMANENT: StartupAlert("行车记录仪模式"),
    ET.NO_ENTRY: NoEntryAlert("行车记录仪模式"),
  },

  EventName.startupNoCar: {
    ET.PERMANENT: StartupAlert("未指定车型"),
  },

  EventName.startupNoSecOcKey: {
    ET.PERMANENT: NormalPermanentAlert("行车记录仪模式",
                                       "安全密钥不可用",
                                       priority=Priority.HIGH),
  },

  EventName.dashcamMode: {
    ET.PERMANENT: NormalPermanentAlert("行车记录仪模式",
                                       priority=Priority.LOWEST),
  },

  EventName.invalidLkasSetting: {
    ET.PERMANENT: invalid_lkas_setting_alert,
    ET.NO_ENTRY: NoEntryAlert("LKAS设置无效"),
  },

  EventName.cruiseMismatch: {
    #ET.PERMANENT: ImmediateDisableAlert("openpilot未能取消巡航"),
  },

  # openpilot无法识别车辆，切换为只读模式。可通过添加车辆指纹解决
  # 参考：https://github.com/commaai/openpilot/wiki/Fingerprinting
  EventName.carUnrecognized: {
    ET.PERMANENT: NormalPermanentAlert("进入设置-设备",
                                       "选择车型",
                                       priority=Priority.LOWEST),
  },

  EventName.aeb: {
    ET.PERMANENT: Alert(
      "紧急制动！",
      "自动紧急制动：碰撞风险",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.none, 2.),
    ET.NO_ENTRY: NoEntryAlert("AEB：碰撞风险"),
  },

  EventName.stockAeb: {
    ET.PERMANENT: Alert(
      "紧急制动！",
      "原厂AEB：碰撞风险",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.none, 2.),
    ET.NO_ENTRY: NoEntryAlert("原厂AEB：碰撞风险"),
  },

  EventName.fcw: {
    ET.PERMANENT: Alert(
      "紧急制动！",
      "碰撞风险",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGHEST, VisualAlert.fcw, AudibleAlert.warningSoft, 2.),
  },

  EventName.ldw: {
    ET.PERMANENT: Alert(
      "检测到车道偏离",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.ldw, AudibleAlert.prompt, 3.),
  },

  # ********** 仅在启用状态下显示的警报 **********
  EventName.steerTempUnavailableSilent: {
    ET.WARNING: Alert(
      "转向辅助暂时不可用",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.prompt, 1.8),
  },

  EventName.preDriverDistracted: {
    ET.PERMANENT: Alert(
      "请注意",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.promptDriverDistracted: {
    ET.PERMANENT: Alert(
      "请注意",
      "驾驶员注意力不集中",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.MID, VisualAlert.steerRequired, AudibleAlert.promptDistracted, .1),
  },

  EventName.driverDistracted: {
    ET.PERMANENT: Alert(
      "立即退出openpilot",
      "驾驶员注意力不集中",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.warningImmediate, .1),
  },

  EventName.preDriverUnresponsive: {
    ET.PERMANENT: Alert(
      "触摸方向盘：未检测到面部",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.none, .1),
  },

  EventName.promptDriverUnresponsive: {
    ET.PERMANENT: Alert(
      "触摸方向盘",
      "驾驶员无响应",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.MID, VisualAlert.steerRequired, AudibleAlert.promptDistracted, .1),
  },

  EventName.driverUnresponsive: {
    ET.PERMANENT: Alert(
      "立即退出openpilot",
      "驾驶员无响应",
      AlertStatus.critical, AlertSize.full,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.warningImmediate, .1),
  },

  EventName.manualRestart: {
    ET.WARNING: Alert(
      "接管车辆",
      "请手动恢复驾驶",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.resumeRequired: {
    ET.WARNING: Alert(
      "请按恢复键退出静止状态",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.belowSteerSpeed: {
    ET.WARNING: below_steer_speed_alert,
  },

  EventName.preLaneChangeLeft: {
    ET.WARNING: Alert(
      "确认安全后向左打方向开始变道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.preLaneChangeRight: {
    ET.WARNING: Alert(
      "确认安全后向右打方向开始变道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.laneChangeBlocked: {
    ET.WARNING: Alert(
      "盲区内检测到车辆",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.prompt, .1),
  },

  EventName.laneChange: {
    ET.WARNING: Alert(
      "正在变道",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.steerSaturated: {
    ET.WARNING: Alert(
      "接管车辆",
      "转向角度超出限制",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.LOW, VisualAlert.steerRequired, AudibleAlert.promptRepeat, 2.),
  },

  # 风扇转速>50%但未旋转时触发
  EventName.fanMalfunction: {
    ET.PERMANENT: NormalPermanentAlert("风扇故障", "可能是硬件问题"),
  },

  # 摄像头无帧输出
  EventName.cameraMalfunction: {
    ET.PERMANENT: camera_malfunction_alert,
    ET.SOFT_DISABLE: soft_disable_alert("摄像头故障"),
    ET.NO_ENTRY: NoEntryAlert("摄像头故障：请重启设备"),
  },

  # 摄像头帧率过低
  EventName.cameraFrameRate: {
    ET.PERMANENT: NormalPermanentAlert("摄像头帧率过低", "请重启设备"),
    ET.SOFT_DISABLE: soft_disable_alert("摄像头帧率过低"),
    ET.NO_ENTRY: NoEntryAlert("摄像头帧率过低：请重启设备"),
  },

  # 未使用的事件
  EventName.locationdTemporaryError: {
    ET.NO_ENTRY: NoEntryAlert("定位服务临时错误"),
    ET.SOFT_DISABLE: soft_disable_alert("定位服务临时错误"),
  },

  EventName.locationdPermanentError: {
    ET.NO_ENTRY: NoEntryAlert("定位服务永久错误"),
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("定位服务永久错误"),
    ET.PERMANENT: NormalPermanentAlert("定位服务永久错误"),
  },

  # openpilot通过观察人类和系统的转向输入学习车辆参数：
  # - 转向比：转向齿条的传动比（方向盘角度/轮胎角度）
  # - 轮胎刚度：轮胎抓地力
  # - 角度偏移：直行时转向角度传感器的偏移量
  # 当参数超出合理范围时触发此警报，可能由四轮定位异常或传感器数据错误导致
  # 如频繁出现请在GitHub提交issue
  EventName.paramsdTemporaryError: {
    ET.NO_ENTRY: paramsd_invalid_alert,
    ET.SOFT_DISABLE: soft_disable_alert("参数标定临时错误"),
  },

  EventName.paramsdPermanentError: {
    ET.NO_ENTRY: NoEntryAlert("参数标定永久错误"),
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("参数标定永久错误"),
    ET.PERMANENT: NormalPermanentAlert("参数标定永久错误"),
  },

  # ********** 影响控制状态切换的事件 **********
  EventName.pcmEnable: {
    ET.ENABLE: EngagementAlert(AudibleAlert.engage),
  },

  EventName.buttonEnable: {
    ET.ENABLE: EngagementAlert(AudibleAlert.engage),
  },

  EventName.pcmDisable: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
  },

  EventName.buttonCancel: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("已按下取消键"),
  },

  EventName.brakeHold: {
    ET.WARNING: Alert(
      "请按恢复键退出自动驻车",
      "",
      AlertStatus.userPrompt, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
  },

  EventName.parkBrake: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("驻车制动已启用"),
  },

  EventName.pedalPressed: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("踏板已踩下",
                              visual_alert=VisualAlert.brakePressed),
  },

  EventName.steerDisengage: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("方向盘已操作"),
  },

  EventName.preEnableStandstill: {
    ET.PRE_ENABLE: Alert(
      "松开刹车以启用",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1, creation_delay=1.),
  },

  EventName.gasPressedOverride: {
    ET.OVERRIDE_LONGITUDINAL: Alert(
      "",
      "",
      AlertStatus.normal, AlertSize.none,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.steerOverride: {
    ET.OVERRIDE_LATERAL: Alert(
      "",
      "",
      AlertStatus.normal, AlertSize.none,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .1),
  },

  EventName.wrongCarMode: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: wrong_car_mode_alert,
  },

  EventName.resumeBlocked: {
    ET.NO_ENTRY: NoEntryAlert("请按设定键启用"),
  },

  EventName.wrongCruiseMode: {
    ET.USER_DISABLE: EngagementAlert(AudibleAlert.disengage),
    ET.NO_ENTRY: NoEntryAlert("自适应巡航已禁用"),
  },

  EventName.steerTempUnavailable: {
    ET.SOFT_DISABLE: soft_disable_alert("转向辅助暂时不可用"),
    ET.NO_ENTRY: NoEntryAlert("转向辅助暂时不可用"),
  },

  EventName.steerTimeLimit: {
    ET.SOFT_DISABLE: soft_disable_alert("车辆转向时间限制"),
    ET.NO_ENTRY: NoEntryAlert("车辆转向时间限制"),
  },

  EventName.outOfSpace: {
    ET.PERMANENT: out_of_space_alert,
    ET.NO_ENTRY: NoEntryAlert("存储空间不足"),
  },

  EventName.belowEngageSpeed: {
    ET.NO_ENTRY: below_engage_speed_alert,
  },

  EventName.sensorDataInvalid: {
    ET.PERMANENT: Alert(
      "传感器数据无效",
      "可能是硬件问题",
      AlertStatus.normal, AlertSize.mid,
      Priority.LOWER, VisualAlert.none, AudibleAlert.none, .2, creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("传感器数据无效"),
    ET.SOFT_DISABLE: soft_disable_alert("传感器数据无效"),
  },

  EventName.noGps: {
  },

  EventName.tooDistracted: {
    ET.NO_ENTRY: NoEntryAlert("注意力分散程度过高"),
  },

  EventName.excessiveActuation: {
    ET.SOFT_DISABLE: soft_disable_alert("执行器操作过度"),
    ET.NO_ENTRY: NoEntryAlert("执行器操作过度"),
  },

  EventName.overheat: {
    ET.PERMANENT: overheat_alert,
    ET.SOFT_DISABLE: soft_disable_alert("系统过热"),
    ET.NO_ENTRY: NoEntryAlert("系统过热"),
  },

  EventName.wrongGear: {
    ET.SOFT_DISABLE: user_soft_disable_alert("挡位未在D挡"),
    ET.NO_ENTRY: NoEntryAlert("挡位未在D挡"),
  },

  # 标定时角度超出可接受范围（如设备指向偏左/偏右过多）触发
  # 通常需完全移除挡风玻璃支架，重新安装并确保设备朝前且水平
  # 参考：https://comma.ai/setup
  EventName.calibrationInvalid: {
    ET.PERMANENT: calibration_invalid_alert,
    ET.SOFT_DISABLE: soft_disable_alert("标定无效：请重新安装设备并重新标定"),
    ET.NO_ENTRY: NoEntryAlert("标定无效：请重新安装设备并重新标定"),
  },

  EventName.calibrationIncomplete: {
    ET.PERMANENT: calibration_incomplete_alert,
    ET.SOFT_DISABLE: soft_disable_alert("标定未完成"),
    ET.NO_ENTRY: NoEntryAlert("标定进行中"),
  },

  EventName.calibrationRecalibrating: {
    ET.PERMANENT: calibration_incomplete_alert,
    ET.SOFT_DISABLE: soft_disable_alert("检测到设备重新安装：正在重新标定"),
    ET.NO_ENTRY: NoEntryAlert("检测到重新安装：正在重新标定"),
  },

  EventName.doorOpen: {
    ET.SOFT_DISABLE: user_soft_disable_alert("车门已打开"),
    ET.NO_ENTRY: NoEntryAlert("车门已打开"),
  },

  EventName.seatbeltNotLatched: {
    ET.SOFT_DISABLE: user_soft_disable_alert("安全带未系"),
    ET.NO_ENTRY: NoEntryAlert("安全带未系"),
  },

  EventName.espDisabled: {
    ET.SOFT_DISABLE: soft_disable_alert("电子稳定控制系统已禁用"),
    ET.NO_ENTRY: NoEntryAlert("电子稳定控制系统已禁用"),
  },

  EventName.lowBattery: {
    ET.SOFT_DISABLE: soft_disable_alert("电量过低"),
    ET.NO_ENTRY: NoEntryAlert("电量过低"),
  },

  # openpilot服务间按固定间隔通信，如通信未按常规调度触发此警报
  # 可能原因：服务崩溃、消息广播超时10倍、平均间隔超出10%
  EventName.commIssue: {
    ET.SOFT_DISABLE: soft_disable_alert("进程间通信异常"),
    ET.NO_ENTRY: comm_issue_alert,
  },

  EventName.commIssueAvgFreq: {
    ET.SOFT_DISABLE: soft_disable_alert("进程间通信速率过低"),
    ET.NO_ENTRY: NoEntryAlert("进程间通信速率过低"),
  },

  EventName.selfdrivedLagging: {
    ET.SOFT_DISABLE: soft_disable_alert("系统响应延迟"),
    ET.NO_ENTRY: NoEntryAlert("Selfdrive进程延迟：请重启设备"),
  },

  # 驾驶时管理器检测到服务意外退出时触发
  EventName.processNotRunning: {
    ET.NO_ENTRY: process_not_running_alert,
    ET.SOFT_DISABLE: soft_disable_alert("进程未运行"),
  },

  EventName.radarFault: {
    ET.SOFT_DISABLE: soft_disable_alert("雷达错误：请重启车辆"),
    ET.NO_ENTRY: NoEntryAlert("雷达错误：请重启车辆"),
  },

  EventName.radarTempUnavailable: {
    ET.SOFT_DISABLE: soft_disable_alert("雷达暂时不可用"),
    ET.NO_ENTRY: NoEntryAlert("雷达暂时不可用"),
  },

  # 摄像头每一帧都应被模型处理，如modeld处理速度过慢导致丢帧>20%时触发
  EventName.modeldLagging: {
    ET.SOFT_DISABLE: soft_disable_alert("驾驶模型处理延迟"),
    ET.NO_ENTRY: NoEntryAlert("驾驶模型处理延迟"),
    ET.PERMANENT: modeld_lagging_alert,
  },

  # 模型除预测路径、车道线和前车数据外，还预测车辆当前速度和转速
  # 车辆行驶时模型对速度预测不确定性过高通常表示模型无法理解场景
  # 用作提醒驾驶员的启发式判断
  EventName.posenetInvalid: {
    ET.SOFT_DISABLE: soft_disable_alert("Posenet速度无效"),
    ET.NO_ENTRY: posenet_invalid_alert,
  },

  # 定位器检测到加速度>40m/s²（约4G）时，提醒驾驶员设备可能从挡风玻璃掉落
  EventName.deviceFalling: {
    ET.SOFT_DISABLE: soft_disable_alert("设备已从支架掉落"),
    ET.NO_ENTRY: NoEntryAlert("设备已从支架掉落"),
  },

  EventName.lowMemory: {
    ET.SOFT_DISABLE: soft_disable_alert("内存不足：请重启设备"),
    ET.PERMANENT: low_memory_alert,
    ET.NO_ENTRY: NoEntryAlert("内存不足：请重启设备"),
  },

  EventName.accFaulted: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("巡航故障：请重启车辆"),
    ET.PERMANENT: NormalPermanentAlert("巡航故障：请重启车辆以启用"),
    ET.NO_ENTRY: NoEntryAlert("巡航故障：请重启车辆"),
  },

  EventName.espActive: {
    ET.SOFT_DISABLE: soft_disable_alert("电子稳定控制系统已激活"),
    ET.NO_ENTRY: NoEntryAlert("电子稳定控制系统已激活"),
  },

  EventName.controlsMismatch: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("控制信号不匹配"),
    ET.NO_ENTRY: NoEntryAlert("控制信号不匹配"),
  },

  # 设备USB协议栈可能进入异常状态，导致与panda的连接丢失
  EventName.usbError: {
    ET.SOFT_DISABLE: soft_disable_alert("USB错误：请重启设备"),
    ET.PERMANENT: NormalPermanentAlert("USB错误：请重启设备"),
    ET.NO_ENTRY: NoEntryAlert("USB错误：请重启设备"),
  },

  # 此警报可能因以下原因触发：
  # - 未接收到任何CAN数据
  # - 接收到CAN数据，但部分消息未按正确频率接收
  # 如未开发新车适配，通常由接线故障导致
  EventName.canError: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("CAN总线错误"),
    ET.PERMANENT: Alert(
      "CAN总线错误：请检查连接",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, 1., creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("CAN总线错误：请检查连接"),
  },

  EventName.canBusMissing: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("CAN总线已断开"),
    ET.PERMANENT: Alert(
      "CAN总线已断开：可能是线缆故障",
      "",
      AlertStatus.normal, AlertSize.small,
      Priority.LOW, VisualAlert.none, AudibleAlert.none, 1., creation_delay=1.),
    ET.NO_ENTRY: NoEntryAlert("CAN总线已断开：请检查连接"),
  },

  EventName.steerUnavailable: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("LKAS故障：请重启车辆"),
    ET.PERMANENT: NormalPermanentAlert("LKAS故障：请重启车辆以启用"),
    ET.NO_ENTRY: NoEntryAlert("LKAS故障：请重启车辆"),
  },

  EventName.reverseGear: {
    ET.PERMANENT: Alert(
      "倒车\n挡位",
      "",
      AlertStatus.normal, AlertSize.full,
      Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .2, creation_delay=0.5),
    ET.USER_DISABLE: ImmediateDisableAlert("倒车挡位"),
    ET.NO_ENTRY: NoEntryAlert("倒车挡位"),
  },

  # 使用原厂ACC的车辆可能因多种原因取消ACC
  # 此时无法继续控制车辆，需立即提醒驾驶员
  EventName.cruiseDisabled: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("巡航已关闭"),
  },

  # 线束盒中的继电器断开LKAS摄像头与车辆其余部分的CAN总线连接
  # 如车辆端接收到LKAS摄像头消息，通常表示继电器未正确断开，触发此警报
  EventName.relayMalfunction: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("线束继电器故障"),
    ET.PERMANENT: NormalPermanentAlert("线束继电器故障", "请检查硬件"),
    ET.NO_ENTRY: NoEntryAlert("线束继电器故障"),
  },

  EventName.speedTooLow: {
    ET.IMMEDIATE_DISABLE: Alert(
      "openpilot已取消",
      "车速过低",
      AlertStatus.normal, AlertSize.mid,
      Priority.HIGH, VisualAlert.none, AudibleAlert.disengage, 3.),
  },

  # 车辆行驶速度超过训练数据中的大多数车速时，模型输出可能不可预测
  EventName.speedTooHigh: {
    ET.WARNING: Alert(
      "车速过高",
      "模型在此速度下预测结果不确定",
      AlertStatus.userPrompt, AlertSize.mid,
      Priority.HIGH, VisualAlert.steerRequired, AudibleAlert.promptRepeat, 4.),
    ET.NO_ENTRY: NoEntryAlert("请减速以启用"),
  },

  EventName.vehicleSensorsInvalid: {
    ET.IMMEDIATE_DISABLE: ImmediateDisableAlert("车辆传感器数据无效"),
    ET.PERMANENT: NormalPermanentAlert("车辆传感器标定中", "请行驶以完成标定"),
    ET.NO_ENTRY: NoEntryAlert("车辆传感器标定中"),
  },

  EventName.personalityChanged: {
    ET.WARNING: personality_changed_alert,
  },

  EventName.userBookmark: {
    ET.PERMANENT: NormalPermanentAlert("书签已保存", duration=1.5),
  },

  EventName.audioFeedback: {
    ET.PERMANENT: audio_feedback_alert,
  },
}

# Mici设备特殊配置
if HARDWARE.get_device_type() == 'mici':
  EVENTS.update({
    EventName.preDriverDistracted: {
      ET.PERMANENT: Alert(
        "请注意",
        "",
        AlertStatus.normal, AlertSize.small,
        Priority.LOW, VisualAlert.none, AudibleAlert.none, 2),
    },
    EventName.promptDriverDistracted: {
      ET.PERMANENT: Alert(
        "请注意",
        "驾驶员注意力不集中",
        AlertStatus.userPrompt, AlertSize.mid,
        Priority.MID, VisualAlert.steerRequired, AudibleAlert.promptDistracted, 1),
    },
    EventName.resumeRequired: {
      ET.WARNING: Alert(
        "请按恢复键",
        "",
        AlertStatus.userPrompt, AlertSize.small,
        Priority.LOW, VisualAlert.none, AudibleAlert.none, .2),
    },
    EventName.preLaneChangeLeft: {
      ET.WARNING: Alert(
        "向左打方向",
        "确认变道",
        AlertStatus.normal, AlertSize.mid,
        Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
    },
    EventName.preLaneChangeRight: {
      ET.WARNING: Alert(
        "向右打方向",
        "确认变道",
        AlertStatus.normal, AlertSize.mid,
        Priority.LOW, VisualAlert.none, AudibleAlert.none, .1),
    },
    EventName.laneChangeBlocked: {
      ET.WARNING: Alert(
        "盲区内有车辆",
        "",
        AlertStatus.userPrompt, AlertSize.small,
        Priority.LOW, VisualAlert.none, AudibleAlert.prompt, .1),
    },
    EventName.steerSaturated: {
      ET.WARNING: steer_saturated_alert,
    },
    EventName.calibrationIncomplete: {
      ET.PERMANENT: calibration_incomplete_alert,
      ET.SOFT_DISABLE: soft_disable_alert("标定未完成"),
      ET.NO_ENTRY: NoEntryAlert("标定中"),
    },
    EventName.reverseGear: {
      ET.PERMANENT: Alert(
        "倒车",
        "",
        AlertStatus.normal, AlertSize.full,
        Priority.LOWEST, VisualAlert.none, AudibleAlert.none, .2, creation_delay=0.5),
      ET.USER_DISABLE: ImmediateDisableAlert("倒车"),
      ET.NO_ENTRY: NoEntryAlert("倒车"),
    },
  })

if __name__ == '__main__':
  # 按类型和优先级打印所有警报
  from cereal.services import SERVICE_LIST
  from collections import defaultdict

  event_names = {v: k for k, v in EventName.schema.enumerants.items()}
  alerts_by_type: dict[str, dict[Priority, list[str]]] = defaultdict(lambda: defaultdict(list))

  CP = car.CarParams.new_message()
  CS = car.CarState.new_message()
  sm = messaging.SubMaster(list(SERVICE_LIST.keys()))

  for i, alerts in EVENTS.items():
    for et, alert in alerts.items():
      if callable(alert):
        alert = alert(CP, CS, sm, False, 1, log.LongitudinalPersonality.standard)
      alerts_by_type[et][alert.priority].append(event_names[i])

  all_alerts: dict[str, list[tuple[Priority, list[str]]]] = {}
  for et, priority_alerts in alerts_by_type.items():
    all_alerts[et] = sorted(priority_alerts.items(), key=lambda x: x[0], reverse=True)

  for status, evs in sorted(all_alerts.items(), key=lambda x: x[0]):
    print(f"**** {status} ****")
    for p, alert_list in evs:
      print(f"  {repr(p)}:")
      print("   ", ', '.join(alert_list), "\n")