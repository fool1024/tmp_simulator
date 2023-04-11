#ifndef PARA_H
#include "para.h"
#endif

#include <queue>
#include <cstdio>

extern int clk;
extern int rst_rd;

using namespace std;

#define Network_Para int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],\
                   int Next_Stage_Full[],\
                    \
                   int *Stage_Full_Om,\
                   int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid

struct Network_reg{
    //omega network 
    int tmp_om_push_flag_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_id_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_value_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_pull_first_flag_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_valid_in[OMEGA_DEPTH + 1][CORE_NUM],
        tmp_om_iteration_end[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_iteration_end_dvalid[OMEGA_DEPTH + 1][CORE_NUM];

    int tmp_om_stage_full[OMEGA_DEPTH + 1][CORE_NUM];

    int buffer_full_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], buffer_full_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM];
    int buffer_full_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], buffer_full_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM];
    
    //###################### omega network
    queue<int> push_flag_buffer_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], id_buffer_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], value_buffer_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], pull_first_flag_buffer_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM]; //原OMEGA_DEPTH+1+1 为 OMEGA_DEPTH+1
    queue<int> push_flag_buffer_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], id_buffer_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], value_buffer_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], pull_first_flag_buffer_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM]; //原OMEGA_DEPTH+1+1 为 OMEGA_DEPTH+1
    queue<int> push_flag_buffer_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], id_buffer_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], value_buffer_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], pull_first_flag_buffer_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM]; //原OMEGA_DEPTH+1+1 为 OMEGA_DEPTH+1
    queue<int> push_flag_buffer_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], id_buffer_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], value_buffer_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], pull_first_flag_buffer_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM]; //原OMEGA_DEPTH+1+1 为 OMEGA_DEPTH+1
    //######################
};

