#include "Framesupport.h"
#include "Network.h"
#include "MEM.h"
#include "global.h"

config(P1,int P1_Global_Iteration_ID[CORE_NUM];)

config(P6,int P6_Push_Flag[CORE_NUM];
    int P6_Update_V_ID[CORE_NUM];
    int P6_Update_V_Value[CORE_NUM] ;
    int P6_Pull_First_Flag[CORE_NUM];
    int P6_Update_V_DValid[CORE_NUM] ;
    int P6_Iteration_End[CORE_NUM];
    int P6_Iteration_End_DValid[CORE_NUM];)

config(P5,int P5_Rd_HBM_Edge_Addr[CORE_NUM];
    int P5_Rd_HBM_Edge_Valid[CORE_NUM];)

config(P7,int P7_Source_Core_Full[CORE_NUM]; 
    int P7_Om2_Send_Push_Flag[CORE_NUM]; 
    int P7_Om2_Send_Update_V_ID[CORE_NUM]; 
    int P7_Om2_Send_Update_V_Value[CORE_NUM]; 
    int P7_Om2_Send_Update_V_Pull_First_Flag[CORE_NUM]; 
    int P7_Om2_Send_Update_V_DValid[CORE_NUM]; 
    int P7_Om2_Iteration_End[CORE_NUM]; 
    int P7_Om2_Iteration_End_DValid[CORE_NUM]; )

config(P_Om1,int Om1_Stage_Full[CORE_NUM];
    int Om1_Push_Flag[CORE_NUM];
    int Om1_Update_V_ID[CORE_NUM];
    int Om1_Update_V_Value[CORE_NUM];
    int Om1_Update_V_Pull_First_Flag[CORE_NUM];
    int Om1_Update_V_DValid[CORE_NUM];
    int Om1_Iteration_End[CORE_NUM];
    int Om1_Iteration_End_DValid[CORE_NUM];)

config(P_Om2,int Om2_Stage_Full[CORE_NUM];
    int Om2_Push_Flag[CORE_NUM];
    int Om2_Update_V_ID[CORE_NUM];
    int Om2_Update_V_Value[CORE_NUM];
    int Om2_Update_V_Pull_First_Flag[CORE_NUM];
    int Om2_Update_V_DValid[CORE_NUM];
    int Om2_Iteration_End[CORE_NUM];
    int Om2_Iteration_End_DValid[CORE_NUM];)

config(P_AIE_Global,int AIE_Active_V_ID[CORE_NUM];
    int AIE_Active_V_Updated[CORE_NUM];
    int AIE_Active_V_DValid[CORE_NUM];)

config(P_HBM,int HBM_Interface_Full[CORE_NUM];
    int HBM_Interface_Active_V_Edge[CORE_NUM];
    int HBM_Interface_Active_V_Edge_Valid[CORE_NUM];)

config(P_Global,Powerlaw_Vid_Set P5_Global_Powerlaw_V_ID[CORE_NUM];
    Powerlaw_Vvisit_Set P5_Global_Powerlaw_V_Visit[CORE_NUM];
    int Om_Global_Iteration_ID[CORE_NUM];
    int P7_Global_Iteration_ID[CORE_NUM];)

config(P_URAM2Edge,int Offset_Uram_Active_V_LOffset[CORE_NUM];
                   int Offset_Uram_Active_V_ROffset[CORE_NUM];
                   int Offset_Uram_Active_V_DValid[CORE_NUM];)

config(P_Offset2URAM,int P2_Active_V_Offset_Addr[CORE_NUM];
                     int P2_Active_V_DValid[CORE_NUM];)

/*next is core_pipe*/


//int P1_Push_Flag[CORE_NUM]; int P1_Active_V_ID[CORE_NUM]; int P1_Active_V_Value[CORE_NUM]; int P1_Active_V_Pull_First_Flag[CORE_NUM]; int P1_Active_V_DValid[CORE_NUM]; int P1_Iteration_End[CORE_NUM]; int P1_Iteration_End_Dvalid[CORE_NUM];
config(P12P2,int P1_Push_Flag[CORE_NUM];
       int P1_Active_V_ID[CORE_NUM];
       int P1_Active_V_Value[CORE_NUM];
       int P1_Active_V_Pull_First_Flag[CORE_NUM];
       int P1_Active_V_DValid[CORE_NUM];
       int P1_Iteration_End[CORE_NUM];
       int P1_Iteration_End_Dvalid[CORE_NUM];)






