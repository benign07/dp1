#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_3965466376747700693);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3336789311454738531);
void car_H_mod_fun(double *state, double *out_4696796896630340657);
void car_f_fun(double *state, double dt, double *out_1724048284685650231);
void car_F_fun(double *state, double dt, double *out_9205615493760263126);
void car_h_25(double *state, double *unused, double *out_5133046620795542038);
void car_H_25(double *state, double *unused, double *out_4985789317019942362);
void car_h_24(double *state, double *unused, double *out_3778698140422748706);
void car_H_24(double *state, double *unused, double *out_825596314002834603);
void car_h_30(double *state, double *unused, double *out_5734464778408905592);
void car_H_30(double *state, double *unused, double *out_1930901024471674393);
void car_h_26(double *state, double *unused, double *out_6562014611259862411);
void car_H_26(double *state, double *unused, double *out_8727292635893998586);
void car_h_27(double *state, double *unused, double *out_131373353134696393);
void car_H_27(double *state, double *unused, double *out_243862287328750518);
void car_h_29(double *state, double *unused, double *out_7894638609955368834);
void car_H_29(double *state, double *unused, double *out_1957225014198301551);
void car_h_28(double *state, double *unused, double *out_1821821187870377353);
void car_H_28(double *state, double *unused, double *out_7039624031267832125);
void car_h_31(double *state, double *unused, double *out_3682415228166658871);
void car_H_31(double *state, double *unused, double *out_4955143355142981934);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}