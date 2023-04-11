// #include "Network.h"
#include "Accelerator.h"
#include <iostream>
int count_;
extern int clk;

extern int vtx_bram_data[CORE_NUM], vtx_bram_data_valid[CORE_NUM],vtx_bram_Read_Addr[CORE_NUM], vtx_bram_Read_Addr_Valid[CORE_NUM],
           vtx_bram_Wr_Addr[CORE_NUM], vtx_bram_Wr_Data[CORE_NUM], vtx_bram_Wr_Addr_Valid[CORE_NUM];

extern int edge_bram_data[CORE_NUM], edge_bram_data_valid[CORE_NUM];

int P1_Push_Flag_rd[CORE_NUM], P1_Active_V_ID_rd[CORE_NUM], P1_Active_V_Value_rd[CORE_NUM], P1_Active_V_Pull_First_Flag_rd[CORE_NUM], P1_Active_V_DValid_rd[CORE_NUM],
    P1_Iteration_End_rd[CORE_NUM], P1_Iteration_End_Dvalid_rd[CORE_NUM];

int Vertex_BRAM_Data_rd[CORE_NUM], Vertex_BRAM_DValid_rd[CORE_NUM];
int Vertex_BRAM_Data_wr[CORE_NUM], Vertex_BRAM_DValid_wr[CORE_NUM];

int Backend_Active_V_ID_rd[CORE_NUM], Backend_Active_V_Updated_rd[CORE_NUM], Backend_Active_V_Pull_First_Flag_rd[CORE_NUM],Backend_Active_V_DValid_rd[CORE_NUM], Backend_Iteration_End_rd[CORE_NUM], Backend_Iteration_End_DValid_rd[CORE_NUM];
int Backend_Active_V_ID_wr[CORE_NUM], Backend_Active_V_Updated_wr[CORE_NUM], Backend_Active_V_Pull_First_Flag_wr[CORE_NUM], Backend_Active_V_DValid_wr[CORE_NUM], Backend_Iteration_End_wr[CORE_NUM], Backend_Iteration_End_DValid_wr[CORE_NUM];

int AIE_Active_V_Pull_First_Flag_rd[CORE_NUM], AIE_Iteration_End_rd[CORE_NUM], AIE_Iteration_End_DValid_rd[CORE_NUM];
int AIE_Active_V_Pull_First_Flag_wr[CORE_NUM], AIE_Iteration_End_wr[CORE_NUM], AIE_Iteration_End_DValid_wr[CORE_NUM];

int P7_Source_Core_Full_wr[CORE_NUM],
    P7_Rd_Vertex_BRAM_Addr_wr[CORE_NUM], P7_Rd_Vertex_BRAM_Valid_wr[CORE_NUM],
    P7_Wr_Vertex_Bram_Push_Flag_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Addr_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Data_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Pull_First_Flag_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Valid_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Iteration_End_wr[CORE_NUM], P7_Wr_Vertex_BRAM_Iteration_End_DValid_wr[CORE_NUM];
 
 int P7_Source_Core_Full_rd[CORE_NUM],
    P7_Rd_Vertex_BRAM_Addr_rd[CORE_NUM], P7_Rd_Vertex_BRAM_Valid_rd[CORE_NUM],
    P7_Wr_Vertex_Bram_Push_Flag_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Addr_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Data_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Pull_First_Flag_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Valid_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Iteration_End_rd[CORE_NUM], P7_Wr_Vertex_BRAM_Iteration_End_DValid_rd[CORE_NUM];

int P6_Stage_Full1_wr[PSEUDO_CHANNEL_NUM], P6_Stage_Full2_wr[CORE_NUM];

int P6_Stage_Full1_rd[PSEUDO_CHANNEL_NUM], P6_Stage_Full2_rd[CORE_NUM];

int P5_2_Stage_Full_rd[CORE_NUM],
    P5_2_Push_Flag_rd[CORE_NUM], P5_2_Active_V_id_rd[CORE_NUM], P5_2_Active_V_Value_rd[CORE_NUM], P5_2_Acitve_V_Edge_rd[CORE_NUM], P5_2_Active_V_DValid_rd[CORE_NUM],
    P5_2_Iteration_End_rd[CORE_NUM], P5_2_Iteration_End_DValid_rd[CORE_NUM];
int P5_2_Stage_Full_wr[CORE_NUM],
    P5_2_Push_Flag_wr[CORE_NUM], P5_2_Active_V_id_wr[CORE_NUM], P5_2_Active_V_Value_wr[CORE_NUM], P5_2_Acitve_V_Edge_wr[CORE_NUM], P5_2_Active_V_DValid_wr[CORE_NUM],
    P5_2_Iteration_End_wr[CORE_NUM], P5_2_Iteration_End_DValid_wr[CORE_NUM];

int P5_Stage_Full_wr[CORE_NUM],
    P5_Push_Flag_wr[CORE_NUM], P5_Active_V_ID_wr[CORE_NUM], P5_Active_V_Value_wr[CORE_NUM], P5_Active_V_DValid_wr[CORE_NUM],
    P5_Iteration_End_wr[CORE_NUM], P5_Iteration_End_DValid_wr[CORE_NUM];

int P5_Stage_Full_rd[CORE_NUM],
    P5_Push_Flag_rd[CORE_NUM], P5_Active_V_ID_rd[CORE_NUM], P5_Active_V_Value_rd[CORE_NUM], P5_Active_V_DValid_rd[CORE_NUM],
    P5_Iteration_End_rd[CORE_NUM], P5_Iteration_End_DValid_rd[CORE_NUM];

int P4_Stage_Full_wr[CORE_NUM],
    P4_Rd_HBM_Edge_Addr_wr[CORE_NUM], P4_HBM_Push_Flag_wr[CORE_NUM], P4_HBM_Active_V_ID_wr[CORE_NUM], P4_HBM_Active_V_Value_wr[CORE_NUM], P4_Rd_HBM_Edge_Valid_wr[CORE_NUM];
int P4_Rd_BRAM_Edge_Addr_wr[CORE_NUM], P4_Rd_BRAM_Edge_Valid_wr[CORE_NUM];
int P4_Rd_BRAM_Edge_Addr_rd[CORE_NUM], P4_Rd_BRAM_Edge_Valid_rd[CORE_NUM];
int P4_BRAM_Active_V_ID_wr[CORE_NUM], P4_BRAM_Active_V_Value_wr[CORE_NUM], P4_BRAM_Push_Flag_wr[CORE_NUM],
    P4_Iteration_End_wr[CORE_NUM], P4_Iteration_End_DValid_wr[CORE_NUM];
BitVector_16 P4_Rd_HBM_Edge_Mask_rd[CORE_NUM];
BitVector_16 P4_Rd_HBM_Edge_Mask_wr[CORE_NUM];

int P4_BRAM_Active_V_ID_rd[CORE_NUM], P4_BRAM_Active_V_Value_rd[CORE_NUM], P4_BRAM_Push_Flag_rd[CORE_NUM],
    P4_Iteration_End_rd[CORE_NUM], P4_Iteration_End_DValid_rd[CORE_NUM];

int P4_Stage_Full_rd[CORE_NUM],
    P4_Rd_HBM_Edge_Addr_rd[CORE_NUM], P4_HBM_Push_Flag_rd[CORE_NUM], P4_HBM_Active_V_ID_rd[CORE_NUM], P4_HBM_Active_V_Value_rd[CORE_NUM], P4_Rd_HBM_Edge_Valid_rd[CORE_NUM];

int P2_Stage_Full_wr[CORE_NUM],
    P2_Push_Flag_wr[CORE_NUM], P2_Active_V_ID_wr[CORE_NUM], P2_Active_V_Value_wr[CORE_NUM], P2_Active_V_Pull_First_Flag_wr[CORE_NUM], 
    P2_Iteration_End_wr[CORE_NUM], P2_Iteration_End_DValid_wr[CORE_NUM];

int P2_Stage_Full_rd[CORE_NUM],
    P2_Push_Flag_rd[CORE_NUM], P2_Active_V_ID_rd[CORE_NUM], P2_Active_V_Value_rd[CORE_NUM],  P2_Active_V_Pull_First_Flag_rd[CORE_NUM], 
    P2_Iteration_End_rd[CORE_NUM], P2_Iteration_End_DValid_rd[CORE_NUM];

int P1_Push_Flag_wr[CORE_NUM], P1_Active_V_ID_wr[CORE_NUM], P1_Active_V_Value_wr[CORE_NUM], P1_Active_V_Pull_First_Flag_wr[CORE_NUM], P1_Active_V_DValid_wr[CORE_NUM],
    P1_Iteration_End_wr[CORE_NUM], P1_Iteration_End_Dvalid_wr[CORE_NUM];





void Compute_Core(pipe_manager* pipes){
    static Info_P1 self_info_p1;
    static Info_P5 self_info_p5;
    static Info_P6 self_info_p6;
    static Info_P7 self_info_p7;
    static Info_P_Om1 self_info_om1;
    static Info_P_Om2 self_info_om2;
    static Info_P_AIE_Global self_info_p_aie_global,self_info_p_aie_global_receive;
    static Info_P_HBM self_info_hbm;
    static Info_P_Global self_p_global;

    static Info_P_URAM2Edge self_info_uram2edge;
    static Info_P_Offset2URAM self_info_offset2uram;
    static Info_P_Offset2URAM self_info_offset2uram_other;
    
    
    pipes->write(&self_info_om1);
    pipes->write(&self_info_om2);
    pipes->write(&self_info_p_aie_global_receive);
    pipes->write(&self_info_hbm);
    pipes->write(&self_p_global);

    pipes->write(&self_info_uram2edge);
    pipes->write(&self_info_offset2uram_other);
    RD_ACTIVE_VERTEX(self_info_p_aie_global_receive.AIE_Active_V_ID, self_info_p_aie_global_receive.AIE_Active_V_Updated, AIE_Active_V_Pull_First_Flag_rd, self_info_p_aie_global_receive.AIE_Active_V_DValid,
                    AIE_Iteration_End_rd, AIE_Iteration_End_DValid_rd,
                    P2_Stage_Full_rd,

                    P1_Push_Flag_wr, P1_Active_V_ID_wr, P1_Active_V_Value_wr, P1_Active_V_Pull_First_Flag_wr, P1_Active_V_DValid_wr,
                    P1_Iteration_End_wr, P1_Iteration_End_Dvalid_wr,
                    self_info_p1.P1_Global_Iteration_ID);


    Read_Active_Vertex_Offset(P1_Push_Flag_rd, P1_Active_V_ID_rd, P1_Active_V_Value_rd, P1_Active_V_Pull_First_Flag_rd, P1_Active_V_DValid_rd,
                                P1_Iteration_End_rd, P1_Iteration_End_Dvalid_rd,
                                P4_Stage_Full_rd,

                                P2_Stage_Full_wr,
                                P2_Push_Flag_wr, P2_Active_V_ID_wr, P2_Active_V_Value_wr, self_info_offset2uram.P2_Active_V_Offset_Addr, P2_Active_V_Pull_First_Flag_wr, self_info_offset2uram.P2_Active_V_DValid,
                                P2_Iteration_End_wr, P2_Iteration_End_DValid_wr);

    
    RD_Active_Vertex_Edge(P2_Push_Flag_rd, P2_Active_V_ID_rd, P2_Active_V_Value_rd, P2_Active_V_Pull_First_Flag_rd, self_info_offset2uram_other.P2_Active_V_DValid,
                          P2_Iteration_End_rd, P2_Iteration_End_DValid_rd,
                          self_info_uram2edge.Offset_Uram_Active_V_LOffset, self_info_uram2edge.Offset_Uram_Active_V_ROffset, self_info_uram2edge.Offset_Uram_Active_V_DValid,
                          P5_Stage_Full_rd, P5_2_Stage_Full_rd,

                          P4_Stage_Full_wr,
                          P4_Rd_HBM_Edge_Addr_wr, P4_Rd_HBM_Edge_Mask_wr, P4_HBM_Push_Flag_wr,  P4_HBM_Active_V_ID_wr, P4_HBM_Active_V_Value_wr, P4_Rd_HBM_Edge_Valid_wr,
                          P4_Rd_BRAM_Edge_Addr_wr, P4_BRAM_Push_Flag_wr, P4_BRAM_Active_V_ID_wr, P4_BRAM_Active_V_Value_wr, P4_Rd_BRAM_Edge_Valid_wr,
                          P4_Iteration_End_wr, P4_Iteration_End_DValid_wr);

    

    Generate_HBM_Edge_Rqst(P4_Rd_HBM_Edge_Addr_rd, P4_Rd_HBM_Edge_Mask_rd, P4_Rd_HBM_Edge_Valid_rd,
                            P4_HBM_Push_Flag_rd, P4_HBM_Active_V_ID_rd, P4_HBM_Active_V_Value_rd,
                            P4_Iteration_End_rd, P4_Iteration_End_DValid_rd,
                            self_info_hbm.HBM_Interface_Full, P6_Stage_Full1_rd,

                            P5_Stage_Full_wr,
                            self_info_p5.P5_Rd_HBM_Edge_Addr, self_info_p5.P5_Rd_HBM_Edge_Valid,
                            P5_Push_Flag_wr, P5_Active_V_ID_wr, P5_Active_V_Value_wr, P5_Active_V_DValid_wr,
                            P5_Iteration_End_wr, P5_Iteration_End_DValid_wr);

    
    Rd_First_Edge_Bram(P4_Rd_BRAM_Edge_Addr_rd, P4_BRAM_Push_Flag_rd, P4_BRAM_Active_V_ID_rd, P4_BRAM_Active_V_Value_rd, P4_Rd_BRAM_Edge_Valid_rd,
                        P4_Iteration_End_rd, P4_Iteration_End_DValid_rd,
                        P6_Stage_Full2_rd,
                        edge_bram_data, edge_bram_data_valid,

                        P5_2_Stage_Full_wr,
                        P5_2_Push_Flag_wr, P5_2_Active_V_id_wr, P5_2_Active_V_Value_wr, P5_2_Acitve_V_Edge_wr, P5_2_Active_V_DValid_wr,
                        P5_2_Iteration_End_wr, P5_2_Iteration_End_DValid_wr);

    ///asynchronous FIFO
    Schedule(P5_Push_Flag_rd, P5_Active_V_ID_rd, P5_Active_V_Value_rd, P5_Active_V_DValid_rd, P5_Iteration_End_rd, P5_Iteration_End_DValid_rd,
             self_info_hbm.HBM_Interface_Active_V_Edge, self_info_hbm.HBM_Interface_Active_V_Edge_Valid,
             P5_2_Push_Flag_rd, P5_2_Active_V_id_rd, P5_2_Active_V_Value_rd, P5_2_Acitve_V_Edge_rd, P5_2_Active_V_DValid_rd, P5_2_Iteration_End_rd, P5_2_Iteration_End_DValid_rd,
             self_p_global.P5_Global_Powerlaw_V_ID, self_p_global.P5_Global_Powerlaw_V_Visit,
             self_info_om1.Om1_Stage_Full,

             P6_Stage_Full1_wr, P6_Stage_Full2_wr,
             self_info_p6.P6_Push_Flag, self_info_p6.P6_Update_V_ID, self_info_p6.P6_Update_V_Value, self_info_p6.P6_Pull_First_Flag, self_info_p6.P6_Update_V_DValid,
             self_info_p6.P6_Iteration_End, self_info_p6.P6_Iteration_End_DValid);



    Backend_Core(self_info_om1.Om1_Push_Flag, self_info_om1.Om1_Update_V_ID, self_info_om1.Om1_Update_V_Value, self_info_om1.Om1_Update_V_Pull_First_Flag, self_info_om1.Om1_Update_V_DValid, self_info_om1.Om1_Iteration_End, self_info_om1.Om1_Iteration_End_DValid,
                self_info_om2.Om2_Push_Flag, self_info_om2.Om2_Update_V_ID, self_info_om2.Om2_Update_V_Value, self_info_om2.Om2_Update_V_Pull_First_Flag, self_info_om2.Om2_Update_V_DValid, self_info_om2.Om2_Iteration_End, self_info_om2.Om2_Iteration_End_DValid,
                Vertex_BRAM_Data_rd, Vertex_BRAM_DValid_rd,
                self_p_global.Om_Global_Iteration_ID,

                self_info_p7.P7_Source_Core_Full,
                P7_Rd_Vertex_BRAM_Addr_wr, P7_Rd_Vertex_BRAM_Valid_wr,
                P7_Wr_Vertex_Bram_Push_Flag_wr, P7_Wr_Vertex_BRAM_Addr_wr, P7_Wr_Vertex_BRAM_Data_wr, P7_Wr_Vertex_BRAM_Pull_First_Flag_wr, P7_Wr_Vertex_BRAM_Valid_wr, P7_Wr_Vertex_BRAM_Iteration_End_wr, P7_Wr_Vertex_BRAM_Iteration_End_DValid_wr,
                self_info_p7.P7_Om2_Send_Push_Flag, self_info_p7.P7_Om2_Send_Update_V_ID, self_info_p7.P7_Om2_Send_Update_V_Value, self_info_p7.P7_Om2_Send_Update_V_Pull_First_Flag, self_info_p7.P7_Om2_Send_Update_V_DValid, self_info_p7.P7_Om2_Iteration_End, self_info_p7.P7_Om2_Iteration_End_DValid);

    
    Vertex_BRAM(P7_Rd_Vertex_BRAM_Addr_rd, P7_Rd_Vertex_BRAM_Valid_rd,
                P7_Wr_Vertex_Bram_Push_Flag_rd, P7_Wr_Vertex_BRAM_Addr_rd, P7_Wr_Vertex_BRAM_Data_rd, P7_Wr_Vertex_BRAM_Pull_First_Flag_rd, P7_Wr_Vertex_BRAM_Valid_rd,
                P7_Wr_Vertex_BRAM_Iteration_End_rd, P7_Wr_Vertex_BRAM_Iteration_End_DValid_rd,
                self_p_global.P7_Global_Iteration_ID,

                vtx_bram_data, vtx_bram_data_valid,
                vtx_bram_Read_Addr, vtx_bram_Read_Addr_Valid,vtx_bram_Wr_Addr, vtx_bram_Wr_Data, vtx_bram_Wr_Addr_Valid,

                Vertex_BRAM_Data_wr, Vertex_BRAM_DValid_wr,
                Backend_Active_V_ID_wr, Backend_Active_V_Updated_wr, Backend_Active_V_Pull_First_Flag_wr, Backend_Active_V_DValid_wr, Backend_Iteration_End_wr, Backend_Iteration_End_DValid_wr);


    for(int i = 0 ; i<CORE_NUM ; i++){
        if(vtx_bram_data_valid[i]){
            count_ ++;
        }
    }
    if(clk % 500 == 0){
        printf("clk:%d count = %d\n",clk,count_);
    }

    Apply_Iteration_End(Backend_Active_V_ID_rd, Backend_Active_V_Updated_rd, Backend_Active_V_Pull_First_Flag_rd, Backend_Active_V_DValid_rd, Backend_Iteration_End_rd, Backend_Iteration_End_DValid_rd,

                        self_info_p_aie_global.AIE_Active_V_ID, self_info_p_aie_global.AIE_Active_V_Updated, AIE_Active_V_Pull_First_Flag_wr, self_info_p_aie_global.AIE_Active_V_DValid, AIE_Iteration_End_wr, AIE_Iteration_End_DValid_wr);

    pipes->read(&self_info_p6);
    pipes->read(&self_info_p5);
    pipes->read(&self_info_p7);
    pipes->read(&self_info_p_aie_global);
    pipes->read(&self_info_p1);
    pipes->read(&self_info_offset2uram);
    for (int i = 0; i < CORE_NUM; ++ i) {
            //edge_bram
            // edge_bram_Wr_Addr_Valid[i] = 0;
            // 7
            P1_Push_Flag_rd[i] = P1_Push_Flag_wr[i];
            P1_Active_V_ID_rd[i] = P1_Active_V_ID_wr[i];
            P1_Active_V_Value_rd[i] = P1_Active_V_Value_wr[i];
            P1_Active_V_Pull_First_Flag_rd[i] = P1_Active_V_Pull_First_Flag_wr[i];
            P1_Active_V_DValid_rd[i] = P1_Active_V_DValid_wr[i];
            P1_Iteration_End_rd[i] = P1_Iteration_End_wr[i];
            P1_Iteration_End_Dvalid_rd[i] = P1_Iteration_End_Dvalid_wr[i];

            // 9
            P2_Stage_Full_rd[i] = P2_Stage_Full_wr[i];
            P2_Push_Flag_rd[i] = P2_Push_Flag_wr[i];
            P2_Active_V_ID_rd[i] = P2_Active_V_ID_wr[i];
            P2_Active_V_Value_rd[i] = P2_Active_V_Value_wr[i];
            P2_Active_V_Pull_First_Flag_rd[i] = P2_Active_V_Pull_First_Flag_wr[i];
            P2_Iteration_End_rd[i] = P2_Iteration_End_wr[i];
            P2_Iteration_End_DValid_rd[i] = P2_Iteration_End_DValid_wr[i];


            // 14
            P4_Stage_Full_rd[i] = P4_Stage_Full_wr[i];
            P4_Rd_HBM_Edge_Addr_rd[i] = P4_Rd_HBM_Edge_Addr_wr[i];
            P4_HBM_Push_Flag_rd[i] = P4_HBM_Push_Flag_wr[i];
            P4_HBM_Active_V_ID_rd[i] = P4_HBM_Active_V_ID_wr[i];
            P4_HBM_Active_V_Value_rd[i] = P4_HBM_Active_V_Value_wr[i];
            P4_Rd_HBM_Edge_Mask_rd[i] = P4_Rd_HBM_Edge_Mask_wr[i];
            P4_Rd_HBM_Edge_Valid_rd[i] = P4_Rd_HBM_Edge_Valid_wr[i];
            P4_Rd_BRAM_Edge_Addr_rd[i] = P4_Rd_BRAM_Edge_Addr_wr[i];
            P4_BRAM_Push_Flag_rd[i] = P4_BRAM_Push_Flag_wr[i];
            P4_BRAM_Active_V_ID_rd[i] = P4_BRAM_Active_V_ID_wr[i];
            P4_BRAM_Active_V_Value_rd[i] = P4_BRAM_Active_V_Value_wr[i];
            P4_Rd_BRAM_Edge_Valid_rd[i] = P4_Rd_BRAM_Edge_Valid_wr[i];
            P4_Iteration_End_rd[i] = P4_Iteration_End_wr[i];
            P4_Iteration_End_DValid_rd[i] = P4_Iteration_End_DValid_wr[i];

            // 9 - 2
            P5_Stage_Full_rd[i] = P5_Stage_Full_wr[i];
            P5_Push_Flag_rd[i] = P5_Push_Flag_wr[i];
            P5_Active_V_ID_rd[i] = P5_Active_V_ID_wr[i];
            P5_Active_V_Value_rd[i] = P5_Active_V_Value_wr[i];
            P5_Active_V_DValid_rd[i] = P5_Active_V_DValid_wr[i];
            P5_Iteration_End_rd[i] = P5_Iteration_End_wr[i];
            P5_Iteration_End_DValid_rd[i] = P5_Iteration_End_DValid_wr[i];

            // 8
            P5_2_Stage_Full_rd[i] = P5_2_Stage_Full_wr[i];
            P5_2_Push_Flag_rd[i] = P5_2_Push_Flag_wr[i];
            P5_2_Active_V_id_rd[i] = P5_2_Active_V_id_wr[i];
            P5_2_Active_V_Value_rd[i] = P5_2_Active_V_Value_wr[i];
            P5_2_Acitve_V_Edge_rd[i] = P5_2_Acitve_V_Edge_wr[i];
            P5_2_Active_V_DValid_rd[i] = P5_2_Active_V_DValid_wr[i];
            P5_2_Iteration_End_rd[i] = P5_2_Iteration_End_wr[i];
            P5_2_Iteration_End_DValid_rd[i] = P5_2_Iteration_End_DValid_wr[i];


            // 9 - 1 - 7(network)
            P6_Stage_Full2_rd[i] = P6_Stage_Full2_wr[i];
            

            // 16

            // 17 - 2 - 5 -1(Network)
            P7_Rd_Vertex_BRAM_Addr_rd[i] = P7_Rd_Vertex_BRAM_Addr_wr[i];
            P7_Rd_Vertex_BRAM_Valid_rd[i] = P7_Rd_Vertex_BRAM_Valid_wr[i];
            P7_Wr_Vertex_Bram_Push_Flag_rd[i] = P7_Wr_Vertex_Bram_Push_Flag_wr[i];
            P7_Wr_Vertex_BRAM_Addr_rd[i] = P7_Wr_Vertex_BRAM_Addr_wr[i];
            P7_Wr_Vertex_BRAM_Data_rd[i] = P7_Wr_Vertex_BRAM_Data_wr[i];
            P7_Wr_Vertex_BRAM_Pull_First_Flag_rd[i] = P7_Wr_Vertex_BRAM_Pull_First_Flag_wr[i];
            P7_Wr_Vertex_BRAM_Valid_rd[i] = P7_Wr_Vertex_BRAM_Valid_wr[i];
            P7_Wr_Vertex_BRAM_Iteration_End_rd[i] = P7_Wr_Vertex_BRAM_Iteration_End_wr[i];
            P7_Wr_Vertex_BRAM_Iteration_End_DValid_rd[i] = P7_Wr_Vertex_BRAM_Iteration_End_DValid_wr[i];

            // 2
            Vertex_BRAM_Data_rd[i] = Vertex_BRAM_Data_wr[i];
            Vertex_BRAM_DValid_rd[i] = Vertex_BRAM_DValid_wr[i];

            // 6
            Backend_Active_V_ID_rd[i] = Backend_Active_V_ID_wr[i];
            Backend_Active_V_Updated_rd[i] = Backend_Active_V_Updated_wr[i];
            Backend_Active_V_Pull_First_Flag_rd[i] = Backend_Active_V_Pull_First_Flag_wr[i];
            Backend_Active_V_DValid_rd[i] = Backend_Active_V_DValid_wr[i];
            Backend_Iteration_End_rd[i] = Backend_Iteration_End_wr[i];
            Backend_Iteration_End_DValid_rd[i] = Backend_Iteration_End_DValid_wr[i];

            // 6
            AIE_Active_V_Pull_First_Flag_rd[i] = AIE_Active_V_Pull_First_Flag_wr[i];
            AIE_Iteration_End_rd[i] = AIE_Iteration_End_wr[i];
            AIE_Iteration_End_DValid_rd[i] = AIE_Iteration_End_DValid_wr[i];
    }

    for (int i = 0; i < PSEUDO_CHANNEL_NUM; ++ i) {

        P6_Stage_Full1_rd[i] = P6_Stage_Full1_wr[i];
    }

}