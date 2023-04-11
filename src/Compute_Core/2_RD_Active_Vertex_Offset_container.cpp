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

void Read_Active_Vertex_Offset(Info_P12P2 self_info_p12p2,int NextStage_Full[],

                               int *Stage_Full,
                               int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *RD_Active_V_Offset_Addr, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                               int *Iteration_End, int *Iteration_End_DValid) {

    for (int i = 0; i < CORE_NUM; ++ i) {
        Read_Active_Vertex_Offset_Single(i,
                                         self_info_p12p2.P1_Push_Flag[i], self_info_p12p2.P1_Active_V_ID[i], self_info_p12p2.P1_Active_V_Value[i], self_info_p12p2.P1_Active_V_Pull_First_Flag[i], self_info_p12p2.P1_Active_V_DValid[i],
                                         self_info_p12p2.P1_Iteration_End[i], self_info_p12p2.P1_Iteration_End_Dvalid[i],
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

    static queue_container<int> v_container[CORE_NUM];
    static value_container<int> Front_V_container[CORE_NUM],V_container[CORE_NUM],Active_V_container[CORE_NUM];


    if(clk == 0){
        if( Core_ID == 0 )
        for(int i = 0 ; i<CORE_NUM ; i++){
            v_container[i].combine({&push_flag_buffer[i], &v_id_buffer[i], &v_value_buffer[i],&pull_first_flag_buffer[i]});
        }
        Front_V_container[Core_ID].combine({&Front_Push_Flag, &Front_Active_V_ID, &Front_Active_V_Value, &Front_Active_V_Pull_First_Flag});
        V_container[Core_ID].combine({Push_Flag, Active_V_ID, Active_V_Value, Active_V_Pull_First_Flag});
        Active_V_container[Core_ID].combine({RD_Active_V_Offset_Addr, Active_V_DValid});
    }

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
            v_container[Core_ID].push(Front_V_container[Core_ID]);
        }
    }

    if (rst_rd) {
        V_container[Core_ID].assign({-1,0,0,0});
        Active_V_container[Core_ID].assign({0,0});
        
    }
    else {
        if (NextStage_Full || v_buffer_empty[Core_ID]) {
            V_container[Core_ID].assign({-1,0,0,0});
            Active_V_container[Core_ID].assign({0,0});
        }
        else {
            V_container[Core_ID].assign(v_container[Core_ID]);
            Active_V_container[Core_ID].assign({v_id_buffer[Core_ID].front() / CORE_NUM,1});
            
            #if (DEBUG && PRINT_CONS)
                if (*Active_V_ID == PRINT_ID) {
                    printf("clk=%d, rd_active_addr=%d, push_flag=%d, active_v_id=%d, active_v_value=%d, active_v_pull_first_flag=%d, active_v_dvalid=%d\n", clk, *RD_Active_V_Offset_Addr, *Push_Flag, *Active_V_ID, *Active_V_Value, *Active_V_Pull_First_Flag, *Active_V_DValid);
                }
            #endif
            
            v_container[Core_ID].pop();
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