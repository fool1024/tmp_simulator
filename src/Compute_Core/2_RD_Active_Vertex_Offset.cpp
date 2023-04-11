#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_RD_ACTIVE_VERTEX_OFFSET debug_M2;
#endif


extern int clk;
extern int rst_rd;

using namespace std;

void Read_Active_Vertex_Offset_Single(int Core_ID,
                                      int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid,
                                      int Front_Iteration_End, int Front_Ieration_End_DValid,
                                      int NextStage_Full,

                                      int *Stage_Full,
                                      int *Push_Flag,
                                      int *Active_V_ID, int *Active_V_Value, int *RD_Active_V_Offset_Addr, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                                      int *Iteration_End, int *Iteration_End_DValid);

void Read_Active_Vertex_Offset(int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Active_V_Pull_First_Flag[], int Front_Active_V_DValid[], 
                               int Front_Iteration_End[], int Front_Iteration_End_DValid[],int NextStage_Full[],

                               int *Stage_Full,
                               int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *RD_Active_V_Offset_Addr, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                               int *Iteration_End, int *Iteration_End_DValid) {

    for (int i = 0; i < CORE_NUM; ++ i) {
        Read_Active_Vertex_Offset_Single(i,
                                         Front_Push_Flag[i], Front_Active_V_ID[i], Front_Active_V_Value[i], Front_Active_V_Pull_First_Flag[i], Front_Active_V_DValid[i],
                                         Front_Iteration_End[i], Front_Iteration_End_DValid[i],
                                         NextStage_Full[i],

                                         &Stage_Full[i],
                                         &Push_Flag[i], &Active_V_ID[i], &Active_V_Value[i], &RD_Active_V_Offset_Addr[i], &Active_V_Pull_First_Flag[i], &Active_V_DValid[i],
                                         &Iteration_End[i], &Iteration_End_DValid[i]);
    }
}



void Read_Active_Vertex_Offset_Single(int Core_ID,
                                      int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid,
                                      int Front_Iteration_End, int Front_Iteration_End_DValid,
                                      int NextStage_Full,

                                      int *Stage_Full,
                                      int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *RD_Active_V_Offset_Addr, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                                      int *Iteration_End, int *Iteration_End_DValid) {

    static queue<int> push_flag_buffer[CORE_NUM], v_id_buffer[CORE_NUM], v_value_buffer[CORE_NUM], pull_first_flag_buffer[CORE_NUM];
    //static queue<int> push_flag_buffer[CORE_NUM], v_id_buffer[CORE_NUM], v_value_buffer[CORE_NUM], pull_first_flag_buffer[CORE_NUM];
    static int v_buffer_empty[CORE_NUM], v_buffer_full[CORE_NUM];

    #if DEBUG
        debug_M2._push_flag_buffer = push_flag_buffer;
        debug_M2._v_id_buffer = v_id_buffer;
        debug_M2._v_value_buffer = v_value_buffer;
        debug_M2._pull_first_flag_buffer = pull_first_flag_buffer;
    #endif

    v_buffer_empty[Core_ID] = (v_id_buffer[Core_ID].size() == 0);
    v_buffer_full[Core_ID] = (v_id_buffer[Core_ID].size() >= FIFO_SIZE);

    ///active vertex management
    if (rst_rd) {
        /*initialize push_flag_queue, v_id_queue, v_value_queue, pull_first_flag_queue*/
        while (!push_flag_buffer[Core_ID].empty())        push_flag_buffer[Core_ID].pop();
        while (!v_id_buffer[Core_ID].empty())             v_id_buffer[Core_ID].pop();
        while (!v_value_buffer[Core_ID].empty())          v_value_buffer[Core_ID].pop();
        while (!pull_first_flag_buffer[Core_ID].empty())  pull_first_flag_buffer[Core_ID].pop();
    }
    else {
        if (Front_Active_V_DValid) {
            push_flag_buffer[Core_ID].push(Front_Push_Flag); 
            v_id_buffer[Core_ID].push(Front_Active_V_ID); 
            v_value_buffer[Core_ID].push(Front_Active_V_Value); 
            pull_first_flag_buffer[Core_ID].push(Front_Active_V_Pull_First_Flag);
        }
    }

    if (rst_rd) {
        *Push_Flag = -1;
        *Active_V_ID = 0;
        *Active_V_Value = 0;
        *RD_Active_V_Offset_Addr = 0;
        *Active_V_Pull_First_Flag = 0;
        *Active_V_DValid = 0;
    }
    else {
        if (NextStage_Full || v_buffer_empty[Core_ID]) {
            *Push_Flag = -1;
            *Active_V_ID = 0;
            *Active_V_Value = 0;
            *RD_Active_V_Offset_Addr = 0;
            *Active_V_DValid = 0;
        }
        else {
            *Push_Flag = push_flag_buffer[Core_ID].front();
            *Active_V_ID = v_id_buffer[Core_ID].front();
            *Active_V_Value = v_value_buffer[Core_ID].front();
            *RD_Active_V_Offset_Addr = v_id_buffer[Core_ID].front() / CORE_NUM;
            *Active_V_Pull_First_Flag = pull_first_flag_buffer[Core_ID].front();
            *Active_V_DValid = 1;
            #if (DEBUG && PRINT_CONS)
                if (*Active_V_ID == PRINT_ID) {
                    printf("clk=%d, rd_active_addr=%d, push_flag=%d, active_v_id=%d, active_v_value=%d, active_v_pull_first_flag=%d, active_v_dvalid=%d\n", clk, *RD_Active_V_Offset_Addr, *Push_Flag, *Active_V_ID, *Active_V_Value, *Active_V_Pull_First_Flag, *Active_V_DValid);
                }
            #endif
            
            push_flag_buffer[Core_ID].pop(); 
            v_id_buffer[Core_ID].pop(); 
            v_value_buffer[Core_ID].pop(); 
            pull_first_flag_buffer[Core_ID].pop();
        }
    }

    ///iteration end management, front_active_v_dvalid is used to avoid early end
    if (!rst_rd && Front_Iteration_End && Front_Iteration_End_DValid && v_buffer_empty[Core_ID] && !Front_Active_V_DValid) {
        *Iteration_End = 1;
        *Iteration_End_DValid = 1;
    }
    else {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }


    //assign stage_full = v_id_bufer_full
    *Stage_Full = (v_id_buffer[Core_ID].size() >= FIFO_SIZE);
}