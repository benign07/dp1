#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7759088267961710722);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4595892263117576086);
void pose_H_mod_fun(double *state, double *out_4980541881785532706);
void pose_f_fun(double *state, double dt, double *out_4686085732492579326);
void pose_F_fun(double *state, double dt, double *out_2099470364484387366);
void pose_h_4(double *state, double *unused, double *out_3570747994735782498);
void pose_H_4(double *state, double *unused, double *out_6537679512233298104);
void pose_h_10(double *state, double *unused, double *out_8780416830356704886);
void pose_H_10(double *state, double *unused, double *out_1231757349924805269);
void pose_h_13(double *state, double *unused, double *out_3296472567236440553);
void pose_H_13(double *state, double *unused, double *out_8075309098173729488);
void pose_h_14(double *state, double *unused, double *out_1949153742329726669);
void pose_H_14(double *state, double *unused, double *out_8826276129180881216);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}