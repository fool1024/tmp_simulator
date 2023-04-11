// #include "Network.h"
#include "Accelerator.h"
#include<iostream>
using namespace std;
Network_reg Nreg_1;
Network_reg Nreg_2;

int P6_Source_Core_Full_rd[CORE_NUM];

int sum1 = 0, sum2 = 0, sum3 = 0;
void Noc(pipe_manager* pipes){
    static Info_P6 self_info_p6;
    static Info_P_Om1 self_info_om1;
    static Info_P_Om2 self_info_om2;
    static Info_P7 self_info_p7;
    
    pipes->write(&self_info_p6);
    pipes->write(&self_info_p7);


    Network(self_info_p6.P6_Push_Flag, self_info_p6.P6_Update_V_ID, self_info_p6.P6_Update_V_Value, self_info_p6.P6_Pull_First_Flag, self_info_p6.P6_Update_V_DValid, self_info_p6.P6_Iteration_End, self_info_p6.P6_Iteration_End_DValid,
            self_info_p7.P7_Source_Core_Full,

            self_info_om1.Om1_Stage_Full,
            self_info_om1.Om1_Push_Flag, self_info_om1.Om1_Update_V_ID, self_info_om1.Om1_Update_V_Value, self_info_om1.Om1_Update_V_Pull_First_Flag, self_info_om1.Om1_Update_V_DValid, self_info_om1.Om1_Iteration_End, self_info_om1.Om1_Iteration_End_DValid,
            Nreg_1,
            0,0);
    
    for(int i = 0 ; i<CORE_NUM ; i++){
        sum1 += self_info_p7.P7_Om2_Send_Update_V_DValid[i]? 1 : 0;
    }
    if(clk % 500 == 0){
        cout << "clk:" << clk << endl;
        cout << "sum1  = " << sum1 << endl;
    }

    Network(self_info_p7.P7_Om2_Send_Push_Flag, self_info_p7.P7_Om2_Send_Update_V_ID, self_info_p7.P7_Om2_Send_Update_V_Value, self_info_p7.P7_Om2_Send_Update_V_Pull_First_Flag, self_info_p7.P7_Om2_Send_Update_V_DValid, self_info_p7.P7_Om2_Iteration_End, self_info_p7.P7_Om2_Iteration_End_DValid,
            P6_Source_Core_Full_rd,

            self_info_om2.Om2_Stage_Full,
            self_info_om2.Om2_Push_Flag, self_info_om2.Om2_Update_V_ID, self_info_om2.Om2_Update_V_Value, self_info_om2.Om2_Update_V_Pull_First_Flag, self_info_om2.Om2_Update_V_DValid, self_info_om2.Om2_Iteration_End, self_info_om2.Om2_Iteration_End_DValid,
            Nreg_2,
            1,0);
    for(int i = 0 ; i<CORE_NUM ; i++){
        sum2 += self_info_om2.Om2_Update_V_DValid[i]? 1 : 0;
    }
    if(clk % 500 == 0)
    cout << "sum2  = " << sum2 << endl;
    if(clk % 500 == 0){
        sum3 = 0;
        for(int j = 0 ; j<10 ; j++){
            for(int i = 0 ; i<OMEGA_SWITCH_NUM ; i++){
                sum3 += Nreg_2.id_buffer_in1_out1[j][i].size();
                sum3 += Nreg_2.id_buffer_in1_out2[j][i].size();
                sum3 += Nreg_2.id_buffer_in2_out1[j][i].size();
                sum3 += Nreg_2.id_buffer_in2_out2[j][i].size();
                if(Nreg_2.id_buffer_in1_out1[j][i].size() > 64){
                    cout << "size:"<< Nreg_2.id_buffer_in1_out1[j][i].size() << " id_buffer1_1:" << Nreg_2.id_buffer_in1_out1[j][i].front()<<" "<<"value_buffer1_1:" << Nreg_2.value_buffer_in1_out1[j][i].front() <<endl;
                }
                if(Nreg_2.id_buffer_in1_out2[j][i].size() > 64){
                    cout << "size:"<< Nreg_2.id_buffer_in1_out2[j][i].size() << " id_buffer1_2:" << Nreg_2.id_buffer_in1_out2[j][i].front()<<" "<<"value_buffer1_2:" << Nreg_2.value_buffer_in1_out2[j][i].front() <<endl;
                }
                if(Nreg_2.id_buffer_in2_out1[j][i].size() > 64){
                    cout << "size:"<< Nreg_2.id_buffer_in2_out1[j][i].size() << " id_buffer2_1:" << Nreg_2.id_buffer_in2_out1[j][i].front()<<" "<<"value_buffer2_1:" << Nreg_2.value_buffer_in2_out1[j][i].front() <<endl;
                }
                if(Nreg_2.id_buffer_in2_out2[j][i].size() > 64){
                    cout << "size:"<< Nreg_2.id_buffer_in2_out2[j][i].size() << " id_buffer2_2:" << Nreg_2.id_buffer_in2_out2[j][i].front()<<" "<<"value_buffer2_2:" << Nreg_2.value_buffer_in2_out2[j][i].front() <<endl;
                }
            }
        }
        cout << "sum3  = " << sum3 << endl;
    }
    for (int i = 0; i < CORE_NUM; ++ i) {
        // 9 - 1 - 1 - 3
        P6_Source_Core_Full_rd[i] = 0;
    }
    pipes->read(&self_info_om1);
    pipes->read(&self_info_om2);
}
