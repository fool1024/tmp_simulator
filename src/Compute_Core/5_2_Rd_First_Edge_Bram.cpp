#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_Rd_First_Edge_Bram debug_M5_2;
#endif

extern int clk;
extern int rst_rd;


using namespace std;

void Rd_First_Edge_Bram_Single(int Core_ID,
                               int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Rd_Edge_Valid,
                               int Bram_Data, int Bram_DValid,
                               int Front_Iteration_End, int Front_Iteration_End_DValid,
                               int NextStage_Full,

                               int *Stage_Full,
                               int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *Active_V_Edge, int *Active_V_DValid,
                               int *Iteration_End, int *Iteration_End_DValid);

void BRAM_IP(int Core_ID,
             int Front_Rd_Edge_Addr, int Front_Rd_Edge_Valid,

             int *Bram_Data, int *Bram_DValid);

void Rd_First_Edge_Bram(int Front_Rd_Edge_Addr[], int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Rd_Edge_Valid[],
                        int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                        int NextStage_Full[],
                        int edge_bram_data[], int edge_bram_dvalid[],
                        
                        int *Stage_Full,
                        int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *Active_V_Edge, int *Active_V_DValid,
                        int *Iteration_End, int *Iteration_End_DValid) {

    static int tmp_bram_data[CORE_NUM], tmp_bram_dvalid[CORE_NUM];

    for (int i = 0; i < CORE_NUM; ++ i) {
        Rd_First_Edge_Bram_Single(i,
                                  Front_Push_Flag[i], Front_Active_V_ID[i], Front_Active_V_Value[i], Front_Rd_Edge_Valid[i],
                                  edge_bram_data[i], edge_bram_dvalid[i],
                                  Front_Iteration_End[i], Front_Iteration_End_DValid[i],
                                  NextStage_Full[i],

                                  &Stage_Full[i],
                                  &Push_Flag[i], &Active_V_ID[i], &Active_V_Value[i], &Active_V_Edge[i], &Active_V_DValid[i],
                                  &Iteration_End[i], &Iteration_End_DValid[i]);

    }
}

void Rd_First_Edge_Bram_Single(int Core_ID,
                               int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Rd_Edge_Valid,
                               int Bram_Data, int Bram_DValid,
                               int Front_Iteration_End, int Front_Iteration_End_DValid,
                               int NextStage_Full,

                               int *Stage_Full,
                               int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *Active_V_Edge, int *Active_V_DValid,
                               int *Iteration_End, int *Iteration_End_DValid) {
    static queue<int> push_flag_buffer[CORE_NUM], v_id_buffer[CORE_NUM], v_value_buffer[CORE_NUM];
    static queue<int> edge_buffer[CORE_NUM];

    #if DEBUG
        debug_M5_2._push_flag_buffer = push_flag_buffer;
        debug_M5_2._v_id_buffer = v_id_buffer;
        debug_M5_2._v_value_buffer = v_value_buffer;
        debug_M5_2._edge_buffer = edge_buffer;
    #endif

    static int v_buffer_empty[CORE_NUM];
    static int e_buffer_empty[CORE_NUM];

    v_buffer_empty[Core_ID] = (v_id_buffer[Core_ID].size() == 0);
    e_buffer_empty[Core_ID] = (edge_buffer[Core_ID].size() == 0);

    if (rst_rd) {
        /* initialize push_flag, v_id, v_value buffers */
        while (!push_flag_buffer[Core_ID].empty())    push_flag_buffer[Core_ID].pop();
        while (!v_id_buffer[Core_ID].empty())         v_id_buffer[Core_ID].pop();
        while (!v_value_buffer[Core_ID].empty())      v_value_buffer[Core_ID].pop();
    }
    else {
        if (Front_Rd_Edge_Valid) {
            push_flag_buffer[Core_ID].push(Front_Push_Flag);
            v_id_buffer[Core_ID].push(Front_Active_V_ID);
            v_value_buffer[Core_ID].push(Front_Active_V_Value);
        }
    }

    if (rst_rd) {
        /* initialize edge buffer */
        while (!edge_buffer[Core_ID].empty())     edge_buffer[Core_ID].pop();
    }
    else {
        if (Bram_DValid) {
            edge_buffer[Core_ID].push(Bram_Data);
        }
        /*
        else if(Bram_DValid && Bram_Data == -1){
            push_flag_buffer[Core_ID].pop();
            v_id_buffer[Core_ID].pop();
            v_value_buffer[Core_ID].pop();
        }
        */
    }

    if (rst_rd || NextStage_Full || v_buffer_empty[Core_ID] || e_buffer_empty[Core_ID]) {
        *Push_Flag = 0;
        *Active_V_ID = 0;
        *Active_V_Value = 0;
        *Active_V_Edge = 0;
        *Active_V_DValid = 0;
    }
    else {
        *Push_Flag = push_flag_buffer[Core_ID].front();
        *Active_V_ID = v_id_buffer[Core_ID].front();
        *Active_V_Value = v_value_buffer[Core_ID].front();
        *Active_V_Edge = edge_buffer[Core_ID].front();
        *Active_V_DValid = 1;
        #if (DEBUG && PRINT_CONS)
            if (*Active_V_ID == PRINT_ID) {
                printf("clk=%d, push_flag=%d, active_v_id=%d, active_v_value=%d, active_v_edge=%d, active_v_dvalid=%d\n", clk, *Push_Flag, *Active_V_ID, *Active_V_Value, *Active_V_Edge, *Active_V_DValid);
            }
        #endif

        push_flag_buffer[Core_ID].pop();
        v_id_buffer[Core_ID].pop();
        v_value_buffer[Core_ID].pop();
        edge_buffer[Core_ID].pop();
    }

    if (!rst_rd && Front_Iteration_End && Front_Iteration_End_DValid && v_buffer_empty[Core_ID] && !Front_Rd_Edge_Valid) {
        *Iteration_End = 1;
        *Iteration_End_DValid = 1;
    }
    else {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }


    *Stage_Full = (v_id_buffer[Core_ID].size() >= FIFO_SIZE);
}

