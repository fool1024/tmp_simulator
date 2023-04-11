// #include "MEM.h"
#include "Accelerator.h"
void mem_connect(Pipe* pipes);
void Mem(Pipe* pipes);

extern int rst_rd;


BRAM vtx_bram(MAX_VERTEX_NUM / CORE_NUM + 1);
BRAM edge_bram(MAX_VERTEX_NUM / CORE_NUM + 1);

int vtx_bram_data[CORE_NUM], vtx_bram_data_valid[CORE_NUM];
int vtx_bram_Read_Addr[CORE_NUM], vtx_bram_Read_Addr_Valid[CORE_NUM],
    vtx_bram_Wr_Addr[CORE_NUM], vtx_bram_Wr_Data[CORE_NUM], vtx_bram_Wr_Addr_Valid[CORE_NUM];

int edge_bram_data[CORE_NUM], edge_bram_data_valid[CORE_NUM];
int edge_bram_Read_Addr[CORE_NUM], edge_bram_Read_Addr_Valid[CORE_NUM],
    edge_bram_Wr_Addr[CORE_NUM], edge_bram_Wr_Data[CORE_NUM], edge_bram_Wr_Addr_Valid[CORE_NUM];

int HBM_Interface_Rd_HBM_Edge_Addr_rd[PSEUDO_CHANNEL_NUM], HBM_Interface_Rd_HBM_Edge_Valid_rd[PSEUDO_CHANNEL_NUM];
int HBM_Interface_Rd_HBM_Edge_Addr_wr[PSEUDO_CHANNEL_NUM], HBM_Interface_Rd_HBM_Edge_Valid_wr[PSEUDO_CHANNEL_NUM];

extern int P4_Rd_BRAM_Edge_Addr_rd[CORE_NUM], P4_Rd_BRAM_Edge_Valid_rd[CORE_NUM];

int HBM_Controller_Full_rd[PSEUDO_CHANNEL_NUM], HBM_Controller_DValid_rd[PSEUDO_CHANNEL_NUM];
Cacheline_16 HBM_Controller_Edge_rd[PSEUDO_CHANNEL_NUM];
int HBM_Controller_Full_wr[PSEUDO_CHANNEL_NUM], HBM_Controller_DValid_wr[PSEUDO_CHANNEL_NUM];
Cacheline_16 HBM_Controller_Edge_wr[PSEUDO_CHANNEL_NUM];


void Mem( pipe_manager* pipes ){
    static Info_P5 self_info_p5;
    static Info_P_HBM self_info_hbm;
    static Info_P_URAM2Edge self_info_uram2edge;
    static Info_P_Offset2URAM self_info_offset2uram;

    pipes->write(&self_info_p5);
    pipes->write(&self_info_offset2uram);

    RD_Offset_Uram(self_info_offset2uram.P2_Active_V_Offset_Addr, self_info_offset2uram.P2_Active_V_DValid,

                   self_info_uram2edge.Offset_Uram_Active_V_LOffset, self_info_uram2edge.Offset_Uram_Active_V_ROffset, self_info_uram2edge.Offset_Uram_Active_V_DValid);

    
    vtx_bram.BRAM_IP(rst_rd,
                    vtx_bram_Read_Addr, vtx_bram_Read_Addr_Valid,
                    vtx_bram_Wr_Addr, vtx_bram_Wr_Data, vtx_bram_Wr_Addr_Valid,

                    vtx_bram_data, vtx_bram_data_valid);

    edge_bram.BRAM_IP(rst_rd,
                   P4_Rd_BRAM_Edge_Addr_rd, P4_Rd_BRAM_Edge_Valid_rd,
                   edge_bram_Wr_Addr, edge_bram_Wr_Data, edge_bram_Wr_Addr_Valid,

                   edge_bram_data, edge_bram_data_valid);
    
    ///asynchronous FIFO
    HBM_Interface(self_info_p5.P5_Rd_HBM_Edge_Addr, self_info_p5.P5_Rd_HBM_Edge_Valid,
                  HBM_Controller_Edge_rd, HBM_Controller_DValid_rd,
                  HBM_Controller_Full_rd,

                  self_info_hbm.HBM_Interface_Full,
                  HBM_Interface_Rd_HBM_Edge_Addr_wr, HBM_Interface_Rd_HBM_Edge_Valid_wr,
                  self_info_hbm.HBM_Interface_Active_V_Edge, self_info_hbm.HBM_Interface_Active_V_Edge_Valid);


    HBM_Controller_IP(HBM_Interface_Rd_HBM_Edge_Addr_rd, HBM_Interface_Rd_HBM_Edge_Valid_rd,

                     HBM_Controller_Full_wr,
                     HBM_Controller_Edge_wr, HBM_Controller_DValid_wr);



    pipes->read(&self_info_hbm);
    pipes->read(&self_info_uram2edge);
    //  connnect
    for(int i = 0 ; i<CORE_NUM ; i++){
        edge_bram_Wr_Addr[i] = 0; 
        edge_bram_Wr_Data[i] = 0; 
        edge_bram_Wr_Addr_Valid[i] = 0;
    }
    for (int i = 0; i < PSEUDO_CHANNEL_NUM; ++ i) {
        HBM_Interface_Rd_HBM_Edge_Addr_rd[i] = HBM_Interface_Rd_HBM_Edge_Addr_wr[i];
        HBM_Interface_Rd_HBM_Edge_Valid_rd[i] = HBM_Interface_Rd_HBM_Edge_Valid_wr[i];

        // HBM_Interface_Full_rd[i] = HBM_Interface_Full_wr[i];
        HBM_Controller_Full_rd[i] = HBM_Controller_Full_wr[i];
        HBM_Controller_DValid_rd[i] = HBM_Controller_DValid_wr[i];

        for (int j = 0; j < GROUP_CORE_NUM; ++ j) HBM_Controller_Edge_rd[i].data[j] = HBM_Controller_Edge_wr[i].data[j];

    }

}


