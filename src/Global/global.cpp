#include "Accelerator.h"
// int P1_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P1_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P1_Global_Powerlaw_V_Visit_rd[CORE_NUM];

// int P1_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P1_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P1_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int P2_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P2_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P2_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int P2_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P2_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P2_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int P4_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P4_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P4_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int P4_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P4_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P4_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int P5_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P5_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P5_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int P5_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P5_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P5_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int P6_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P6_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P6_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int P6_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P6_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P6_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int Om_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set Om_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set Om_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int Om_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set Om_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set Om_Global_Powerlaw_V_Visit_wr[CORE_NUM];

int P7_Global_Iteration_ID_rd[CORE_NUM];
Powerlaw_Vid_Set P7_Global_Powerlaw_V_ID_rd[CORE_NUM];
Powerlaw_Vvisit_Set P7_Global_Powerlaw_V_Visit_rd[CORE_NUM];

int P7_Global_Iteration_ID_wr[CORE_NUM];
Powerlaw_Vid_Set P7_Global_Powerlaw_V_ID_wr[CORE_NUM];
Powerlaw_Vvisit_Set P7_Global_Powerlaw_V_Visit_wr[CORE_NUM];

// class Info_P_Global info_p_global_rd,info_p_global_wr;

// void global_connect(Pipe *pipes){
//     pipes[8].bind(&info_p_global_wr,&info_p_global_rd);
// }

void Global(pipe_manager* pipes){

    static Info_P_AIE_Global self_info_p_aie_global;
    static Info_P_Global self_p_global;
    static Info_P1 self_info_p1;
    pipes->write(&self_info_p_aie_global);
    pipes->write(&self_info_p1);
    RD_ACTIVE_VERTEX_GLOBAL_SIGNAL(self_info_p_aie_global.AIE_Active_V_ID, self_info_p_aie_global.AIE_Active_V_Updated, self_info_p_aie_global.AIE_Active_V_DValid,
                                    
                                    P1_Global_Powerlaw_V_ID_wr, P1_Global_Powerlaw_V_Visit_wr);
    
    Read_Active_Vertex_Offset_Global_Signal(self_info_p1.P1_Global_Iteration_ID, P1_Global_Powerlaw_V_ID_rd, P1_Global_Powerlaw_V_Visit_rd,
    
                                            P2_Global_Iteration_ID_wr, P2_Global_Powerlaw_V_ID_wr, P2_Global_Powerlaw_V_Visit_wr);

    RD_Active_Vertex_Edge_Global_Signal(P2_Global_Iteration_ID_rd, P2_Global_Powerlaw_V_ID_rd, P2_Global_Powerlaw_V_Visit_rd,
    
                                        P4_Global_Iteration_ID_wr, P4_Global_Powerlaw_V_ID_wr, P4_Global_Powerlaw_V_Visit_wr);

    Generate_HBM_Edge_Rqst_Global_Signal(P4_Global_Iteration_ID_rd, P4_Global_Powerlaw_V_ID_rd, P4_Global_Powerlaw_V_Visit_rd,
    
                                            P5_Global_Iteration_ID_wr, self_p_global.P5_Global_Powerlaw_V_ID, self_p_global.P5_Global_Powerlaw_V_Visit);

    Schedule_Global_Signal(P5_Global_Iteration_ID_rd, P5_Global_Powerlaw_V_ID_rd, P5_Global_Powerlaw_V_Visit_rd,
    
                            P6_Global_Iteration_ID_wr, P6_Global_Powerlaw_V_ID_wr, P6_Global_Powerlaw_V_Visit_wr);



    Omega_Network_Global_Signal(P6_Global_Iteration_ID_rd, P6_Global_Powerlaw_V_ID_rd, P6_Global_Powerlaw_V_Visit_rd,
    
                                self_p_global.Om_Global_Iteration_ID, Om_Global_Powerlaw_V_ID_wr, Om_Global_Powerlaw_V_Visit_wr);

    Backend_Core_Global_Signal(Om_Global_Iteration_ID_rd, Om_Global_Powerlaw_V_ID_rd, Om_Global_Powerlaw_V_Visit_rd,
    
                                self_p_global.P7_Global_Iteration_ID, P7_Global_Powerlaw_V_ID_wr, P7_Global_Powerlaw_V_Visit_wr);
    
    pipes->read(&self_p_global);
    
    for(int i = 0 ; i<CORE_NUM ; i++){
        
        // P1_Global_Iteration_ID_rd[i] = P1_Global_Iteration_ID_wr[i];
        P1_Global_Powerlaw_V_ID_rd[i] = P1_Global_Powerlaw_V_ID_wr[i];
        P1_Global_Powerlaw_V_Visit_rd[i] = P1_Global_Powerlaw_V_Visit_wr[i];
        
        P2_Global_Iteration_ID_rd[i] = P2_Global_Iteration_ID_wr[i];
        P2_Global_Powerlaw_V_ID_rd[i] = P2_Global_Powerlaw_V_ID_wr[i];
        P2_Global_Powerlaw_V_Visit_rd[i] = P2_Global_Powerlaw_V_Visit_wr[i];
        
        P4_Global_Iteration_ID_rd[i] = P4_Global_Iteration_ID_wr[i];
        P4_Global_Powerlaw_V_ID_rd[i] = P4_Global_Powerlaw_V_ID_wr[i];
        P4_Global_Powerlaw_V_Visit_rd[i] = P4_Global_Powerlaw_V_Visit_wr[i];

        P5_Global_Iteration_ID_rd[i] = P5_Global_Iteration_ID_wr[i];
        P5_Global_Powerlaw_V_ID_rd[i] = self_p_global.P5_Global_Powerlaw_V_ID[i];
        P5_Global_Powerlaw_V_Visit_rd[i] = self_p_global.P5_Global_Powerlaw_V_Visit[i];

        P6_Global_Iteration_ID_rd[i] = P6_Global_Iteration_ID_wr[i];
        P6_Global_Powerlaw_V_ID_rd[i] = P6_Global_Powerlaw_V_ID_wr[i];
        P6_Global_Powerlaw_V_Visit_rd[i] = P6_Global_Powerlaw_V_Visit_wr[i];

        P7_Global_Iteration_ID_rd[i] = P7_Global_Iteration_ID_wr[i];
        P7_Global_Powerlaw_V_ID_rd[i] = P7_Global_Powerlaw_V_ID_wr[i];
        P7_Global_Powerlaw_V_Visit_rd[i] = P7_Global_Powerlaw_V_Visit_wr[i];

        Om_Global_Iteration_ID_rd[i] = self_p_global.Om_Global_Iteration_ID[i];
        Om_Global_Powerlaw_V_ID_rd[i] = Om_Global_Powerlaw_V_ID_wr[i];
        Om_Global_Powerlaw_V_Visit_rd[i] = Om_Global_Powerlaw_V_Visit_wr[i];
    }
}