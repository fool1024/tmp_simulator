#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_Omega_Network debug_M8;
#endif

extern int clk;
extern int rst_rd;

using namespace std;

#define Network_Para int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],\
                   int Next_Stage_Full[],\
                    \
                   int *Stage_Full_Om,\
                   int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid



void Omega_Network_Single(int Omega_Depth_ID, int Omega_Switch_ID,
                            int Push_Flag1, int V_ID_In1, int V_Value_In1, int Pull_First_Flag1, int V_Valid_In1, int Iteration_End_In1, int Iteration_End_DValid_In1,
                            int Push_Flag2, int V_ID_In2, int V_Value_In2, int Pull_First_Flag2, int V_Valid_In2, int Iteration_End_In2, int Iteration_End_DValid_In2,
                            int Next_Stage_Full1, int Next_Stage_Full2,
                            int switch_router,

                            int *Stage_Full1, int *Stage_Full2,
                            int *Om1_Push_Flag_Out1, int *Om1_V_ID_Out1, int *Om1_V_Value_Out1, int *Om1_Pull_First_Flag1, int *Om1_V_Valid_Out1, int *Om1_Iteration_End_Out1, int *Om1_Iteration_End_DValid_Out1,
                            int *Om1_Push_Flag_Out2, int *Om1_V_ID_Out2, int *Om1_V_Value_Out2, int *Om1_Pull_First_Flag2, int *Om1_V_Valid_Out2, int *Om1_Iteration_End_Out2, int *Om1_Iteration_End_DValid_Out2,
                            Network_reg& Nreg);

void Omega_Network(int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],
                   int Next_Stage_Full[],
                    
                   int *Stage_Full_Om,
                   int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid, Network_reg& Nreg,
                   int switch_router);

void Network(int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],
             int Next_Stage_Full[],
             int *Stage_Full_Om,
             int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid, Network_reg& Nreg, 
             int switch_router, int network_num){
    if( network_num ==  0)
        Omega_Network(Om_Front_Push_Flag, Om_Front_Update_V_ID, Om_Front_Update_V_Value, Om_Front_Pull_First_Flag, Om_Front_Update_V_DValid, Om_Front_Iteration_End, Om_Front_Iteration_End_DValid,
                    Next_Stage_Full,

                    Stage_Full_Om,
                    Om_Push_Flag, Om_Update_V_ID, Om_Update_V_Value, Om_Pull_First_Flag, Om_Update_V_DValid, Om_Iteration_End, Om_Iteration_End_DValid,

                    Nreg, switch_router);

}

void Omega_Network(int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],
                   int Next_Stage_Full[],
                   int *Stage_Full_Om,
                   int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid, Network_reg& Nreg,
                   int switch_router) {
    // how to solve the condition keeping
    // static int tmp_om_push_flag_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_id_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_value_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_pull_first_flag_in[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_v_valid_in[OMEGA_DEPTH + 1][CORE_NUM],
    //     tmp_om_iteration_end[OMEGA_DEPTH + 1][CORE_NUM], tmp_om_iteration_end_dvalid[OMEGA_DEPTH + 1][CORE_NUM];

    // static int tmp_om_stage_full[OMEGA_DEPTH + 1][CORE_NUM];

    //logic
    for (int i = 0; i < CORE_NUM; ++ i) {
        Nreg.tmp_om_push_flag_in[0][i] = Om_Front_Push_Flag[i];
        Nreg.tmp_om_v_id_in[0][i] = Om_Front_Update_V_ID[i];
        Nreg.tmp_om_v_value_in[0][i] = Om_Front_Update_V_Value[i];
        Nreg.tmp_om_pull_first_flag_in[0][i] = Om_Front_Pull_First_Flag[i];
        Nreg.tmp_om_v_valid_in[0][i] = Om_Front_Update_V_DValid[i];
        Nreg.tmp_om_iteration_end[0][i] = Om_Front_Iteration_End[i];
        Nreg.tmp_om_iteration_end_dvalid[0][i] = Om_Front_Iteration_End_DValid[i];
        Nreg.tmp_om_stage_full[OMEGA_DEPTH][i] = Next_Stage_Full[i];

    }


    for (int i = OMEGA_DEPTH; i >= 1; -- i) {
        for (int j = 0; j < OMEGA_SWITCH_NUM; ++ j) {
            int in_id1 = j;
            int in_id2 = j + OMEGA_SWITCH_NUM;
            int out_id1 = j * 2;
            int out_id2 = j * 2 + 1;
            Omega_Network_Single(i - 1, j,
                                Nreg.tmp_om_push_flag_in[i - 1][in_id1], Nreg.tmp_om_v_id_in[i - 1][in_id1], Nreg.tmp_om_v_value_in[i - 1][in_id1], Nreg.tmp_om_pull_first_flag_in[i - 1][in_id1], Nreg.tmp_om_v_valid_in[i - 1][in_id1], Nreg.tmp_om_iteration_end[i - 1][in_id1], Nreg.tmp_om_iteration_end_dvalid[i - 1][in_id1],
                                Nreg.tmp_om_push_flag_in[i - 1][in_id2], Nreg.tmp_om_v_id_in[i - 1][in_id2], Nreg.tmp_om_v_value_in[i - 1][in_id2], Nreg.tmp_om_pull_first_flag_in[i - 1][in_id2], Nreg.tmp_om_v_valid_in[i - 1][in_id2], Nreg.tmp_om_iteration_end[i - 1][in_id2], Nreg.tmp_om_iteration_end_dvalid[i - 1][in_id2],
                                Nreg.tmp_om_stage_full[i][out_id1], Nreg.tmp_om_stage_full[i][out_id2],
                                switch_router,

                                &Nreg.tmp_om_stage_full[i - 1][in_id1], &Nreg.tmp_om_stage_full[i - 1][in_id2],
                                &Nreg.tmp_om_push_flag_in[i][out_id1], &Nreg.tmp_om_v_id_in[i][out_id1], &Nreg.tmp_om_v_value_in[i][out_id1], &Nreg.tmp_om_pull_first_flag_in[i][out_id1], &Nreg.tmp_om_v_valid_in[i][out_id1], &Nreg.tmp_om_iteration_end[i][out_id1], &Nreg.tmp_om_iteration_end_dvalid[i][out_id1],
                                &Nreg.tmp_om_push_flag_in[i][out_id2], &Nreg.tmp_om_v_id_in[i][out_id2], &Nreg.tmp_om_v_value_in[i][out_id2], &Nreg.tmp_om_pull_first_flag_in[i][out_id2], &Nreg.tmp_om_v_valid_in[i][out_id2], &Nreg.tmp_om_iteration_end[i][out_id2], &Nreg.tmp_om_iteration_end_dvalid[i][out_id2],
                                Nreg);

        }
    }

    for (int i = 0; i < CORE_NUM; ++ i) {
        Stage_Full_Om[i] = Nreg.tmp_om_stage_full[0][i];
    }

    for (int i = 0; i < CORE_NUM; ++ i) {
        Om_Push_Flag[i] = Nreg.tmp_om_push_flag_in[OMEGA_DEPTH][i];
        Om_Update_V_ID[i] = Nreg.tmp_om_v_id_in[OMEGA_DEPTH][i];
        Om_Update_V_Value[i] = Nreg.tmp_om_v_value_in[OMEGA_DEPTH][i];
        Om_Pull_First_Flag[i] = Nreg.tmp_om_pull_first_flag_in[OMEGA_DEPTH][i];
        Om_Update_V_DValid[i] = Nreg.tmp_om_v_valid_in[OMEGA_DEPTH][i];
        Om_Iteration_End[i] = Nreg.tmp_om_iteration_end[OMEGA_DEPTH][i];
        Om_Iteration_End_DValid[i] = Nreg.tmp_om_iteration_end_dvalid[OMEGA_DEPTH][i];
    }
}

void Omega_Network_Single(int Omega_Depth_ID, int Omega_Switch_ID,
                          int Push_Flag1, int V_ID_In1, int V_Value_In1, int Pull_First_Flag1, int V_Valid_In1, int Iteration_End_In1, int Iteration_End_DValid_In1,
                          int Push_Flag2, int V_ID_In2, int V_Value_In2, int Pull_First_Flag2, int V_Valid_In2, int Iteration_End_In2, int Iteration_End_DValid_In2,
                          int Next_Stage_Full1, int Next_Stage_Full2,
                          int switch_router,

                          int *Stage_Full1, int *Stage_Full2,
                          int *Om1_Push_Flag_Out1, int *Om1_V_ID_Out1, int *Om1_V_Value_Out1, int *Om1_Pull_First_Flag1, int *Om1_V_Valid_Out1, int *Om1_Iteration_End_Out1, int *Om1_Iteration_End_DValid_Out1,
                          int *Om1_Push_Flag_Out2, int *Om1_V_ID_Out2, int *Om1_V_Value_Out2, int *Om1_Pull_First_Flag2, int *Om1_V_Valid_Out2, int *Om1_Iteration_End_Out2, int *Om1_Iteration_End_DValid_Out2,
                          Network_reg& Nreg) {

    
    static int buffer_empty_in1_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], buffer_empty_in1_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM];
    static int buffer_empty_in2_out1[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM], buffer_empty_in2_out2[OMEGA_DEPTH+1][OMEGA_SWITCH_NUM];
    static int array2n[10];
    #if DEBUG
        debug_M8._om1_push_flag_buffer_in1_out1 = push_flag_buffer_in1_out1;
        debug_M8._om1_id_buffer_in1_out1 = id_buffer_in1_out1;
        debug_M8._om1_value_buffer_in1_out1 = value_buffer_in1_out1;
        debug_M8._om1_push_flag_buffer_in2_out1 = push_flag_buffer_in2_out1;
        debug_M8._om1_id_buffer_in2_out1 = id_buffer_in2_out1;
        debug_M8._om1_value_buffer_in2_out1 = value_buffer_in2_out1;
        debug_M8._om1_push_flag_buffer_in1_out2 = push_flag_buffer_in1_out2;
        debug_M8._om1_id_buffer_in1_out2 = id_buffer_in1_out2;
        debug_M8._om1_value_buffer_in1_out2 = value_buffer_in1_out2;
        debug_M8._om1_push_flag_buffer_in2_out2 = push_flag_buffer_in2_out2;
        debug_M8._om1_id_buffer_in2_out2 = id_buffer_in2_out2;
        debug_M8._om1_value_buffer_in2_out2 = value_buffer_in2_out2;
    #endif

    int in1_src_route, in2_src_route;
    int in1_dst_route,in2_dst_route;
    int in1_route_flag, in2_route_flag;
    int in1_port_id, in2_port_id;
    if(switch_router == 0){
        in1_src_route = 0, in2_src_route = array2n[OMEGA_DEPTH - 1];
        in1_dst_route = Push_Flag1 ? ((V_ID_In1 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]) : ((V_Value_In1 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]);
        in2_dst_route = Push_Flag2 ? ((V_ID_In2 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]) : ((V_Value_In2 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]);
        in1_route_flag = (in1_src_route ^ in1_dst_route), in2_route_flag = (in2_src_route ^ in2_dst_route);
    }else{
        in1_port_id = Omega_Switch_ID, in2_port_id = Omega_Switch_ID + OMEGA_SWITCH_NUM;
        in1_src_route = in1_port_id & array2n[OMEGA_DEPTH - 1], in2_src_route = in2_port_id & array2n[OMEGA_DEPTH - 1];
        in1_dst_route = ((V_ID_In1 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]), in2_dst_route =  ((V_ID_In2 << Omega_Depth_ID) & array2n[OMEGA_DEPTH - 1]);
        in1_route_flag = (in1_src_route ^ in1_dst_route), in2_route_flag = (in2_src_route ^ in2_dst_route);
    }


    buffer_empty_in1_out1[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).size() == 0);
    buffer_empty_in1_out2[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).size() == 0);
    buffer_empty_in2_out1[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).size() == 0);
    buffer_empty_in2_out2[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).size() == 0);

    #if DEBUG
        debug_M8._om1_push_flag_buffer_in1_out1 = push_flag_buffer_in1_out1;
        debug_M8._om1_id_buffer_in1_out1 = id_buffer_in1_out1;
        debug_M8._om1_value_buffer_in1_out1 = value_buffer_in1_out1;
        debug_M8._om1_push_flag_buffer_in2_out1 = push_flag_buffer_in2_out1;
        debug_M8._om1_id_buffer_in2_out1 = id_buffer_in2_out1;
        debug_M8._om1_value_buffer_in2_out1 = value_buffer_in2_out1;
        debug_M8._om1_push_flag_buffer_in1_out2 = push_flag_buffer_in1_out2;
        debug_M8._om1_id_buffer_in1_out2 = id_buffer_in1_out2;
        debug_M8._om1_value_buffer_in1_out2 = value_buffer_in1_out2;
        debug_M8._om1_push_flag_buffer_in2_out2 = push_flag_buffer_in2_out2;
        debug_M8._om1_id_buffer_in2_out2 = id_buffer_in2_out2;
        debug_M8._om1_value_buffer_in2_out2 = value_buffer_in2_out2;
    #endif

    if (rst_rd) {
        /*buffer initilization*/
        for (int i = 0; i < OMEGA_DEPTH; ++ i) array2n[i] = 1 << i;
        while (!(Nreg.push_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())        (Nreg.push_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.push_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())        (Nreg.push_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.push_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())        (Nreg.push_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.push_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())        (Nreg.push_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())               (Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())               (Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())               (Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())               (Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.value_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())            (Nreg.value_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.value_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())            (Nreg.value_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.value_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())            (Nreg.value_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.value_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())            (Nreg.value_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.pull_first_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())  (Nreg.pull_first_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.pull_first_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())  (Nreg.pull_first_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.pull_first_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).empty())  (Nreg.pull_first_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        while (!(Nreg.pull_first_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).empty())  (Nreg.pull_first_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
    }
    else {
        if (V_Valid_In1) {
            if (in1_route_flag == 0) {
                (Nreg.push_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).push(Push_Flag1);
                (Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).push(V_ID_In1);
                (Nreg.value_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).push(V_Value_In1);
                (Nreg.pull_first_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).push(Pull_First_Flag1);
            }
            else {
                (Nreg.push_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).push(Push_Flag1);
                (Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).push(V_ID_In1);
                (Nreg.value_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).push(V_Value_In1);
                (Nreg.pull_first_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).push(Pull_First_Flag1);
            }
        }
        if (V_Valid_In2) {
            if (in2_route_flag == 0) {
                (Nreg.push_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).push(Push_Flag2);
                (Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).push(V_ID_In2);
                (Nreg.value_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).push(V_Value_In2);
                (Nreg.pull_first_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).push(Pull_First_Flag2);
            }
            else {
                (Nreg.push_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).push(Push_Flag2);
                (Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).push(V_ID_In2);
                (Nreg.value_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).push(V_Value_In2);
                (Nreg.pull_first_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).push(Pull_First_Flag2);
            }
        }
    }

    if (rst_rd || Next_Stage_Full1) {
        *Om1_Push_Flag_Out1 = 0;
        *Om1_V_ID_Out1 = 0;
        *Om1_V_Value_Out1 = 0;
        *Om1_V_Valid_Out1 = 0;
    }
    else {
        if ((Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).size() > (Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).size()) { //in1_out1 cannot be empty
            *Om1_Push_Flag_Out1 = (Nreg.push_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_ID_Out1 = (Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_Value_Out1 = (Nreg.value_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_Pull_First_Flag1 = (Nreg.pull_first_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_Valid_Out1 = 1;

            (Nreg.push_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.value_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.pull_first_flag_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
        }
        else {
            if (buffer_empty_in2_out1[Omega_Depth_ID][Omega_Switch_ID]) {
                *Om1_Push_Flag_Out1 = 0;
                *Om1_V_ID_Out1 = 0;
                *Om1_V_Value_Out1 = 0;
                *Om1_V_Valid_Out1 = 0;
            }
            else {
                *Om1_Push_Flag_Out1 = (Nreg.push_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_ID_Out1 = (Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_Value_Out1 = (Nreg.value_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_Pull_First_Flag1 = (Nreg.pull_first_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_Valid_Out1 = 1;

                (Nreg.push_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.value_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.pull_first_flag_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).pop();
            }
        }
    }

    if (rst_rd || Next_Stage_Full2) {
        *Om1_Push_Flag_Out2 = 0;
        *Om1_V_ID_Out2 = 0;
        *Om1_V_Value_Out2 = 0;
        *Om1_V_Valid_Out2 = 0;
    }
    else {
        if ((Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).size() > (Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).size()) { //in1_out1 cannot be empty
            *Om1_Push_Flag_Out2 = (Nreg.push_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_ID_Out2 = (Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_Value_Out2 = (Nreg.value_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_Pull_First_Flag2 = (Nreg.pull_first_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
            *Om1_V_Valid_Out2 = 1;

            (Nreg.push_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.value_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
            (Nreg.pull_first_flag_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
        }
        else {
            if (buffer_empty_in2_out2[Omega_Depth_ID][Omega_Switch_ID]) {
                *Om1_Push_Flag_Out2 = 0;
                *Om1_V_ID_Out2 = 0;
                *Om1_V_Value_Out2 = 0;
                *Om1_V_Valid_Out2 = 0;
            }
            else {
                *Om1_Push_Flag_Out2 = (Nreg.push_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_ID_Out2 = (Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_Value_Out2 = (Nreg.value_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_Pull_First_Flag2 = (Nreg.pull_first_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).front();
                *Om1_V_Valid_Out2 = 1;

                (Nreg.push_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.value_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
                (Nreg.pull_first_flag_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).pop();
            }
        }
    }

    if (!rst_rd && Iteration_End_In1 && Iteration_End_DValid_In1 && buffer_empty_in1_out1[Omega_Depth_ID][Omega_Switch_ID] && buffer_empty_in2_out1[Omega_Depth_ID][Omega_Switch_ID]) {
        *Om1_Iteration_End_Out1 = 1;
        *Om1_Iteration_End_DValid_Out1 = 1;
    }
    else {
        *Om1_Iteration_End_Out1 = 0;
        *Om1_Iteration_End_DValid_Out1 = 0;
    }

    if (!rst_rd && Iteration_End_In2 && Iteration_End_DValid_In2 && buffer_empty_in1_out2[Omega_Depth_ID][Omega_Switch_ID] && buffer_empty_in2_out2[Omega_Depth_ID][Omega_Switch_ID]) {
        *Om1_Iteration_End_Out2 = 1;
        *Om1_Iteration_End_DValid_Out2 = 1;
    }
    else {
        *Om1_Iteration_End_Out2 = 0;
        *Om1_Iteration_End_DValid_Out2 = 0;
    }

    Nreg.buffer_full_in1_out1[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in1_out1[Omega_Depth_ID][Omega_Switch_ID]).size() >= OM_FIFO_SIZE);
    Nreg.buffer_full_in1_out2[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in1_out2[Omega_Depth_ID][Omega_Switch_ID]).size() >= OM_FIFO_SIZE);
    Nreg.buffer_full_in2_out1[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in2_out1[Omega_Depth_ID][Omega_Switch_ID]).size() >= OM_FIFO_SIZE);
    Nreg.buffer_full_in2_out2[Omega_Depth_ID][Omega_Switch_ID] = ((Nreg.id_buffer_in2_out2[Omega_Depth_ID][Omega_Switch_ID]).size() >= OM_FIFO_SIZE);

    *Stage_Full1 = Nreg.buffer_full_in1_out1[Omega_Depth_ID][Omega_Switch_ID] || Nreg.buffer_full_in1_out2[Omega_Depth_ID][Omega_Switch_ID];
    *Stage_Full2 = Nreg.buffer_full_in2_out1[Omega_Depth_ID][Omega_Switch_ID] || Nreg.buffer_full_in2_out2[Omega_Depth_ID][Omega_Switch_ID];
}

void Omega_Network_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}
