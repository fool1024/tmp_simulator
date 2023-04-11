//v_buffer ,iteration_buffer, && HBM_data must be synchronized here
#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_Schedule debug_M7;
#endif

extern int clk;
extern int rst_rd;

using namespace std;

//32 signals: stage_full
void Schedule_Single(int Core_ID,
                     int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_DValid, int Front_Iteration_End, int Front_Iteration_End_DValid,
                     int HBM_Iterface_Active_V_Edge, int HBM_Iterface_Active_V_Edge_Valid,
                     int Front2_Push_Flag, int Front2_Active_V_ID, int Front2_Active_V_Value, int Front2_Active_V_Edge, int Front2_Active_V_DValid, int Front2_Iteration_End, int Front2_Iteration_End_DValid,
                     Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit,
                     int NextStage_Full,

                     int *Stage_Full1, int *Stage_Full2,
                     int *Push_Flag, int *Update_V_ID, int *Update_V_Value, int *Pull_First_Flag, int *Update_V_DValid,
                     int *Iteration_End, int *Iteration_End_DValid);

void Schedule(int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Active_V_DValid[], int Front_Iteration_End[], int Front_Iteration_End_DValid[],
              int HBM_Iterface_Active_V_Edge[], int HBM_Iterface_Active_V_Edge_Valid[],
              int Front2_Push_Flag[], int Front2_Active_V_ID[], int Front2_Active_V_Value[], int Front2_Active_V_Edge[], int Front2_Active_V_DValid[], int Front2_Iteration_End[], int Front2_Iteration_End_DValid[],
              Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],
              int NextStage_Full[],

              int *Stage_Full1, int *Stage_Full2,
              int *Push_Flag, int *Update_V_ID, int *Update_V_Value, int *Pull_First_Flag, int *Update_V_DValid,
              int *Iteration_End, int *Iteration_End_DValid) {

    int tmp_stage_full[CORE_NUM];

    for (int i = 0; i < CORE_NUM; ++ i) {
        Schedule_Single(i,
                        Front_Push_Flag[i], Front_Active_V_ID[i], Front_Active_V_Value[i], Front_Active_V_DValid[i], Front_Iteration_End[i], Front_Iteration_End_DValid[i],
                        HBM_Iterface_Active_V_Edge[i], HBM_Iterface_Active_V_Edge_Valid[i],
                        Front2_Push_Flag[i], Front2_Active_V_ID[i], Front2_Active_V_Value[i], Front2_Active_V_Edge[i], Front2_Active_V_DValid[i], Front2_Iteration_End[i], Front2_Iteration_End_DValid[i],
                        Front_Global_Powerlaw_V_ID[i], Front_Global_Powerlaw_V_Visit[i],
                        NextStage_Full[i],

                        &tmp_stage_full[i], &Stage_Full2[i],
                        &Push_Flag[i], &Update_V_ID[i], &Update_V_Value[i], &Pull_First_Flag[i], &Update_V_DValid[i],
                        &Iteration_End[i], &Iteration_End_DValid[i]);
    }

    int tmp_stage_full_group;
    for (int i = 0; i < PSEUDO_CHANNEL_NUM; ++ i) {
        tmp_stage_full_group = 0;
        for (int j = i * GROUP_CORE_NUM; j < (i + 1) * GROUP_CORE_NUM; ++ j) {
            tmp_stage_full_group = tmp_stage_full_group || tmp_stage_full[j];
        }

        Stage_Full1[i] = tmp_stage_full_group;
    }
}


void Schedule_Single(int Core_ID,
                     int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_DValid, int Front_Iteration_End, int Front_Iteration_End_DValid,
                     int HBM_Iterface_Active_V_Edge, int HBM_Iterface_Active_V_Edge_Valid,
                     int Front2_Push_Flag, int Front2_Active_V_ID, int Front2_Active_V_Value, int Front2_Active_V_Edge, int Front2_Active_V_DValid, int Front2_Iteration_End, int Front2_Iteration_End_DValid,
                     Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit,
                     int NextStage_Full,

                     int *Stage_Full1, int *Stage_Full2,
                     int *Push_Flag, int *Update_V_ID, int *Update_V_Value, int *Pull_First_Flag, int *Update_V_DValid,
                     int *Iteration_End, int *Iteration_End_DValid) {

    static queue<int> push_flag_buffer1[CORE_NUM], v_id_buffer1[CORE_NUM], v_value_buffer1[CORE_NUM], edge_buffer1[CORE_NUM];
    static queue<int> push_flag_buffer2[CORE_NUM], v_id_buffer2[CORE_NUM], v_value_buffer2[CORE_NUM], edge_buffer2[CORE_NUM];
    static int v_buffer_empty1[CORE_NUM], e_buffer_empty1[CORE_NUM];
    static int v_buffer_empty2[CORE_NUM];

    #if DEBUG
        debug_M7._push_flag_buffer1 = push_flag_buffer1;
        debug_M7._push_flag_buffer2 = push_flag_buffer2;
        debug_M7._v_id_buffer1 = v_id_buffer1;
        debug_M7._v_id_buffer2 = v_id_buffer2;
        debug_M7._v_value_buffer1 = v_value_buffer1;
        debug_M7._v_value_buffer2 = v_value_buffer2;
        debug_M7._edge_buffer1 = edge_buffer1;
        debug_M7._edge_buffer2 = edge_buffer2;
    #endif

    v_buffer_empty1[Core_ID] = (v_id_buffer1[Core_ID].size() == 0);
    e_buffer_empty1[Core_ID] = (edge_buffer1[Core_ID].size() == 0);
    v_buffer_empty2[Core_ID] = (v_id_buffer2[Core_ID].size() == 0);

    if (rst_rd || NextStage_Full) {
        *Push_Flag = 0;
        *Update_V_ID = 0;
        *Update_V_Value = 0;
        *Pull_First_Flag = 0;
        *Update_V_DValid = 0;
    }
    else {
        //task from buffer 1 will never be the first edge task in pull model
        if (edge_buffer1[Core_ID].size() > v_id_buffer2[Core_ID].size()) {
            if (v_buffer_empty1[Core_ID] || e_buffer_empty1[Core_ID]) cout << "error in schedule" << endl;

           if (push_flag_buffer1[Core_ID].front()) {
                // if (Core_ID == 1) printf("front_id=%d\n", v_id_buffer1[Core_ID].front());
                if (v_id_buffer1[Core_ID].front() != -1) {
                    *Push_Flag = push_flag_buffer1[Core_ID].front();
                    *Update_V_ID = edge_buffer1[Core_ID].front();
                    *Update_V_Value = v_value_buffer1[Core_ID].front();
                    *Pull_First_Flag = 0;
                    *Update_V_DValid = 1;
                    #if (DEBUG && PRINT_CONS)
                        if (*Update_V_ID == PRINT_ID) {
                            printf("clk=%d, push_flag=%d, update_v_id=%d, update_v_value=%d, pull_first_flag=%d, update_v_dvalid=%d\n", clk, *Push_Flag, *Update_V_ID, *Update_V_Value, *Pull_First_Flag, *Update_V_DValid);
                        }
                    #endif
                } else {
                    *Push_Flag = 0;
                    *Update_V_ID = 0;
                    *Update_V_Value = 0;
                    *Pull_First_Flag = 0;
                    *Update_V_DValid = 0;
                }
            }
            else {
                // special case for powerlaw vertex in pull mode
                bool ch_flag = false;
                for (int i = 0; i < POWER_LAW_V_NUM; i++) {
                    // if (edge_buffer1[Core_ID].front() == Front_Global_Powerlaw_V_ID.v[i] && Front_Global_Powerlaw_V_Visit.visit[i]) {
                    if (edge_buffer1[Core_ID].front() == Front_Global_Powerlaw_V_ID.v[i]) {
                        ch_flag = true;
                    }
                    if (edge_buffer1[Core_ID].front() == VERTEX_NOT_READ[i] && edge_buffer1[Core_ID].front() != Front_Global_Powerlaw_V_ID.v[i]) {
                        printf("catch signal not match, edge_buffer=%d, front=%d\n", edge_buffer1[Core_ID].front(), Front_Global_Powerlaw_V_ID.v[i]);
                        exit(-1);
                    }
                }
                *Push_Flag = push_flag_buffer1[Core_ID].front();
                *Update_V_ID = v_id_buffer1[Core_ID].front();
                *Update_V_Value = ch_flag ? v_id_buffer1[Core_ID].front() : edge_buffer1[Core_ID].front();
                *Pull_First_Flag = 0;
                *Update_V_DValid = 1;
                #if (DEBUG && PRINT_CONS)
                    if (*Update_V_ID == PRINT_ID) {
                        printf("push_flag=%d, update_v_id=%d, update_v_value=%d, pull_first_flag=%d, update_v_dvalid=%d\n", *Push_Flag, *Update_V_ID, *Update_V_Value, *Pull_First_Flag, *Update_V_DValid);
                    }
                #endif
            }

            push_flag_buffer1[Core_ID].pop();
            v_id_buffer1[Core_ID].pop();
            v_value_buffer1[Core_ID].pop();
            edge_buffer1[Core_ID].pop();

        }
        else {
            //both buffer 1 and 2 are empty
            if (v_buffer_empty2[Core_ID]) {
                *Push_Flag = 0;
                *Update_V_ID = 0;
                *Update_V_Value = 0;
                *Pull_First_Flag = 0;
                *Update_V_DValid = 0;
            }
            //task from buffer will always be the first edge task in pull model
            else {
                // special case for powerlaw vertex in pull mode
                bool ch_flag = false;
                for (int i = 0; i < POWER_LAW_V_NUM; i++) {
                    // if (edge_buffer2[Core_ID].front() == Front_Global_Powerlaw_V_ID.v[i] && Front_Global_Powerlaw_V_Visit.visit[i]) {
                    if (edge_buffer2[Core_ID].front() == Front_Global_Powerlaw_V_ID.v[i]) {
                        ch_flag = true;
                    }
                    if (edge_buffer1[Core_ID].front() == VERTEX_NOT_READ[i] && edge_buffer1[Core_ID].front() != Front_Global_Powerlaw_V_ID.v[i]) {
                        printf("catch signal not match, edge_buffer=%d, front=%d\n", edge_buffer1[Core_ID].front(), Front_Global_Powerlaw_V_ID.v[i]);
                        exit(-1);
                    }
                }
                *Push_Flag = push_flag_buffer2[Core_ID].front();
                *Update_V_ID = v_id_buffer2[Core_ID].front();
                *Update_V_Value = ch_flag ? v_id_buffer2[Core_ID].front() : edge_buffer2[Core_ID].front();
                *Pull_First_Flag = 1;
                *Update_V_DValid = 1;
                #if (DEBUG && PRINT_CONS)
                    if (*Update_V_ID == PRINT_ID) {
                        printf("push_flag=%d, update_v_id=%d, update_v_value=%d, pull_first_flag=%d, update_v_dvalid=%d\n", *Push_Flag, *Update_V_ID, *Update_V_Value, *Pull_First_Flag, *Update_V_DValid);
                    }
                #endif

                push_flag_buffer2[Core_ID].pop();
                v_id_buffer2[Core_ID].pop();
                v_value_buffer2[Core_ID].pop();
                edge_buffer2[Core_ID].pop();
            }
        }
    }

    if (rst_rd) {
        /*initialize push_flag1, v_id1, v_value1 buffers*/
        while (!push_flag_buffer1[Core_ID].empty())   push_flag_buffer1[Core_ID].pop();
        while (!v_id_buffer1[Core_ID].empty())        v_id_buffer1[Core_ID].pop();
        while (!v_value_buffer1[Core_ID].empty())     v_value_buffer1[Core_ID].pop();
    }
    else {
        if (Front_Active_V_DValid) {
            push_flag_buffer1[Core_ID].push(Front_Push_Flag);
            v_id_buffer1[Core_ID].push(Front_Active_V_ID);
            v_value_buffer1[Core_ID].push(Front_Active_V_Value);
        }
    }

    if (rst_rd) {
        /*initialize edge_buffer1*/
        while (!edge_buffer1[Core_ID].empty())    edge_buffer1[Core_ID].pop();
    }
    else {
        if (HBM_Iterface_Active_V_Edge_Valid) {
            edge_buffer1[Core_ID].push(HBM_Iterface_Active_V_Edge);
        }
    }

    if (rst_rd) {
        /* initialize push_flag2, v_id2, v_value2, edge2 buffers */
        while (!push_flag_buffer2[Core_ID].empty())   push_flag_buffer2[Core_ID].pop();
        while (!v_id_buffer2[Core_ID].empty())        v_id_buffer2[Core_ID].pop();
        while (!v_value_buffer2[Core_ID].empty())     v_value_buffer2[Core_ID].pop();
        while (!edge_buffer2[Core_ID].empty())        edge_buffer2[Core_ID].pop();
    }
    else {
        if (Front2_Active_V_DValid) {
            push_flag_buffer2[Core_ID].push(Front2_Push_Flag);
            v_id_buffer2[Core_ID].push(Front2_Active_V_ID);
            v_value_buffer2[Core_ID].push(Front2_Active_V_Value);
            edge_buffer2[Core_ID].push(Front2_Active_V_Edge);
        }
    }

    if (!rst_rd && Front_Iteration_End && Front_Iteration_End_DValid && Front2_Iteration_End && Front2_Iteration_End_DValid && v_buffer_empty1[Core_ID] && v_buffer_empty2[Core_ID]) {
        *Iteration_End = 1;
        *Iteration_End_DValid = 1;
    }
    else {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }

    *Stage_Full1 = (v_id_buffer1[Core_ID].size() >= FIFO_SIZE);
    *Stage_Full2 = (v_id_buffer2[Core_ID].size() >= FIFO_SIZE);
}