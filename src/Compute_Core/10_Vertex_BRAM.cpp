#include "Accelerator.h"
// #include "MEM.h"
#if DEBUG
    #include "debug.h"
    debug_Vertex_BRAM debug_M10;
#endif

extern int clk;
extern int rst_rd;
#if DEBUG
    int last_update_cycle = 0;
#endif

using namespace std;

void Vertex_BRAM_Single(int Core_ID,
                        int Rd_Addr_Src, int Rd_Valid_Src,
                        int Wr_Push_Flag_Dst, int Wr_Addr_Dst, int Wr_V_Value_Dst, int Wr_Pull_First_Flag_Dst, int Wr_Valid_Dst,
                        int Front_Iteration_End, int Front_Iteration_End_DValid,
                        int Front_Iteration_ID,
                        int Tmp_Bram_Data, int Tmp_Bram_Data_Valid,

                        int *Tmp_Rd_Addr, int *Tmp_Rd_Addr_Valid,
                        int *Tmp_Wr_Addr, int *Tmp_Wr_Value, int *Tmp_Wr_Valid,
                        int *Src_Recv_Update_V_Value, int *Src_Recv_Update_V_DValid,
                        int *Backend_Active_V_ID, int *Backend_Active_V_Updated, int *Backend_Active_V_Pull_First_Flag, int *Backend_Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid);

void BRAM_IP(int Core_ID,
             int Tmp_rd_addr, int Tmp_rd_addr_valid,
             int Tmp_wr_addr, int tmp_wr_data, int Tmp_wr_addr_valid,

             int *Tmp_bram_data, int *Tmp_bram_data_valid);

void Vertex_BRAM(int Rd_Addr_Src[], int Rd_Valid_Src[],
                 int Wr_Push_Flag_Dst[], int Wr_Addr_Dst[], int Wr_V_Value_Dst[], int Wr_Pull_First_Flag_Dst[], int Wr_Valid_Dst[],
                 int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                 int Front_Iteration_ID[],

                 
                 int vtx_bram_data[], int vtx_bram_data_valid[],
                 int *vtx_bram_Read_Addr, int *vtx_bram_Read_Addr_Valid,int *vtx_bram_Wr_Addr, int *vtx_bram_Wr_Data, int *vtx_bram_Wr_Addr_Valid,

                 int *Src_Recv_Update_V_Value, int *Src_Recv_Update_V_DValid,
                 int *Backend_Active_V_ID, int *Backend_Active_V_Updated, int *Backend_Active_V_Pull_First_Flag, int *Backend_Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid) {


    for (int i = 0; i < CORE_NUM; ++ i) {
        Vertex_BRAM_Single(i,
                           Rd_Addr_Src[i], Rd_Valid_Src[i],
                           Wr_Push_Flag_Dst[i], Wr_Addr_Dst[i], Wr_V_Value_Dst[i], Wr_Pull_First_Flag_Dst[i], Wr_Valid_Dst[i],
                           Front_Iteration_End[i], Front_Iteration_End_DValid[i],
                           Front_Iteration_ID[i],
                           vtx_bram_data[i], vtx_bram_data_valid[i],

                           &vtx_bram_Read_Addr[i], &vtx_bram_Read_Addr_Valid[i],
                           &vtx_bram_Wr_Addr[i], &vtx_bram_Wr_Data[i], &vtx_bram_Wr_Addr_Valid[i],
                           &Src_Recv_Update_V_Value[i], &Src_Recv_Update_V_DValid[i],
                           &Backend_Active_V_ID[i], &Backend_Active_V_Updated[i], &Backend_Active_V_Pull_First_Flag[i], &Backend_Active_V_DValid[i], &Iteration_End[i], &Iteration_End_DValid[i]);
        // Vertex_bram.BRAM_IP(rst_rd,i,
        //         tmp_rd_addr[i], tmp_rd_addr_valid[i],
        //         tmp_wr_addr[i], tmp_wr_data[i], tmp_wr_addr_valid[i],

        //         &tmp_bram_data[i], &tmp_bram_data_valid[i]);
    }
}

void Vertex_BRAM_Single(int Core_ID,
                        int Rd_Addr_Src, int Rd_Valid_Src,
                        int Wr_Push_Flag_Dst, int Wr_Addr_Dst, int Wr_V_Value_Dst, int Wr_Pull_First_Flag_Dst, int Wr_Valid_Dst,
                        int Front_Iteration_End, int Front_Iteration_End_DValid,
                        int Front_Iteration_ID,
                        int Tmp_Bram_Data, int Tmp_Bram_Data_Valid,

                        int *Tmp_Rd_Addr, int *Tmp_Rd_Addr_Valid,
                        int *Tmp_Wr_Addr, int *Tmp_Wr_Value, int *Tmp_Wr_Valid,
                        int *Src_Recv_Update_V_Value, int *Src_Recv_Update_V_DValid,
                        int *Backend_Active_V_ID, int *Backend_Active_V_Updated, int *Backend_Active_V_Pull_First_Flag, int *Backend_Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid) {

    //om1 (src) only read
    //om2 (dst) may write or write-after-read
    static queue<int> push_flag_buffer[CORE_NUM], addr_buffer[CORE_NUM], value_buffer[CORE_NUM], pull_first_flag_buffer[CORE_NUM];
    static int buffer_empty[CORE_NUM];

    #if DEBUG
        debug_M10._push_flag_buffer = push_flag_buffer;
        debug_M10._addr_buffer = addr_buffer;
        debug_M10._value_buffer = value_buffer;
        debug_M10._pull_first_flag_buffer = pull_first_flag_buffer;
    #endif

    buffer_empty[Core_ID] = (addr_buffer[Core_ID].size() == 0);

    if (rst_rd) {
        *Tmp_Rd_Addr = 0;
        *Tmp_Rd_Addr_Valid = 0;
        while (!push_flag_buffer[Core_ID].empty())        push_flag_buffer[Core_ID].pop();
        while (!addr_buffer[Core_ID].empty())             addr_buffer[Core_ID].pop();
        while (!value_buffer[Core_ID].empty())            value_buffer[Core_ID].pop();
        while (!pull_first_flag_buffer[Core_ID].empty())  pull_first_flag_buffer[Core_ID].pop();
    }
    else {
        //pull read
        if (Rd_Valid_Src) {
            if (Wr_Addr_Dst && Wr_Push_Flag_Dst) cout << "error 1 in vertex bram" << endl;

            *Tmp_Rd_Addr = Rd_Addr_Src == -2 ? -2 : Rd_Addr_Src / CORE_NUM;
            *Tmp_Rd_Addr_Valid = 1;

            push_flag_buffer[Core_ID].push(0);
            addr_buffer[Core_ID].push(0);
            value_buffer[Core_ID].push(0);
            pull_first_flag_buffer[Core_ID].push(0);
        }
        else {
            //push read
            if (Wr_Valid_Dst && Wr_Push_Flag_Dst) {
                *Tmp_Rd_Addr = Wr_Addr_Dst / CORE_NUM;
                *Tmp_Rd_Addr_Valid = 1;

                push_flag_buffer[Core_ID].push(1);
                addr_buffer[Core_ID].push(Wr_Addr_Dst);
                value_buffer[Core_ID].push(Wr_V_Value_Dst);
                pull_first_flag_buffer[Core_ID].push(Wr_Pull_First_Flag_Dst);
            }
            //pull write
            else {
                *Tmp_Rd_Addr = 0;
                *Tmp_Rd_Addr_Valid = 0;
            }
        }
    }

    if (rst_rd) {
        *Tmp_Wr_Addr = 0;
        *Tmp_Wr_Value = 0;
        *Tmp_Wr_Valid = 0;

        *Src_Recv_Update_V_Value = 0;
        *Src_Recv_Update_V_DValid = 0;

        *Backend_Active_V_ID = 0;
        *Backend_Active_V_Updated = 0;
        *Backend_Active_V_Pull_First_Flag = 0;
        *Backend_Active_V_DValid = 0;
    }
    else {
        //pull write
        if (Wr_Valid_Dst && Wr_Push_Flag_Dst == 0) {
            if (Wr_V_Value_Dst != -1 && Wr_V_Value_Dst <= Front_Iteration_ID) {
                *Tmp_Wr_Addr = Wr_Addr_Dst / CORE_NUM;
                *Tmp_Wr_Value = Wr_V_Value_Dst + 1;
                *Tmp_Wr_Valid = 1;

                *Backend_Active_V_ID = Wr_Addr_Dst;///modify
                *Backend_Active_V_Updated = 1;
                *Backend_Active_V_Pull_First_Flag = Wr_Pull_First_Flag_Dst;
                *Backend_Active_V_DValid = 1;
            }
            else {
                *Tmp_Wr_Addr = 0;
                *Tmp_Wr_Value = 0;
                *Tmp_Wr_Valid = 0;

                *Backend_Active_V_ID = Wr_Addr_Dst;///modify
                *Backend_Active_V_Updated = 0;
                *Backend_Active_V_Pull_First_Flag = Wr_Pull_First_Flag_Dst;
                *Backend_Active_V_DValid = 1;
            }
            #if (DEBUG && PRINT_CONS)
                if (*Backend_Active_V_ID == PRINT_ID) {
                    printf("clk=%d, backend_active_v_id=%d, backend_active_v_updated=%d, backend_active_v_pull_first_flag=%d, backend_active_v_dvalid=%d\n",
                        clk, *Backend_Active_V_ID, *Backend_Active_V_Updated, *Backend_Active_V_Pull_First_Flag, *Backend_Active_V_DValid);
                }
            #endif
        }
        //push write
        else {
            if (Tmp_Bram_Data_Valid && push_flag_buffer[Core_ID].front() == 1) {
                if (Tmp_Bram_Data > value_buffer[Core_ID].front() || Tmp_Bram_Data == -1) {//考虑-1情况 ，且Tmp_Bram_Data > value_buffer[Core_ID].front()即可应当不需要Tmp_Bram_Data > value_buffer[Core_ID].front()+1
                    *Tmp_Wr_Addr = addr_buffer[Core_ID].front() / CORE_NUM;
                    *Tmp_Wr_Value = value_buffer[Core_ID].front();
                    *Tmp_Wr_Valid = 1;
                    #if (DEBUG && PRINT_CONS)
                        if (addr_buffer[Core_ID].front() == PRINT_ID) {
                            printf("clk=%d, tmp_wr_addr=%d, tmp_wr_value=%d, tmp_wr_valid=%d\n", clk, *Tmp_Wr_Addr, *Tmp_Wr_Value, *Tmp_Wr_Valid);
                        }
                    #endif

                    *Backend_Active_V_ID = addr_buffer[Core_ID].front();///modify
                    *Backend_Active_V_Updated = 1;
                    *Backend_Active_V_Pull_First_Flag = pull_first_flag_buffer[Core_ID].front();
                    *Backend_Active_V_DValid = 1;
                }
                else {
                    *Tmp_Wr_Addr = 0;
                    *Tmp_Wr_Value = 0;
                    *Tmp_Wr_Valid = 0;

                    *Backend_Active_V_ID = addr_buffer[Core_ID].front();///modify
                    *Backend_Active_V_Updated = 0;
                    *Backend_Active_V_Pull_First_Flag = pull_first_flag_buffer[Core_ID].front();
                    *Backend_Active_V_DValid = 1;
                }
            }
            else {
                *Tmp_Wr_Addr = 0;
                *Tmp_Wr_Value = 0;
                *Tmp_Wr_Valid = 0;

                *Backend_Active_V_ID = 0;
                *Backend_Active_V_Updated = 0;
                *Backend_Active_V_Pull_First_Flag = 0;
                *Backend_Active_V_DValid = 0;
            }
        }

        if (Tmp_Bram_Data_Valid && push_flag_buffer[Core_ID].front() == 0) {
            *Src_Recv_Update_V_Value = Tmp_Bram_Data;
            *Src_Recv_Update_V_DValid = 1;
        }
        else {
            *Src_Recv_Update_V_Value = 0;
            *Src_Recv_Update_V_DValid = 0;
        }

        if (Tmp_Bram_Data_Valid) {
            push_flag_buffer[Core_ID].pop();
            addr_buffer[Core_ID].pop();
            value_buffer[Core_ID].pop();
            pull_first_flag_buffer[Core_ID].pop();
        }
    }

    if (!rst_rd && Front_Iteration_End && Front_Iteration_End_DValid && buffer_empty[Core_ID]) {
        *Iteration_End = 1;
        *Iteration_End_DValid = 1;
    }
    else {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }
}


void dump_result(int V_Num) {
      FILE *wb_File;
      if ((wb_File = fopen("data/result1.txt", "w")) == NULL) fprintf(stderr, "\033[31mERR:\033[m Can't open file result.txt\n");
      //for (int i = 0; i < V_Num; i++)   fprintf(wb_File, "%d %d\n", i, VERTEX_BRAM[i%CORE_NUM][i/CORE_NUM]);
      fprintf(stderr, "\033[32m[INFO]\033[m Results dumped to the file wb_ans.txt\n");
      fclose(wb_File);
}