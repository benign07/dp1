#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7759088267961710722) {
   out_7759088267961710722[0] = delta_x[0] + nom_x[0];
   out_7759088267961710722[1] = delta_x[1] + nom_x[1];
   out_7759088267961710722[2] = delta_x[2] + nom_x[2];
   out_7759088267961710722[3] = delta_x[3] + nom_x[3];
   out_7759088267961710722[4] = delta_x[4] + nom_x[4];
   out_7759088267961710722[5] = delta_x[5] + nom_x[5];
   out_7759088267961710722[6] = delta_x[6] + nom_x[6];
   out_7759088267961710722[7] = delta_x[7] + nom_x[7];
   out_7759088267961710722[8] = delta_x[8] + nom_x[8];
   out_7759088267961710722[9] = delta_x[9] + nom_x[9];
   out_7759088267961710722[10] = delta_x[10] + nom_x[10];
   out_7759088267961710722[11] = delta_x[11] + nom_x[11];
   out_7759088267961710722[12] = delta_x[12] + nom_x[12];
   out_7759088267961710722[13] = delta_x[13] + nom_x[13];
   out_7759088267961710722[14] = delta_x[14] + nom_x[14];
   out_7759088267961710722[15] = delta_x[15] + nom_x[15];
   out_7759088267961710722[16] = delta_x[16] + nom_x[16];
   out_7759088267961710722[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4595892263117576086) {
   out_4595892263117576086[0] = -nom_x[0] + true_x[0];
   out_4595892263117576086[1] = -nom_x[1] + true_x[1];
   out_4595892263117576086[2] = -nom_x[2] + true_x[2];
   out_4595892263117576086[3] = -nom_x[3] + true_x[3];
   out_4595892263117576086[4] = -nom_x[4] + true_x[4];
   out_4595892263117576086[5] = -nom_x[5] + true_x[5];
   out_4595892263117576086[6] = -nom_x[6] + true_x[6];
   out_4595892263117576086[7] = -nom_x[7] + true_x[7];
   out_4595892263117576086[8] = -nom_x[8] + true_x[8];
   out_4595892263117576086[9] = -nom_x[9] + true_x[9];
   out_4595892263117576086[10] = -nom_x[10] + true_x[10];
   out_4595892263117576086[11] = -nom_x[11] + true_x[11];
   out_4595892263117576086[12] = -nom_x[12] + true_x[12];
   out_4595892263117576086[13] = -nom_x[13] + true_x[13];
   out_4595892263117576086[14] = -nom_x[14] + true_x[14];
   out_4595892263117576086[15] = -nom_x[15] + true_x[15];
   out_4595892263117576086[16] = -nom_x[16] + true_x[16];
   out_4595892263117576086[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4980541881785532706) {
   out_4980541881785532706[0] = 1.0;
   out_4980541881785532706[1] = 0.0;
   out_4980541881785532706[2] = 0.0;
   out_4980541881785532706[3] = 0.0;
   out_4980541881785532706[4] = 0.0;
   out_4980541881785532706[5] = 0.0;
   out_4980541881785532706[6] = 0.0;
   out_4980541881785532706[7] = 0.0;
   out_4980541881785532706[8] = 0.0;
   out_4980541881785532706[9] = 0.0;
   out_4980541881785532706[10] = 0.0;
   out_4980541881785532706[11] = 0.0;
   out_4980541881785532706[12] = 0.0;
   out_4980541881785532706[13] = 0.0;
   out_4980541881785532706[14] = 0.0;
   out_4980541881785532706[15] = 0.0;
   out_4980541881785532706[16] = 0.0;
   out_4980541881785532706[17] = 0.0;
   out_4980541881785532706[18] = 0.0;
   out_4980541881785532706[19] = 1.0;
   out_4980541881785532706[20] = 0.0;
   out_4980541881785532706[21] = 0.0;
   out_4980541881785532706[22] = 0.0;
   out_4980541881785532706[23] = 0.0;
   out_4980541881785532706[24] = 0.0;
   out_4980541881785532706[25] = 0.0;
   out_4980541881785532706[26] = 0.0;
   out_4980541881785532706[27] = 0.0;
   out_4980541881785532706[28] = 0.0;
   out_4980541881785532706[29] = 0.0;
   out_4980541881785532706[30] = 0.0;
   out_4980541881785532706[31] = 0.0;
   out_4980541881785532706[32] = 0.0;
   out_4980541881785532706[33] = 0.0;
   out_4980541881785532706[34] = 0.0;
   out_4980541881785532706[35] = 0.0;
   out_4980541881785532706[36] = 0.0;
   out_4980541881785532706[37] = 0.0;
   out_4980541881785532706[38] = 1.0;
   out_4980541881785532706[39] = 0.0;
   out_4980541881785532706[40] = 0.0;
   out_4980541881785532706[41] = 0.0;
   out_4980541881785532706[42] = 0.0;
   out_4980541881785532706[43] = 0.0;
   out_4980541881785532706[44] = 0.0;
   out_4980541881785532706[45] = 0.0;
   out_4980541881785532706[46] = 0.0;
   out_4980541881785532706[47] = 0.0;
   out_4980541881785532706[48] = 0.0;
   out_4980541881785532706[49] = 0.0;
   out_4980541881785532706[50] = 0.0;
   out_4980541881785532706[51] = 0.0;
   out_4980541881785532706[52] = 0.0;
   out_4980541881785532706[53] = 0.0;
   out_4980541881785532706[54] = 0.0;
   out_4980541881785532706[55] = 0.0;
   out_4980541881785532706[56] = 0.0;
   out_4980541881785532706[57] = 1.0;
   out_4980541881785532706[58] = 0.0;
   out_4980541881785532706[59] = 0.0;
   out_4980541881785532706[60] = 0.0;
   out_4980541881785532706[61] = 0.0;
   out_4980541881785532706[62] = 0.0;
   out_4980541881785532706[63] = 0.0;
   out_4980541881785532706[64] = 0.0;
   out_4980541881785532706[65] = 0.0;
   out_4980541881785532706[66] = 0.0;
   out_4980541881785532706[67] = 0.0;
   out_4980541881785532706[68] = 0.0;
   out_4980541881785532706[69] = 0.0;
   out_4980541881785532706[70] = 0.0;
   out_4980541881785532706[71] = 0.0;
   out_4980541881785532706[72] = 0.0;
   out_4980541881785532706[73] = 0.0;
   out_4980541881785532706[74] = 0.0;
   out_4980541881785532706[75] = 0.0;
   out_4980541881785532706[76] = 1.0;
   out_4980541881785532706[77] = 0.0;
   out_4980541881785532706[78] = 0.0;
   out_4980541881785532706[79] = 0.0;
   out_4980541881785532706[80] = 0.0;
   out_4980541881785532706[81] = 0.0;
   out_4980541881785532706[82] = 0.0;
   out_4980541881785532706[83] = 0.0;
   out_4980541881785532706[84] = 0.0;
   out_4980541881785532706[85] = 0.0;
   out_4980541881785532706[86] = 0.0;
   out_4980541881785532706[87] = 0.0;
   out_4980541881785532706[88] = 0.0;
   out_4980541881785532706[89] = 0.0;
   out_4980541881785532706[90] = 0.0;
   out_4980541881785532706[91] = 0.0;
   out_4980541881785532706[92] = 0.0;
   out_4980541881785532706[93] = 0.0;
   out_4980541881785532706[94] = 0.0;
   out_4980541881785532706[95] = 1.0;
   out_4980541881785532706[96] = 0.0;
   out_4980541881785532706[97] = 0.0;
   out_4980541881785532706[98] = 0.0;
   out_4980541881785532706[99] = 0.0;
   out_4980541881785532706[100] = 0.0;
   out_4980541881785532706[101] = 0.0;
   out_4980541881785532706[102] = 0.0;
   out_4980541881785532706[103] = 0.0;
   out_4980541881785532706[104] = 0.0;
   out_4980541881785532706[105] = 0.0;
   out_4980541881785532706[106] = 0.0;
   out_4980541881785532706[107] = 0.0;
   out_4980541881785532706[108] = 0.0;
   out_4980541881785532706[109] = 0.0;
   out_4980541881785532706[110] = 0.0;
   out_4980541881785532706[111] = 0.0;
   out_4980541881785532706[112] = 0.0;
   out_4980541881785532706[113] = 0.0;
   out_4980541881785532706[114] = 1.0;
   out_4980541881785532706[115] = 0.0;
   out_4980541881785532706[116] = 0.0;
   out_4980541881785532706[117] = 0.0;
   out_4980541881785532706[118] = 0.0;
   out_4980541881785532706[119] = 0.0;
   out_4980541881785532706[120] = 0.0;
   out_4980541881785532706[121] = 0.0;
   out_4980541881785532706[122] = 0.0;
   out_4980541881785532706[123] = 0.0;
   out_4980541881785532706[124] = 0.0;
   out_4980541881785532706[125] = 0.0;
   out_4980541881785532706[126] = 0.0;
   out_4980541881785532706[127] = 0.0;
   out_4980541881785532706[128] = 0.0;
   out_4980541881785532706[129] = 0.0;
   out_4980541881785532706[130] = 0.0;
   out_4980541881785532706[131] = 0.0;
   out_4980541881785532706[132] = 0.0;
   out_4980541881785532706[133] = 1.0;
   out_4980541881785532706[134] = 0.0;
   out_4980541881785532706[135] = 0.0;
   out_4980541881785532706[136] = 0.0;
   out_4980541881785532706[137] = 0.0;
   out_4980541881785532706[138] = 0.0;
   out_4980541881785532706[139] = 0.0;
   out_4980541881785532706[140] = 0.0;
   out_4980541881785532706[141] = 0.0;
   out_4980541881785532706[142] = 0.0;
   out_4980541881785532706[143] = 0.0;
   out_4980541881785532706[144] = 0.0;
   out_4980541881785532706[145] = 0.0;
   out_4980541881785532706[146] = 0.0;
   out_4980541881785532706[147] = 0.0;
   out_4980541881785532706[148] = 0.0;
   out_4980541881785532706[149] = 0.0;
   out_4980541881785532706[150] = 0.0;
   out_4980541881785532706[151] = 0.0;
   out_4980541881785532706[152] = 1.0;
   out_4980541881785532706[153] = 0.0;
   out_4980541881785532706[154] = 0.0;
   out_4980541881785532706[155] = 0.0;
   out_4980541881785532706[156] = 0.0;
   out_4980541881785532706[157] = 0.0;
   out_4980541881785532706[158] = 0.0;
   out_4980541881785532706[159] = 0.0;
   out_4980541881785532706[160] = 0.0;
   out_4980541881785532706[161] = 0.0;
   out_4980541881785532706[162] = 0.0;
   out_4980541881785532706[163] = 0.0;
   out_4980541881785532706[164] = 0.0;
   out_4980541881785532706[165] = 0.0;
   out_4980541881785532706[166] = 0.0;
   out_4980541881785532706[167] = 0.0;
   out_4980541881785532706[168] = 0.0;
   out_4980541881785532706[169] = 0.0;
   out_4980541881785532706[170] = 0.0;
   out_4980541881785532706[171] = 1.0;
   out_4980541881785532706[172] = 0.0;
   out_4980541881785532706[173] = 0.0;
   out_4980541881785532706[174] = 0.0;
   out_4980541881785532706[175] = 0.0;
   out_4980541881785532706[176] = 0.0;
   out_4980541881785532706[177] = 0.0;
   out_4980541881785532706[178] = 0.0;
   out_4980541881785532706[179] = 0.0;
   out_4980541881785532706[180] = 0.0;
   out_4980541881785532706[181] = 0.0;
   out_4980541881785532706[182] = 0.0;
   out_4980541881785532706[183] = 0.0;
   out_4980541881785532706[184] = 0.0;
   out_4980541881785532706[185] = 0.0;
   out_4980541881785532706[186] = 0.0;
   out_4980541881785532706[187] = 0.0;
   out_4980541881785532706[188] = 0.0;
   out_4980541881785532706[189] = 0.0;
   out_4980541881785532706[190] = 1.0;
   out_4980541881785532706[191] = 0.0;
   out_4980541881785532706[192] = 0.0;
   out_4980541881785532706[193] = 0.0;
   out_4980541881785532706[194] = 0.0;
   out_4980541881785532706[195] = 0.0;
   out_4980541881785532706[196] = 0.0;
   out_4980541881785532706[197] = 0.0;
   out_4980541881785532706[198] = 0.0;
   out_4980541881785532706[199] = 0.0;
   out_4980541881785532706[200] = 0.0;
   out_4980541881785532706[201] = 0.0;
   out_4980541881785532706[202] = 0.0;
   out_4980541881785532706[203] = 0.0;
   out_4980541881785532706[204] = 0.0;
   out_4980541881785532706[205] = 0.0;
   out_4980541881785532706[206] = 0.0;
   out_4980541881785532706[207] = 0.0;
   out_4980541881785532706[208] = 0.0;
   out_4980541881785532706[209] = 1.0;
   out_4980541881785532706[210] = 0.0;
   out_4980541881785532706[211] = 0.0;
   out_4980541881785532706[212] = 0.0;
   out_4980541881785532706[213] = 0.0;
   out_4980541881785532706[214] = 0.0;
   out_4980541881785532706[215] = 0.0;
   out_4980541881785532706[216] = 0.0;
   out_4980541881785532706[217] = 0.0;
   out_4980541881785532706[218] = 0.0;
   out_4980541881785532706[219] = 0.0;
   out_4980541881785532706[220] = 0.0;
   out_4980541881785532706[221] = 0.0;
   out_4980541881785532706[222] = 0.0;
   out_4980541881785532706[223] = 0.0;
   out_4980541881785532706[224] = 0.0;
   out_4980541881785532706[225] = 0.0;
   out_4980541881785532706[226] = 0.0;
   out_4980541881785532706[227] = 0.0;
   out_4980541881785532706[228] = 1.0;
   out_4980541881785532706[229] = 0.0;
   out_4980541881785532706[230] = 0.0;
   out_4980541881785532706[231] = 0.0;
   out_4980541881785532706[232] = 0.0;
   out_4980541881785532706[233] = 0.0;
   out_4980541881785532706[234] = 0.0;
   out_4980541881785532706[235] = 0.0;
   out_4980541881785532706[236] = 0.0;
   out_4980541881785532706[237] = 0.0;
   out_4980541881785532706[238] = 0.0;
   out_4980541881785532706[239] = 0.0;
   out_4980541881785532706[240] = 0.0;
   out_4980541881785532706[241] = 0.0;
   out_4980541881785532706[242] = 0.0;
   out_4980541881785532706[243] = 0.0;
   out_4980541881785532706[244] = 0.0;
   out_4980541881785532706[245] = 0.0;
   out_4980541881785532706[246] = 0.0;
   out_4980541881785532706[247] = 1.0;
   out_4980541881785532706[248] = 0.0;
   out_4980541881785532706[249] = 0.0;
   out_4980541881785532706[250] = 0.0;
   out_4980541881785532706[251] = 0.0;
   out_4980541881785532706[252] = 0.0;
   out_4980541881785532706[253] = 0.0;
   out_4980541881785532706[254] = 0.0;
   out_4980541881785532706[255] = 0.0;
   out_4980541881785532706[256] = 0.0;
   out_4980541881785532706[257] = 0.0;
   out_4980541881785532706[258] = 0.0;
   out_4980541881785532706[259] = 0.0;
   out_4980541881785532706[260] = 0.0;
   out_4980541881785532706[261] = 0.0;
   out_4980541881785532706[262] = 0.0;
   out_4980541881785532706[263] = 0.0;
   out_4980541881785532706[264] = 0.0;
   out_4980541881785532706[265] = 0.0;
   out_4980541881785532706[266] = 1.0;
   out_4980541881785532706[267] = 0.0;
   out_4980541881785532706[268] = 0.0;
   out_4980541881785532706[269] = 0.0;
   out_4980541881785532706[270] = 0.0;
   out_4980541881785532706[271] = 0.0;
   out_4980541881785532706[272] = 0.0;
   out_4980541881785532706[273] = 0.0;
   out_4980541881785532706[274] = 0.0;
   out_4980541881785532706[275] = 0.0;
   out_4980541881785532706[276] = 0.0;
   out_4980541881785532706[277] = 0.0;
   out_4980541881785532706[278] = 0.0;
   out_4980541881785532706[279] = 0.0;
   out_4980541881785532706[280] = 0.0;
   out_4980541881785532706[281] = 0.0;
   out_4980541881785532706[282] = 0.0;
   out_4980541881785532706[283] = 0.0;
   out_4980541881785532706[284] = 0.0;
   out_4980541881785532706[285] = 1.0;
   out_4980541881785532706[286] = 0.0;
   out_4980541881785532706[287] = 0.0;
   out_4980541881785532706[288] = 0.0;
   out_4980541881785532706[289] = 0.0;
   out_4980541881785532706[290] = 0.0;
   out_4980541881785532706[291] = 0.0;
   out_4980541881785532706[292] = 0.0;
   out_4980541881785532706[293] = 0.0;
   out_4980541881785532706[294] = 0.0;
   out_4980541881785532706[295] = 0.0;
   out_4980541881785532706[296] = 0.0;
   out_4980541881785532706[297] = 0.0;
   out_4980541881785532706[298] = 0.0;
   out_4980541881785532706[299] = 0.0;
   out_4980541881785532706[300] = 0.0;
   out_4980541881785532706[301] = 0.0;
   out_4980541881785532706[302] = 0.0;
   out_4980541881785532706[303] = 0.0;
   out_4980541881785532706[304] = 1.0;
   out_4980541881785532706[305] = 0.0;
   out_4980541881785532706[306] = 0.0;
   out_4980541881785532706[307] = 0.0;
   out_4980541881785532706[308] = 0.0;
   out_4980541881785532706[309] = 0.0;
   out_4980541881785532706[310] = 0.0;
   out_4980541881785532706[311] = 0.0;
   out_4980541881785532706[312] = 0.0;
   out_4980541881785532706[313] = 0.0;
   out_4980541881785532706[314] = 0.0;
   out_4980541881785532706[315] = 0.0;
   out_4980541881785532706[316] = 0.0;
   out_4980541881785532706[317] = 0.0;
   out_4980541881785532706[318] = 0.0;
   out_4980541881785532706[319] = 0.0;
   out_4980541881785532706[320] = 0.0;
   out_4980541881785532706[321] = 0.0;
   out_4980541881785532706[322] = 0.0;
   out_4980541881785532706[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_4686085732492579326) {
   out_4686085732492579326[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_4686085732492579326[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_4686085732492579326[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_4686085732492579326[3] = dt*state[12] + state[3];
   out_4686085732492579326[4] = dt*state[13] + state[4];
   out_4686085732492579326[5] = dt*state[14] + state[5];
   out_4686085732492579326[6] = state[6];
   out_4686085732492579326[7] = state[7];
   out_4686085732492579326[8] = state[8];
   out_4686085732492579326[9] = state[9];
   out_4686085732492579326[10] = state[10];
   out_4686085732492579326[11] = state[11];
   out_4686085732492579326[12] = state[12];
   out_4686085732492579326[13] = state[13];
   out_4686085732492579326[14] = state[14];
   out_4686085732492579326[15] = state[15];
   out_4686085732492579326[16] = state[16];
   out_4686085732492579326[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2099470364484387366) {
   out_2099470364484387366[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2099470364484387366[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2099470364484387366[2] = 0;
   out_2099470364484387366[3] = 0;
   out_2099470364484387366[4] = 0;
   out_2099470364484387366[5] = 0;
   out_2099470364484387366[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2099470364484387366[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2099470364484387366[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2099470364484387366[9] = 0;
   out_2099470364484387366[10] = 0;
   out_2099470364484387366[11] = 0;
   out_2099470364484387366[12] = 0;
   out_2099470364484387366[13] = 0;
   out_2099470364484387366[14] = 0;
   out_2099470364484387366[15] = 0;
   out_2099470364484387366[16] = 0;
   out_2099470364484387366[17] = 0;
   out_2099470364484387366[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2099470364484387366[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2099470364484387366[20] = 0;
   out_2099470364484387366[21] = 0;
   out_2099470364484387366[22] = 0;
   out_2099470364484387366[23] = 0;
   out_2099470364484387366[24] = 0;
   out_2099470364484387366[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2099470364484387366[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2099470364484387366[27] = 0;
   out_2099470364484387366[28] = 0;
   out_2099470364484387366[29] = 0;
   out_2099470364484387366[30] = 0;
   out_2099470364484387366[31] = 0;
   out_2099470364484387366[32] = 0;
   out_2099470364484387366[33] = 0;
   out_2099470364484387366[34] = 0;
   out_2099470364484387366[35] = 0;
   out_2099470364484387366[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2099470364484387366[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2099470364484387366[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2099470364484387366[39] = 0;
   out_2099470364484387366[40] = 0;
   out_2099470364484387366[41] = 0;
   out_2099470364484387366[42] = 0;
   out_2099470364484387366[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2099470364484387366[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2099470364484387366[45] = 0;
   out_2099470364484387366[46] = 0;
   out_2099470364484387366[47] = 0;
   out_2099470364484387366[48] = 0;
   out_2099470364484387366[49] = 0;
   out_2099470364484387366[50] = 0;
   out_2099470364484387366[51] = 0;
   out_2099470364484387366[52] = 0;
   out_2099470364484387366[53] = 0;
   out_2099470364484387366[54] = 0;
   out_2099470364484387366[55] = 0;
   out_2099470364484387366[56] = 0;
   out_2099470364484387366[57] = 1;
   out_2099470364484387366[58] = 0;
   out_2099470364484387366[59] = 0;
   out_2099470364484387366[60] = 0;
   out_2099470364484387366[61] = 0;
   out_2099470364484387366[62] = 0;
   out_2099470364484387366[63] = 0;
   out_2099470364484387366[64] = 0;
   out_2099470364484387366[65] = 0;
   out_2099470364484387366[66] = dt;
   out_2099470364484387366[67] = 0;
   out_2099470364484387366[68] = 0;
   out_2099470364484387366[69] = 0;
   out_2099470364484387366[70] = 0;
   out_2099470364484387366[71] = 0;
   out_2099470364484387366[72] = 0;
   out_2099470364484387366[73] = 0;
   out_2099470364484387366[74] = 0;
   out_2099470364484387366[75] = 0;
   out_2099470364484387366[76] = 1;
   out_2099470364484387366[77] = 0;
   out_2099470364484387366[78] = 0;
   out_2099470364484387366[79] = 0;
   out_2099470364484387366[80] = 0;
   out_2099470364484387366[81] = 0;
   out_2099470364484387366[82] = 0;
   out_2099470364484387366[83] = 0;
   out_2099470364484387366[84] = 0;
   out_2099470364484387366[85] = dt;
   out_2099470364484387366[86] = 0;
   out_2099470364484387366[87] = 0;
   out_2099470364484387366[88] = 0;
   out_2099470364484387366[89] = 0;
   out_2099470364484387366[90] = 0;
   out_2099470364484387366[91] = 0;
   out_2099470364484387366[92] = 0;
   out_2099470364484387366[93] = 0;
   out_2099470364484387366[94] = 0;
   out_2099470364484387366[95] = 1;
   out_2099470364484387366[96] = 0;
   out_2099470364484387366[97] = 0;
   out_2099470364484387366[98] = 0;
   out_2099470364484387366[99] = 0;
   out_2099470364484387366[100] = 0;
   out_2099470364484387366[101] = 0;
   out_2099470364484387366[102] = 0;
   out_2099470364484387366[103] = 0;
   out_2099470364484387366[104] = dt;
   out_2099470364484387366[105] = 0;
   out_2099470364484387366[106] = 0;
   out_2099470364484387366[107] = 0;
   out_2099470364484387366[108] = 0;
   out_2099470364484387366[109] = 0;
   out_2099470364484387366[110] = 0;
   out_2099470364484387366[111] = 0;
   out_2099470364484387366[112] = 0;
   out_2099470364484387366[113] = 0;
   out_2099470364484387366[114] = 1;
   out_2099470364484387366[115] = 0;
   out_2099470364484387366[116] = 0;
   out_2099470364484387366[117] = 0;
   out_2099470364484387366[118] = 0;
   out_2099470364484387366[119] = 0;
   out_2099470364484387366[120] = 0;
   out_2099470364484387366[121] = 0;
   out_2099470364484387366[122] = 0;
   out_2099470364484387366[123] = 0;
   out_2099470364484387366[124] = 0;
   out_2099470364484387366[125] = 0;
   out_2099470364484387366[126] = 0;
   out_2099470364484387366[127] = 0;
   out_2099470364484387366[128] = 0;
   out_2099470364484387366[129] = 0;
   out_2099470364484387366[130] = 0;
   out_2099470364484387366[131] = 0;
   out_2099470364484387366[132] = 0;
   out_2099470364484387366[133] = 1;
   out_2099470364484387366[134] = 0;
   out_2099470364484387366[135] = 0;
   out_2099470364484387366[136] = 0;
   out_2099470364484387366[137] = 0;
   out_2099470364484387366[138] = 0;
   out_2099470364484387366[139] = 0;
   out_2099470364484387366[140] = 0;
   out_2099470364484387366[141] = 0;
   out_2099470364484387366[142] = 0;
   out_2099470364484387366[143] = 0;
   out_2099470364484387366[144] = 0;
   out_2099470364484387366[145] = 0;
   out_2099470364484387366[146] = 0;
   out_2099470364484387366[147] = 0;
   out_2099470364484387366[148] = 0;
   out_2099470364484387366[149] = 0;
   out_2099470364484387366[150] = 0;
   out_2099470364484387366[151] = 0;
   out_2099470364484387366[152] = 1;
   out_2099470364484387366[153] = 0;
   out_2099470364484387366[154] = 0;
   out_2099470364484387366[155] = 0;
   out_2099470364484387366[156] = 0;
   out_2099470364484387366[157] = 0;
   out_2099470364484387366[158] = 0;
   out_2099470364484387366[159] = 0;
   out_2099470364484387366[160] = 0;
   out_2099470364484387366[161] = 0;
   out_2099470364484387366[162] = 0;
   out_2099470364484387366[163] = 0;
   out_2099470364484387366[164] = 0;
   out_2099470364484387366[165] = 0;
   out_2099470364484387366[166] = 0;
   out_2099470364484387366[167] = 0;
   out_2099470364484387366[168] = 0;
   out_2099470364484387366[169] = 0;
   out_2099470364484387366[170] = 0;
   out_2099470364484387366[171] = 1;
   out_2099470364484387366[172] = 0;
   out_2099470364484387366[173] = 0;
   out_2099470364484387366[174] = 0;
   out_2099470364484387366[175] = 0;
   out_2099470364484387366[176] = 0;
   out_2099470364484387366[177] = 0;
   out_2099470364484387366[178] = 0;
   out_2099470364484387366[179] = 0;
   out_2099470364484387366[180] = 0;
   out_2099470364484387366[181] = 0;
   out_2099470364484387366[182] = 0;
   out_2099470364484387366[183] = 0;
   out_2099470364484387366[184] = 0;
   out_2099470364484387366[185] = 0;
   out_2099470364484387366[186] = 0;
   out_2099470364484387366[187] = 0;
   out_2099470364484387366[188] = 0;
   out_2099470364484387366[189] = 0;
   out_2099470364484387366[190] = 1;
   out_2099470364484387366[191] = 0;
   out_2099470364484387366[192] = 0;
   out_2099470364484387366[193] = 0;
   out_2099470364484387366[194] = 0;
   out_2099470364484387366[195] = 0;
   out_2099470364484387366[196] = 0;
   out_2099470364484387366[197] = 0;
   out_2099470364484387366[198] = 0;
   out_2099470364484387366[199] = 0;
   out_2099470364484387366[200] = 0;
   out_2099470364484387366[201] = 0;
   out_2099470364484387366[202] = 0;
   out_2099470364484387366[203] = 0;
   out_2099470364484387366[204] = 0;
   out_2099470364484387366[205] = 0;
   out_2099470364484387366[206] = 0;
   out_2099470364484387366[207] = 0;
   out_2099470364484387366[208] = 0;
   out_2099470364484387366[209] = 1;
   out_2099470364484387366[210] = 0;
   out_2099470364484387366[211] = 0;
   out_2099470364484387366[212] = 0;
   out_2099470364484387366[213] = 0;
   out_2099470364484387366[214] = 0;
   out_2099470364484387366[215] = 0;
   out_2099470364484387366[216] = 0;
   out_2099470364484387366[217] = 0;
   out_2099470364484387366[218] = 0;
   out_2099470364484387366[219] = 0;
   out_2099470364484387366[220] = 0;
   out_2099470364484387366[221] = 0;
   out_2099470364484387366[222] = 0;
   out_2099470364484387366[223] = 0;
   out_2099470364484387366[224] = 0;
   out_2099470364484387366[225] = 0;
   out_2099470364484387366[226] = 0;
   out_2099470364484387366[227] = 0;
   out_2099470364484387366[228] = 1;
   out_2099470364484387366[229] = 0;
   out_2099470364484387366[230] = 0;
   out_2099470364484387366[231] = 0;
   out_2099470364484387366[232] = 0;
   out_2099470364484387366[233] = 0;
   out_2099470364484387366[234] = 0;
   out_2099470364484387366[235] = 0;
   out_2099470364484387366[236] = 0;
   out_2099470364484387366[237] = 0;
   out_2099470364484387366[238] = 0;
   out_2099470364484387366[239] = 0;
   out_2099470364484387366[240] = 0;
   out_2099470364484387366[241] = 0;
   out_2099470364484387366[242] = 0;
   out_2099470364484387366[243] = 0;
   out_2099470364484387366[244] = 0;
   out_2099470364484387366[245] = 0;
   out_2099470364484387366[246] = 0;
   out_2099470364484387366[247] = 1;
   out_2099470364484387366[248] = 0;
   out_2099470364484387366[249] = 0;
   out_2099470364484387366[250] = 0;
   out_2099470364484387366[251] = 0;
   out_2099470364484387366[252] = 0;
   out_2099470364484387366[253] = 0;
   out_2099470364484387366[254] = 0;
   out_2099470364484387366[255] = 0;
   out_2099470364484387366[256] = 0;
   out_2099470364484387366[257] = 0;
   out_2099470364484387366[258] = 0;
   out_2099470364484387366[259] = 0;
   out_2099470364484387366[260] = 0;
   out_2099470364484387366[261] = 0;
   out_2099470364484387366[262] = 0;
   out_2099470364484387366[263] = 0;
   out_2099470364484387366[264] = 0;
   out_2099470364484387366[265] = 0;
   out_2099470364484387366[266] = 1;
   out_2099470364484387366[267] = 0;
   out_2099470364484387366[268] = 0;
   out_2099470364484387366[269] = 0;
   out_2099470364484387366[270] = 0;
   out_2099470364484387366[271] = 0;
   out_2099470364484387366[272] = 0;
   out_2099470364484387366[273] = 0;
   out_2099470364484387366[274] = 0;
   out_2099470364484387366[275] = 0;
   out_2099470364484387366[276] = 0;
   out_2099470364484387366[277] = 0;
   out_2099470364484387366[278] = 0;
   out_2099470364484387366[279] = 0;
   out_2099470364484387366[280] = 0;
   out_2099470364484387366[281] = 0;
   out_2099470364484387366[282] = 0;
   out_2099470364484387366[283] = 0;
   out_2099470364484387366[284] = 0;
   out_2099470364484387366[285] = 1;
   out_2099470364484387366[286] = 0;
   out_2099470364484387366[287] = 0;
   out_2099470364484387366[288] = 0;
   out_2099470364484387366[289] = 0;
   out_2099470364484387366[290] = 0;
   out_2099470364484387366[291] = 0;
   out_2099470364484387366[292] = 0;
   out_2099470364484387366[293] = 0;
   out_2099470364484387366[294] = 0;
   out_2099470364484387366[295] = 0;
   out_2099470364484387366[296] = 0;
   out_2099470364484387366[297] = 0;
   out_2099470364484387366[298] = 0;
   out_2099470364484387366[299] = 0;
   out_2099470364484387366[300] = 0;
   out_2099470364484387366[301] = 0;
   out_2099470364484387366[302] = 0;
   out_2099470364484387366[303] = 0;
   out_2099470364484387366[304] = 1;
   out_2099470364484387366[305] = 0;
   out_2099470364484387366[306] = 0;
   out_2099470364484387366[307] = 0;
   out_2099470364484387366[308] = 0;
   out_2099470364484387366[309] = 0;
   out_2099470364484387366[310] = 0;
   out_2099470364484387366[311] = 0;
   out_2099470364484387366[312] = 0;
   out_2099470364484387366[313] = 0;
   out_2099470364484387366[314] = 0;
   out_2099470364484387366[315] = 0;
   out_2099470364484387366[316] = 0;
   out_2099470364484387366[317] = 0;
   out_2099470364484387366[318] = 0;
   out_2099470364484387366[319] = 0;
   out_2099470364484387366[320] = 0;
   out_2099470364484387366[321] = 0;
   out_2099470364484387366[322] = 0;
   out_2099470364484387366[323] = 1;
}
void h_4(double *state, double *unused, double *out_3570747994735782498) {
   out_3570747994735782498[0] = state[6] + state[9];
   out_3570747994735782498[1] = state[7] + state[10];
   out_3570747994735782498[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_6537679512233298104) {
   out_6537679512233298104[0] = 0;
   out_6537679512233298104[1] = 0;
   out_6537679512233298104[2] = 0;
   out_6537679512233298104[3] = 0;
   out_6537679512233298104[4] = 0;
   out_6537679512233298104[5] = 0;
   out_6537679512233298104[6] = 1;
   out_6537679512233298104[7] = 0;
   out_6537679512233298104[8] = 0;
   out_6537679512233298104[9] = 1;
   out_6537679512233298104[10] = 0;
   out_6537679512233298104[11] = 0;
   out_6537679512233298104[12] = 0;
   out_6537679512233298104[13] = 0;
   out_6537679512233298104[14] = 0;
   out_6537679512233298104[15] = 0;
   out_6537679512233298104[16] = 0;
   out_6537679512233298104[17] = 0;
   out_6537679512233298104[18] = 0;
   out_6537679512233298104[19] = 0;
   out_6537679512233298104[20] = 0;
   out_6537679512233298104[21] = 0;
   out_6537679512233298104[22] = 0;
   out_6537679512233298104[23] = 0;
   out_6537679512233298104[24] = 0;
   out_6537679512233298104[25] = 1;
   out_6537679512233298104[26] = 0;
   out_6537679512233298104[27] = 0;
   out_6537679512233298104[28] = 1;
   out_6537679512233298104[29] = 0;
   out_6537679512233298104[30] = 0;
   out_6537679512233298104[31] = 0;
   out_6537679512233298104[32] = 0;
   out_6537679512233298104[33] = 0;
   out_6537679512233298104[34] = 0;
   out_6537679512233298104[35] = 0;
   out_6537679512233298104[36] = 0;
   out_6537679512233298104[37] = 0;
   out_6537679512233298104[38] = 0;
   out_6537679512233298104[39] = 0;
   out_6537679512233298104[40] = 0;
   out_6537679512233298104[41] = 0;
   out_6537679512233298104[42] = 0;
   out_6537679512233298104[43] = 0;
   out_6537679512233298104[44] = 1;
   out_6537679512233298104[45] = 0;
   out_6537679512233298104[46] = 0;
   out_6537679512233298104[47] = 1;
   out_6537679512233298104[48] = 0;
   out_6537679512233298104[49] = 0;
   out_6537679512233298104[50] = 0;
   out_6537679512233298104[51] = 0;
   out_6537679512233298104[52] = 0;
   out_6537679512233298104[53] = 0;
}
void h_10(double *state, double *unused, double *out_8780416830356704886) {
   out_8780416830356704886[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_8780416830356704886[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_8780416830356704886[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_1231757349924805269) {
   out_1231757349924805269[0] = 0;
   out_1231757349924805269[1] = 9.8100000000000005*cos(state[1]);
   out_1231757349924805269[2] = 0;
   out_1231757349924805269[3] = 0;
   out_1231757349924805269[4] = -state[8];
   out_1231757349924805269[5] = state[7];
   out_1231757349924805269[6] = 0;
   out_1231757349924805269[7] = state[5];
   out_1231757349924805269[8] = -state[4];
   out_1231757349924805269[9] = 0;
   out_1231757349924805269[10] = 0;
   out_1231757349924805269[11] = 0;
   out_1231757349924805269[12] = 1;
   out_1231757349924805269[13] = 0;
   out_1231757349924805269[14] = 0;
   out_1231757349924805269[15] = 1;
   out_1231757349924805269[16] = 0;
   out_1231757349924805269[17] = 0;
   out_1231757349924805269[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_1231757349924805269[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_1231757349924805269[20] = 0;
   out_1231757349924805269[21] = state[8];
   out_1231757349924805269[22] = 0;
   out_1231757349924805269[23] = -state[6];
   out_1231757349924805269[24] = -state[5];
   out_1231757349924805269[25] = 0;
   out_1231757349924805269[26] = state[3];
   out_1231757349924805269[27] = 0;
   out_1231757349924805269[28] = 0;
   out_1231757349924805269[29] = 0;
   out_1231757349924805269[30] = 0;
   out_1231757349924805269[31] = 1;
   out_1231757349924805269[32] = 0;
   out_1231757349924805269[33] = 0;
   out_1231757349924805269[34] = 1;
   out_1231757349924805269[35] = 0;
   out_1231757349924805269[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_1231757349924805269[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_1231757349924805269[38] = 0;
   out_1231757349924805269[39] = -state[7];
   out_1231757349924805269[40] = state[6];
   out_1231757349924805269[41] = 0;
   out_1231757349924805269[42] = state[4];
   out_1231757349924805269[43] = -state[3];
   out_1231757349924805269[44] = 0;
   out_1231757349924805269[45] = 0;
   out_1231757349924805269[46] = 0;
   out_1231757349924805269[47] = 0;
   out_1231757349924805269[48] = 0;
   out_1231757349924805269[49] = 0;
   out_1231757349924805269[50] = 1;
   out_1231757349924805269[51] = 0;
   out_1231757349924805269[52] = 0;
   out_1231757349924805269[53] = 1;
}
void h_13(double *state, double *unused, double *out_3296472567236440553) {
   out_3296472567236440553[0] = state[3];
   out_3296472567236440553[1] = state[4];
   out_3296472567236440553[2] = state[5];
}
void H_13(double *state, double *unused, double *out_8075309098173729488) {
   out_8075309098173729488[0] = 0;
   out_8075309098173729488[1] = 0;
   out_8075309098173729488[2] = 0;
   out_8075309098173729488[3] = 1;
   out_8075309098173729488[4] = 0;
   out_8075309098173729488[5] = 0;
   out_8075309098173729488[6] = 0;
   out_8075309098173729488[7] = 0;
   out_8075309098173729488[8] = 0;
   out_8075309098173729488[9] = 0;
   out_8075309098173729488[10] = 0;
   out_8075309098173729488[11] = 0;
   out_8075309098173729488[12] = 0;
   out_8075309098173729488[13] = 0;
   out_8075309098173729488[14] = 0;
   out_8075309098173729488[15] = 0;
   out_8075309098173729488[16] = 0;
   out_8075309098173729488[17] = 0;
   out_8075309098173729488[18] = 0;
   out_8075309098173729488[19] = 0;
   out_8075309098173729488[20] = 0;
   out_8075309098173729488[21] = 0;
   out_8075309098173729488[22] = 1;
   out_8075309098173729488[23] = 0;
   out_8075309098173729488[24] = 0;
   out_8075309098173729488[25] = 0;
   out_8075309098173729488[26] = 0;
   out_8075309098173729488[27] = 0;
   out_8075309098173729488[28] = 0;
   out_8075309098173729488[29] = 0;
   out_8075309098173729488[30] = 0;
   out_8075309098173729488[31] = 0;
   out_8075309098173729488[32] = 0;
   out_8075309098173729488[33] = 0;
   out_8075309098173729488[34] = 0;
   out_8075309098173729488[35] = 0;
   out_8075309098173729488[36] = 0;
   out_8075309098173729488[37] = 0;
   out_8075309098173729488[38] = 0;
   out_8075309098173729488[39] = 0;
   out_8075309098173729488[40] = 0;
   out_8075309098173729488[41] = 1;
   out_8075309098173729488[42] = 0;
   out_8075309098173729488[43] = 0;
   out_8075309098173729488[44] = 0;
   out_8075309098173729488[45] = 0;
   out_8075309098173729488[46] = 0;
   out_8075309098173729488[47] = 0;
   out_8075309098173729488[48] = 0;
   out_8075309098173729488[49] = 0;
   out_8075309098173729488[50] = 0;
   out_8075309098173729488[51] = 0;
   out_8075309098173729488[52] = 0;
   out_8075309098173729488[53] = 0;
}
void h_14(double *state, double *unused, double *out_1949153742329726669) {
   out_1949153742329726669[0] = state[6];
   out_1949153742329726669[1] = state[7];
   out_1949153742329726669[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8826276129180881216) {
   out_8826276129180881216[0] = 0;
   out_8826276129180881216[1] = 0;
   out_8826276129180881216[2] = 0;
   out_8826276129180881216[3] = 0;
   out_8826276129180881216[4] = 0;
   out_8826276129180881216[5] = 0;
   out_8826276129180881216[6] = 1;
   out_8826276129180881216[7] = 0;
   out_8826276129180881216[8] = 0;
   out_8826276129180881216[9] = 0;
   out_8826276129180881216[10] = 0;
   out_8826276129180881216[11] = 0;
   out_8826276129180881216[12] = 0;
   out_8826276129180881216[13] = 0;
   out_8826276129180881216[14] = 0;
   out_8826276129180881216[15] = 0;
   out_8826276129180881216[16] = 0;
   out_8826276129180881216[17] = 0;
   out_8826276129180881216[18] = 0;
   out_8826276129180881216[19] = 0;
   out_8826276129180881216[20] = 0;
   out_8826276129180881216[21] = 0;
   out_8826276129180881216[22] = 0;
   out_8826276129180881216[23] = 0;
   out_8826276129180881216[24] = 0;
   out_8826276129180881216[25] = 1;
   out_8826276129180881216[26] = 0;
   out_8826276129180881216[27] = 0;
   out_8826276129180881216[28] = 0;
   out_8826276129180881216[29] = 0;
   out_8826276129180881216[30] = 0;
   out_8826276129180881216[31] = 0;
   out_8826276129180881216[32] = 0;
   out_8826276129180881216[33] = 0;
   out_8826276129180881216[34] = 0;
   out_8826276129180881216[35] = 0;
   out_8826276129180881216[36] = 0;
   out_8826276129180881216[37] = 0;
   out_8826276129180881216[38] = 0;
   out_8826276129180881216[39] = 0;
   out_8826276129180881216[40] = 0;
   out_8826276129180881216[41] = 0;
   out_8826276129180881216[42] = 0;
   out_8826276129180881216[43] = 0;
   out_8826276129180881216[44] = 1;
   out_8826276129180881216[45] = 0;
   out_8826276129180881216[46] = 0;
   out_8826276129180881216[47] = 0;
   out_8826276129180881216[48] = 0;
   out_8826276129180881216[49] = 0;
   out_8826276129180881216[50] = 0;
   out_8826276129180881216[51] = 0;
   out_8826276129180881216[52] = 0;
   out_8826276129180881216[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_7759088267961710722) {
  err_fun(nom_x, delta_x, out_7759088267961710722);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_4595892263117576086) {
  inv_err_fun(nom_x, true_x, out_4595892263117576086);
}
void pose_H_mod_fun(double *state, double *out_4980541881785532706) {
  H_mod_fun(state, out_4980541881785532706);
}
void pose_f_fun(double *state, double dt, double *out_4686085732492579326) {
  f_fun(state,  dt, out_4686085732492579326);
}
void pose_F_fun(double *state, double dt, double *out_2099470364484387366) {
  F_fun(state,  dt, out_2099470364484387366);
}
void pose_h_4(double *state, double *unused, double *out_3570747994735782498) {
  h_4(state, unused, out_3570747994735782498);
}
void pose_H_4(double *state, double *unused, double *out_6537679512233298104) {
  H_4(state, unused, out_6537679512233298104);
}
void pose_h_10(double *state, double *unused, double *out_8780416830356704886) {
  h_10(state, unused, out_8780416830356704886);
}
void pose_H_10(double *state, double *unused, double *out_1231757349924805269) {
  H_10(state, unused, out_1231757349924805269);
}
void pose_h_13(double *state, double *unused, double *out_3296472567236440553) {
  h_13(state, unused, out_3296472567236440553);
}
void pose_H_13(double *state, double *unused, double *out_8075309098173729488) {
  H_13(state, unused, out_8075309098173729488);
}
void pose_h_14(double *state, double *unused, double *out_1949153742329726669) {
  h_14(state, unused, out_1949153742329726669);
}
void pose_H_14(double *state, double *unused, double *out_8826276129180881216) {
  H_14(state, unused, out_8826276129180881216);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
