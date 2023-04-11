#ifndef ACC_H
#define ACC_H

#define DEBUG 0
//change the macro here
#define PRINT_ID 0
// #include "Macro.h"

#if DEBUG
    int _main(char *off_file, char *edge_file, int root_id, int max_clk);
    #define PRINT_CONS 0
    #define PRINT_ID 0
    #define CONTROL_FLAG 1
#endif

#include "self_config.h"
#include "dramsim.h"
#include <bits/stdc++.h>
#include <queue>
using namespace std;



void Initialize_Input_Graph(char *off_file, char *edge_file);

void Initialize_Offset_Uram(int V_Num);

void Initialize_Vertex_bram(int root_id);

void Initialize_Edge_bram(int V_Num);

void Initialize_VERTEX_DEGREE(int V_Num);

void Initialize_Bitmap(int root_id, int V_Num);

void Compute_Core(pipe_manager* pipes);
void Mem(pipe_manager*);
void Noc(pipe_manager*);
void Global(pipe_manager*);

// void compute_connect(Pipe* pipes);
// void mem_connect(Pipe* pipes);
// void noc_connect(Pipe* pipes);
// void global_connect(Pipe *pipes);
// void connect(Pipe *pipes);

void RD_ACTIVE_VERTEX(int Backend_Active_V_ID[], int Backend_Active_V_Updated[], int Backend_Active_V_Pull_First_Flag[], int Backend_Active_V_DValid[],
                      int Backend_Iteration_End[], int Backend_Iteration_End_DValid[],
                      int NextStage_Full[],

                      Info_P12P2& self_info_p12p2,
                      int *Global_Iteration_ID);

void RD_ACTIVE_VERTEX_GLOBAL_SIGNAL(int Backend_Active_V_ID[], int Backend_Active_V_Updated[], int Backend_Active_V_DValid[],

                                    Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void Read_Active_Vertex_Offset(Info_P12P2 self_info_p12p2,
                               int NextStage_Full[],

                               int *Stage_Full,
                               int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *RD_Active_V_Offset_Addr, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                               int *Iteration_End, int *Iteration_End_DValid);

void Read_Active_Vertex_Offset_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                            int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void RD_Offset_Uram(int Front_RD_Active_V_Offset_Addr[], int Front_Active_V_DValid[],

                    int *Active_V_LOffset, int *Active_V_ROffset,int *Active_V_DValid);

void RD_Active_Vertex_Edge(int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Active_V_Pull_First_Flag[], int Front_Active_V_DValid[],
                           int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                           int Front_Active_V_LOffset[], int Front_Active_V_Roffset[], int Front_Offset_DValid[],
                           int CombineStage_Full[], int BramStage_Full[],

                           int *Stage_Full,
                           int *Rd_HBM_Edge_Addr, BitVector_16 *Rd_HBM_Edge_Mask, int *HBM_Push_Flag, int *HBM_Active_V_ID, int *HBM_Active_V_Value, int *Rd_HBM_Edge_Valid,
                           int *Rd_BRAM_Edge_Addr, int *BRAM_Push_Flag, int *BRAM_Active_V_ID, int *BRAM_Active_V_Value, int *Rd_BRAM_Edge_Valid,
                           int *Iteration_End, int *Iteration_End_DValid);

void RD_Active_Vertex_Edge_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                            int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void Generate_HBM_Edge_Rqst(int Front_Rd_HBM_Edge_Addr[], BitVector_16 Front_Rd_HBM_Edge_Mask[], int Front_Rd_HBM_Edge_Valid[],
                            int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[],
                            int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                            int HBM_Full[], int NextStage_Full[],

                            int *Stage_Full,
                            int *Rd_HBM_Edge_Addr, int *Rd_HBM_Edge_Valid,
                            int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *Active_V_DValid,
                            int *Iteration_End, int *Iteration_End_DValid);

void Generate_HBM_Edge_Rqst_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                         int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void Rd_First_Edge_Bram(int Front_Rd_Edge_Addr[], int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Rd_Edge_Valid[],
                        int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                        int NextStage_Full[],
                        int edge_bram_data[], int edge_bram_data_valid[],

                        int *Stage_Full,
                        int *Push_Flag, int *Active_V_ID, int *Active_V_Value, int *Active_V_Edge, int *Active_V_DValid,
                        int *Iteration_End, int *Iteration_End_DValid);

void HBM_Interface(int Front_Rd_HBM_Edge_Addr[], int Front_Rd_HBM_Edge_Valid[],
                   Cacheline_16 HBM_Controller_Edge[], int HBM_Controller_DValid[],
                   int HBM_Controller_Full[],

                   int *Stage_Full,
                   int *Rd_HBM_Edge_Addr, int *Rd_HBM_Edge_Valid,
                   int *Active_V_Edge, int *Active_V_Edge_Valid);

void HBM_Controller_IP(int Rd_HBM_Edge_Addr[], int Rd_HBM_Edge_Valid[],

                       int *HBM_Controller_Full,
                       Cacheline_16 *HBM_Controller_Edge, int *HBM_Controller_DValid);

void Schedule(int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Active_V_DValid[], int Front_Iteration_End[], int Front_Iteration_End_DValid[],
              int HBM_Iterface_Active_V_Edge[], int Active_V_Edge_Valid[],
              int Front2_Push_Flag[], int Front2_Active_V_ID[], int Front2_Active_V_Value[], int Front2_Active_V_Edge[], int Front2_Active_V_DValid[], int Front2_Iteration_End[], int Front2_Iteration_End_DValid[],
              Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],
              int NextStage_Full[],

              int *Stage_Full1, int *Stage_Full2,
              int *Push_Flag, int *Update_V_ID, int *Update_V_VALUE, int *Pull_First_Flag, int *Update_V_DValid,
              int *Iteration_End, int *Iteration_End_DValid);

void Schedule_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                           int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void Network(int Om_Front_Push_Flag[], int Om_Front_Update_V_ID[], int Om_Front_Update_V_Value[], int Om_Front_Pull_First_Flag[], int Om_Front_Update_V_DValid[], int Om_Front_Iteration_End[], int Om_Front_Iteration_End_DValid[],
            int Next_Stage_Full[],
            
            int *Stage_Full_Om,
            int *Om_Push_Flag, int *Om_Update_V_ID, int *Om_Update_V_Value, int *Om_Pull_First_Flag, int *Om_Update_V_DValid, int *Om_Iteration_End, int *Om_Iteration_End_DValid,
            Network_reg& Nreg, 
            int switch_router = 0, int network_num = 0);

void Omega_Network_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                  int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);

void Backend_Core(int Om1_Recv_Push_Flag[], int Om1_Recv_Update_V_ID[], int Om1_Recv_Update_V_Value[], int Om1_Recv_Update_V_Pull_First_Flag[], int Om1_Recv_Update_V_DValid[], int Om1_Recv_Iteration_End[], int Om1_Recv_Iteration_End_DValid[],
                  int Om2_Recv_Push_Flag[], int Om2_Recv_Update_V_ID[], int Om2_Recv_Update_V_Value[], int Om2_Recv_Update_V_Pull_First_Flag[], int Om2_Recv_Update_V_DValid[], int Om2_Recv_Iteration_End[], int Om2_Recv_Iteration_End_DValid[],
                  int Vertex_BRAM_Data[], int Vertex_BRAM_DValid[],
                  int Om_Global_Iteration_ID[],

                  int *Source_Core_Full,
                  int *Rd_Vertex_BRAM_Addr, int *Rd_Vertex_BRAM_Valid,
                  int *Wr_Vertex_Bram_Push_Flag, int *Wr_Vertex_BRAM_Addr, int *Wr_Vertex_BRAM_Data, int *Wr_Vertex_BRAM_Pull_First_Flag, int *Wr_Vertex_BRAM_Valid, int *Wr_Vertex_BRAM_Iteration_End, int *Wr_Vertex_BRAM_Iteration_End_DValid,
                  int *Om2_Send_Push_Flag, int *Om2_Send_Update_V_ID, int *Om2_Send_Update_V_Value, int *Om2_Send_Update_V_Pull_First_Flag, int *Om2_Send_Update_V_DValid, int *Om2_Send_Iteration_End, int *Om2_Send_Iteration_End_DValid);

void Backend_Core_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                               int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);


void Vertex_BRAM(int Rd_Addr_Src[], int Rd_Valid_Src[],
                 int Wr_Push_Flag_Dst[], int Wr_Addr_Dst[], int Wr_V_Value_Dst[], int Wr_Pull_First_Flag_Dst[], int Wr_Valid_Dst[],
                 int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                 int Front_Iteration_ID[],

                 
                 int vtx_bram_data[], int vtx_bram_data_valid[],
                 int *vtx_bram_Read_Addr, int *vtx_bram_Read_Addr_Valid,int *vtx_bram_Wr_Addr, int *vtx_bram_Wr_Data, int *vtx_bram_Wr_Addr_Valid,

                 int *Src_Recv_Update_V_Value, int *Src_Recv_Update_V_DValid,
                 int *Backend_Active_V_ID, int *Backend_Active_V_Updated, int *Backend_Active_V_Pull_First_Flag, int *Backend_Active_V_DValid, int *Iteration_End, int *Iteration_End_DValid);

void Vertex_BRAM_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                               int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit);


void Apply_Iteration_End(int Front_Active_V_ID[], int Front_Active_V_Updated[], int Front_Active_V_Pull_First_Flag[], int Front_Active_V_DValid[], int Front_Iteration_End[], int Front_Iteration_End_DValid[],

                         int *Active_V_ID, int *Active_V_Updated, int *Active_V_Pull_First_Flag, int *Active_V_DValid,int *Iteration_End, int *Iteration_End_DValid);

void dump_result(int V_Num);
#endif // ACC_H