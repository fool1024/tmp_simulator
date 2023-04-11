#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_Backend_Core_Process debug_M9;
#endif

extern int clk;
extern int rst_rd;

using namespace std;

void Backend_Source_Core_Single(int Core_ID,
                                int Om1_Recv_Push_Flag, int Om1_Recv_Update_V_ID, int Om1_Recv_Update_V_Value, int Om1_Recv_Update_V_Pull_First_Flag, int Om1_Recv_Update_V_DValid, int Om1_Recv_Iteration_End, int Om1_Recv_Iteration_End_DValid,
                                int Vertex_BRAM_Data, int Vertex_BRAM_DValid,
                                int Om_Global_Iteration_ID,

                                int *Source_Core_Full,
                                int *Rd_Vertex_BRAM_Addr, int *Rd_Vertex_BRAM_Valid,
                                int *Om2_Send_Push_Flag, int *Om2_Send_Update_V_ID, int *Om2_Send_Update_V_Value, int *Om2_Send_Update_V_Pull_First_Flag, int *Om2_Send_Update_V_DValid, int *Om2_Send_Iteration_End, int *Om2_Send_Iteration_End_DValid);


void Backend_Destination_Core_Single(int Core_ID,
                                     int Om2_Recv_Push_Flag, int Om2_Recv_Update_V_ID, int Om2_Recv_Update_V_Value, int Om2_Recv_Update_V_Pull_First_Flag, int Om2_Recv_Update_V_DValid, int Om2_Recv_Iteration_End, int Om2_Recv_Iteration_End_DValid,

                                     int *Wr_Vertex_Bram_Push_Flag, int *Wr_Vertex_BRAM_Addr, int *Wr_Vertex_BRAM_Data, int *Wr_Vertex_BRAM_Pull_First_Flag, int *Wr_Vertex_BRAM_Valid,
                                     int *Wr_Vertex_BRAM_Iteration_End, int *Wr_Vertex_BRAM_Iteration_End_DValid);

//om1: src to dst
//om2: dst to src
void Backend_Core(int Om1_Recv_Push_Flag[], int Om1_Recv_Update_V_ID[], int Om1_Recv_Update_V_Value[], int Om1_Recv_Update_V_Pull_First_Flag[], int Om1_Recv_Update_V_DValid[], int Om1_Recv_Iteration_End[], int Om1_Recv_Iteration_End_DValid[],
                  int Om2_Recv_Push_Flag[], int Om2_Recv_Update_V_ID[], int Om2_Recv_Update_V_Value[], int Om2_Recv_Update_V_Pull_First_Flag[], int Om2_Recv_Update_V_DValid[], int Om2_Recv_Iteration_End[], int Om2_Recv_Iteration_End_DValid[],
                  int Vertex_BRAM_Data[], int Vertex_BRAM_DValid[],
                  int Om_Global_Iteration_ID[],

                  int *Source_Core_Full,
                  int *Rd_Vertex_BRAM_Addr, int *Rd_Vertex_BRAM_Valid,
                  int *Wr_Vertex_Bram_Push_Flag, int *Wr_Vertex_BRAM_Addr, int *Wr_Vertex_BRAM_Data, int *Wr_Vertex_BRAM_Pull_First_Flag, int *Wr_Vertex_BRAM_Valid, int *Wr_Vertex_BRAM_Iteration_End, int *Wr_Vertex_BRAM_Iteration_End_DValid,
                  int *Om2_Send_Push_Flag, int *Om2_Send_Update_V_ID, int *Om2_Send_Update_V_Value, int *Om2_Send_Update_V_Pull_First_Flag, int *Om2_Send_Update_V_DValid, int *Om2_Send_Iteration_End, int *Om2_Send_Iteration_End_DValid) {

    for (int i = 0; i < CORE_NUM; ++ i) {
        Backend_Source_Core_Single(i,
                                   Om1_Recv_Push_Flag[i], Om1_Recv_Update_V_ID[i], Om1_Recv_Update_V_Value[i], Om1_Recv_Update_V_Pull_First_Flag[i], Om1_Recv_Update_V_DValid[i], Om1_Recv_Iteration_End[i], Om1_Recv_Iteration_End_DValid[i],
                                   Vertex_BRAM_Data[i], Vertex_BRAM_DValid[i],
                                   Om_Global_Iteration_ID[i],

                                   &Source_Core_Full[i],
                                   &Rd_Vertex_BRAM_Addr[i], &Rd_Vertex_BRAM_Valid[i],
                                   &Om2_Send_Push_Flag[i], &Om2_Send_Update_V_ID[i], &Om2_Send_Update_V_Value[i], &Om2_Send_Update_V_Pull_First_Flag[i], &Om2_Send_Update_V_DValid[i], &Om2_Send_Iteration_End[i], &Om2_Send_Iteration_End_DValid[i]);

        Backend_Destination_Core_Single(i,
                                        Om2_Recv_Push_Flag[i], Om2_Recv_Update_V_ID[i], Om2_Recv_Update_V_Value[i], Om2_Recv_Update_V_Pull_First_Flag[i], Om2_Recv_Update_V_DValid[i], Om2_Recv_Iteration_End[i], Om2_Recv_Iteration_End_DValid[i],

                                        &Wr_Vertex_Bram_Push_Flag[i], &Wr_Vertex_BRAM_Addr[i], &Wr_Vertex_BRAM_Data[i], &Wr_Vertex_BRAM_Pull_First_Flag[i], &Wr_Vertex_BRAM_Valid[i], &Wr_Vertex_BRAM_Iteration_End[i], &Wr_Vertex_BRAM_Iteration_End_DValid[i]);


    }
}


void Backend_Source_Core_Single(int Core_ID,
                                int Om1_Recv_Push_Flag, int Om1_Recv_Update_V_ID, int Om1_Recv_Update_V_Value, int Om1_Recv_Update_V_Pull_First_Flag, int Om1_Recv_Update_V_DValid, int Om1_Recv_Iteration_End, int Om1_Recv_Iteration_End_DValid,
                                int Vertex_BRAM_Data, int Vertex_BRAM_DValid,
                                int Om_Global_Iteration_ID,

                                int *Source_Core_Full,
                                int *Rd_Vertex_BRAM_Addr, int *Rd_Vertex_BRAM_Valid,
                                int *Om2_Send_Push_Flag, int *Om2_Send_Update_V_ID, int *Om2_Send_Update_V_Value, int *Om2_Send_Update_V_Pull_First_Flag, int *Om2_Send_Update_V_DValid, int *Om2_Send_Iteration_End, int *Om2_Send_Iteration_End_DValid) {

    static queue<int> update_v_id_buffer[CORE_NUM], pull_first_flag_buffer[CORE_NUM];
    static int buffer_empty[CORE_NUM], buffer_full[CORE_NUM];

    #if DEBUG
        debug_M9._update_v_id_buffer = update_v_id_buffer;
        debug_M9._pull_first_flag_buffer = pull_first_flag_buffer;
    #endif

    if (rst_rd) {
        *Rd_Vertex_BRAM_Addr = 0;
        *Rd_Vertex_BRAM_Valid = 0;
        while (!update_v_id_buffer[Core_ID].empty())      update_v_id_buffer[Core_ID].pop();
        while (!pull_first_flag_buffer[Core_ID].empty())  pull_first_flag_buffer[Core_ID].pop();
    }
    else {
        if (Om1_Recv_Update_V_DValid && Om1_Recv_Push_Flag == 0) {
            // special case for powerlaw vertex
            if (Om1_Recv_Update_V_ID != Om1_Recv_Update_V_Value) {
                *Rd_Vertex_BRAM_Addr = Om1_Recv_Update_V_Value;
            } else {
                *Rd_Vertex_BRAM_Addr = -2;
            }
            *Rd_Vertex_BRAM_Valid = 1;

            update_v_id_buffer[Core_ID].push(Om1_Recv_Update_V_ID);
            pull_first_flag_buffer[Core_ID].push(Om1_Recv_Update_V_Pull_First_Flag);
        }
        else {
            *Rd_Vertex_BRAM_Addr = 0;
            *Rd_Vertex_BRAM_Valid = 0;
        }
    }

    if (rst_rd) {
        *Om2_Send_Push_Flag = 0;
        *Om2_Send_Update_V_ID = 0;
        *Om2_Send_Update_V_Value = 0;
        *Om2_Send_Update_V_Pull_First_Flag = 0;
        *Om2_Send_Update_V_DValid = 0;
    }
    else {
        if (Vertex_BRAM_DValid) {
            if (buffer_empty[Core_ID]) cout << "Cycle " << clk << ": error 1 in backend_source_core_single" << endl;
            if (Om1_Recv_Update_V_DValid && Om1_Recv_Push_Flag) cout << "Cycle " << clk << ": error 2 in backend_source_core_single" << endl;

            *Om2_Send_Push_Flag = 0;
            *Om2_Send_Update_V_ID = update_v_id_buffer[Core_ID].front();
            *Om2_Send_Update_V_Value = (Vertex_BRAM_Data == -2 ? Om_Global_Iteration_ID : Vertex_BRAM_Data);
            *Om2_Send_Update_V_Pull_First_Flag = pull_first_flag_buffer[Core_ID].front();
            *Om2_Send_Update_V_DValid = 1;

            #if (DEBUG && PRINT_CONS)
                if (*Om2_Send_Update_V_ID == PRINT_ID && *Om2_Send_Update_V_DValid == 1) {
                    printf("clk=%d, om2_send_push_flag=%d, om2_send_update_v_id=%d, om2_send_update_v_value=%d, om2_send_update_v_pull_first_flag=%d, om2_send_update_v_dvalid=%d\n",
                        clk, *Om2_Send_Push_Flag, *Om2_Send_Update_V_ID, *Om2_Send_Update_V_Value, *Om2_Send_Update_V_Pull_First_Flag, *Om2_Send_Update_V_DValid);
                }
            #endif

            update_v_id_buffer[Core_ID].pop();
            pull_first_flag_buffer[Core_ID].pop();
        }
        else {
            if (Om1_Recv_Update_V_DValid && Om1_Recv_Push_Flag) {
                *Om2_Send_Push_Flag = 1;
                *Om2_Send_Update_V_ID = Om1_Recv_Update_V_ID;
                *Om2_Send_Update_V_Value = Om1_Recv_Update_V_Value;
                *Om2_Send_Update_V_Pull_First_Flag = 0;
                *Om2_Send_Update_V_DValid = 1;
            }
            else {
                *Om2_Send_Push_Flag = 0;
                *Om2_Send_Update_V_ID = 0;
                *Om2_Send_Update_V_Value = 0;
                *Om2_Send_Update_V_Pull_First_Flag = 0;
                *Om2_Send_Update_V_DValid = 0;
            }
        }
    }

    if (!rst_rd && Om1_Recv_Iteration_End && Om1_Recv_Iteration_End_DValid && buffer_empty[Core_ID]) {
        *Om2_Send_Iteration_End = 1;
        *Om2_Send_Iteration_End_DValid = 1;
    }
    else {
        *Om2_Send_Iteration_End = 0;
        *Om2_Send_Iteration_End_DValid = 0;
    }

    buffer_empty[Core_ID] = (update_v_id_buffer[Core_ID].size() == 0);
    buffer_full[Core_ID] = (update_v_id_buffer[Core_ID].size() >= FIFO_SIZE);

    *Source_Core_Full = buffer_full[Core_ID];
}

void Backend_Destination_Core_Single(int Core_ID,
                                     int Om2_Recv_Push_Flag, int Om2_Recv_Update_V_ID, int Om2_Recv_Update_V_Value, int Om2_Recv_Update_V_Pull_First_Flag, int Om2_Recv_Update_V_DValid, int Om2_Recv_Iteration_End, int Om2_Recv_Iteration_End_DValid,

                                     int *Wr_Vertex_Bram_Push_Flag, int *Wr_Vertex_BRAM_Addr, int *Wr_Vertex_BRAM_Data, int *Wr_Vertex_BRAM_Pull_First_Flag, int *Wr_Vertex_BRAM_Valid,
                                     int *Wr_Vertex_BRAM_Iteration_End, int *Wr_Vertex_BRAM_Iteration_End_DValid) {
    if (rst_rd) {
        *Wr_Vertex_Bram_Push_Flag = 0;
        *Wr_Vertex_BRAM_Addr = 0;
        *Wr_Vertex_BRAM_Data = 0;
        *Wr_Vertex_BRAM_Pull_First_Flag = 0;
        *Wr_Vertex_BRAM_Valid = 0;
    }
    else {
        if (Om2_Recv_Update_V_DValid) {
            *Wr_Vertex_Bram_Push_Flag = Om2_Recv_Push_Flag;
            *Wr_Vertex_BRAM_Addr = Om2_Recv_Update_V_ID;
            *Wr_Vertex_BRAM_Data = Om2_Recv_Update_V_Value;
            *Wr_Vertex_BRAM_Pull_First_Flag = Om2_Recv_Update_V_Pull_First_Flag;
            *Wr_Vertex_BRAM_Valid = 1;

            #if (DEBUG && PRINT_CONS)
                if (*Wr_Vertex_BRAM_Addr == PRINT_ID && *Wr_Vertex_BRAM_Valid == 1) {
                    printf("clk=%d, wr_vertex_bram_push_flag=%d, wr_vertex_bram_addr=%d, wr_vertex_bram_data=%d, wr_vertex_bram_pull_first_flag=%d, wr_vertex_bram_valid=%d\n",
                        clk, *Wr_Vertex_Bram_Push_Flag, *Wr_Vertex_BRAM_Addr, *Wr_Vertex_BRAM_Data, *Wr_Vertex_BRAM_Pull_First_Flag, *Wr_Vertex_BRAM_Valid);
                }
            #endif
        }
        else{
            *Wr_Vertex_Bram_Push_Flag = 0;
            *Wr_Vertex_BRAM_Addr = 0;
            *Wr_Vertex_BRAM_Data = 0;
            *Wr_Vertex_BRAM_Pull_First_Flag = 0;
            *Wr_Vertex_BRAM_Valid = 0;
        }
    }

    if (!rst_rd && Om2_Recv_Iteration_End && Om2_Recv_Iteration_End_DValid && !Om2_Recv_Update_V_DValid) {
        *Wr_Vertex_BRAM_Iteration_End = 1;
        *Wr_Vertex_BRAM_Iteration_End_DValid = 1;
    }
    else {
        *Wr_Vertex_BRAM_Iteration_End = 0;
        *Wr_Vertex_BRAM_Iteration_End_DValid = 0;
    }
}