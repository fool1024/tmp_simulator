#include "Accelerator.h"

extern int clk;
extern int rst_rd;

void Apply_Iteration_End_Single(int Core_ID,
                                int Front_Active_V_ID, int Front_Active_V_Updated, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid, int Front_Iteration_End, int Front_Iteration_End_DValid,

                                int *Active_V_ID, int *Active_V_Updated, int *Active_V_Pull_First_Flag, int *Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid);

void Apply_Iteration_End(int Front_Active_V_ID[], int Front_Active_V_Updated[], int Front_Active_V_Pull_First_Flag[], int Front_Active_V_DValid[], int Front_Iteration_End[], int Front_Iteration_End_DValid[],

                         int *Active_V_ID, int *Active_V_Updated, int *Active_V_Pull_First_Flag, int *Active_V_DValid,int *Iteration_End, int *Iteration_End_DValid) {

    int all_front_iteration_end;

    all_front_iteration_end = Front_Iteration_End[0];
    for (int i = 1; i < CORE_NUM; ++ i) {
        all_front_iteration_end = all_front_iteration_end && Front_Iteration_End[i];
    }

    for (int i = 0; i < CORE_NUM; ++ i) {
        Apply_Iteration_End_Single(i,
                                   Front_Active_V_ID[i], Front_Active_V_Updated[i], Front_Active_V_Pull_First_Flag[i], Front_Active_V_DValid[i], all_front_iteration_end, Front_Iteration_End_DValid[i],

                                   &Active_V_ID[i], &Active_V_Updated[i], &Active_V_Pull_First_Flag[i], &Active_V_DValid[i], &Iteration_End[i], &Iteration_End_DValid[i]);
    }
}

void Apply_Iteration_End_Single(int Core_ID,
                                int Front_Active_V_ID, int Front_Active_V_Updated, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid, int Front_Iteration_End, int Front_Iteration_End_DValid,

                                int *Active_V_ID, int *Active_V_Updated, int *Active_V_Pull_First_Flag, int *Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid) {

    if (rst_rd || !Front_Active_V_DValid) {
        *Active_V_ID = 0;
        *Active_V_Updated = 0;
        *Active_V_Pull_First_Flag = 0;
        *Active_V_DValid = 0;
    }
    else {
        *Active_V_ID = Front_Active_V_ID;
        *Active_V_Updated = Front_Active_V_Updated;
        *Active_V_Pull_First_Flag = Front_Active_V_Pull_First_Flag;
        *Active_V_DValid = 1;
    }

    if (rst_rd || !Front_Iteration_End_DValid) {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }
    else {
        *Iteration_End = Front_Iteration_End;
        *Iteration_End_DValid = 1;
    }
}