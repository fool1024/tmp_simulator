#include "Accelerator.h"

#if DEBUG
    #include "debug.h"
    debug_RD_ACTIVE_VERTEX_SINGLE debug_M1;
    int DEBUG_PUSH_FLAG = 0;
    int DEBUG_NEXT_PUSH_FLAG = 0;
#endif

using namespace std;

extern int clk;
extern int rst_rd;
extern int rst_root;
extern int ROOT_ID;
extern bitmap Init_Bitmap[CORE_NUM][BitMap_Compressed_NUM + 1];
extern int iteration_end_flag;
extern int MAX_ITERATION;
extern int vertex_updated;

void RD_ACTIVE_VERTEX_SINGLE(int Core_ID,
                             int Backend_Active_V_ID, int Backend_Active_V_Updated, int Backend_Active_V_Pull_First_Flag, int Backend_Active_V_DValid,
                             int Backend_Iteration_End, int Backend_Iteration_End_DValid,
                             int NextStage_Full,

                             int *Push_Flag,
                             int *Active_V_ID, int *Active_V_Value, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                             int *Iteration_End, int *Iteration_End_DValid,
                             int *Global_Iteration_ID);

void RD_ACTIVE_VERTEX(int Backend_Active_V_ID[], int Backend_Active_V_Updated[], int Backend_Active_V_Pull_First_Flag[], int Backend_Active_V_DValid[],
                      int Backend_Iteration_End[], int Backend_Iteration_End_DValid[],
                      int NextStage_Full[],

                      Info_P12P2& self_info_p12p2,
                      int *Global_Iteration_ID) {
    Powerlaw_Vvisit_Set tmp_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; ++i) {
        RD_ACTIVE_VERTEX_SINGLE(i,
                               Backend_Active_V_ID[i], Backend_Active_V_Updated[i], Backend_Active_V_Pull_First_Flag[i], Backend_Active_V_DValid[i],
                               Backend_Iteration_End[i], Backend_Iteration_End_DValid[i],
                               NextStage_Full[i],

                               &(self_info_p12p2.P1_Push_Flag[i]),
                               &(self_info_p12p2.P1_Active_V_ID[i]), &(self_info_p12p2.P1_Active_V_Value[i]), &(self_info_p12p2.P1_Active_V_Pull_First_Flag[i]), &(self_info_p12p2.P1_Active_V_DValid[i]),
                               &(self_info_p12p2.P1_Iteration_End[i]), &(self_info_p12p2.P1_Iteration_End_Dvalid[i]),
                               &Global_Iteration_ID[i]);
    }
}

void RD_ACTIVE_VERTEX_GLOBAL_SIGNAL(int Backend_Active_V_ID[], int Backend_Active_V_Updated[], int Backend_Active_V_DValid[],

                                    Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static Powerlaw_Vid_Set powerlaw_v_id[CORE_NUM];
    static Powerlaw_Vvisit_Set powerlaw_v_visit[CORE_NUM];

    Powerlaw_Vvisit_Set tmp_visit;
    for (int i = 0; i < CORE_NUM; i++) {
        for (int j = 0; j < POWER_LAW_V_NUM; j++) {
            if (powerlaw_v_visit[i].visit[j]) {
                tmp_visit.visit[j] = true;
            }
        }
    }

    for (int core_id = 0; core_id < CORE_NUM; core_id++) {
        // assign
        Global_Powerlaw_V_ID[core_id].set(powerlaw_v_id[core_id]);
        Global_Powerlaw_V_Visit[core_id].set(powerlaw_v_visit[core_id]);
        // reg
        if (rst_rd) {
            for (int i = 0; i < POWER_LAW_V_NUM; i++) {
                powerlaw_v_id[core_id].v[i] = VERTEX_NOT_READ[i];
                powerlaw_v_visit[core_id].visit[i] = false;
            }
        } else {
            if (Backend_Active_V_ID[core_id] % CORE_NUM == core_id && Backend_Active_V_DValid[core_id] && Backend_Active_V_Updated[core_id]) {
                for (int i = 0; i < POWER_LAW_V_NUM; i++) {
                    if (Backend_Active_V_ID[core_id] == VERTEX_NOT_READ[i]) {
                        powerlaw_v_visit[core_id].visit[i] = true;
                    }
                }
            }
        }
    }
}

void RD_ACTIVE_VERTEX_SINGLE(int Core_ID,
                             int Backend_Active_V_ID, int Backend_Active_V_Updated, int Backend_Active_V_Pull_First_Flag, int Backend_Active_V_DValid,
                             int Backend_Iteration_End, int Backend_Iteration_End_DValid,
                             int NextStage_Full,

                             int *Push_Flag,
                             int *Active_V_ID, int *Active_V_Value, int *Active_V_Pull_First_Flag, int *Active_V_DValid,
                             int *Iteration_End, int *Iteration_End_DValid,
                             int *Global_Iteration_ID) {

    ///local data structures. static int -> reg, int -> wire
    //control signal
    static int iteration_id[CORE_NUM];
    static int init_bitmap_id[CORE_NUM];
    static int iteration_end[CORE_NUM];

    //push mode: active vertex queue
    static queue<int> active_vid_queue[2][CORE_NUM]; //1: now_active_vid_queue, 2: next_active_vid_queue
    static int now_buffer_empty[CORE_NUM];

    //pull mode: active vertex map, each element represents 32 vertices
    static bitmap vis_bitmap[CORE_NUM][BitMap_Compressed_NUM + 1];
    static int vis_bitmap_index[CORE_NUM];
    static bitmap vis_bitmap_now[CORE_NUM];
    static int scheduled_vertex_num;

    static int iteration_flag[CORE_NUM]; //用来判断传进来的iteration end是否是连续的（无效的）
    static int task_num[CORE_NUM]; //用来确认pull模式下分发的任务未超过流水线总和
    //static bool task_visit[MAX_VERTEX_NUM];
    #if DEBUG
        debug_M1._iteration_id = iteration_id;
        debug_M1._init_bitmap_id = init_bitmap_id;
        debug_M1._iteration_end = iteration_end;
        debug_M1._active_vid_queue = active_vid_queue;
        debug_M1._vis_bitmap = vis_bitmap;
        debug_M1._vis_bitmap_index = vis_bitmap_index;
        debug_M1._vis_bitmap_now = vis_bitmap_now;
    #endif

    int now_buffer_id = iteration_id[Core_ID] % 2;
    int next_buffer_id = (iteration_id[Core_ID] + 1) % 2;

    #if (DEBUG && CONTROL_FLAG)
        int now_push_flag = DEBUG_PUSH_FLAG;
        int next_push_flag = DEBUG_PUSH_FLAG;
    #else
        int now_push_flag = (iteration_id[Core_ID] <= 1 || iteration_id[Core_ID] >= 5);
        int next_push_flag = ((iteration_id[Core_ID] <= 0 || iteration_id[Core_ID] >= 4));
    #endif

    iteration_end_flag = (iteration_id[Core_ID] == MAX_ITERATION);

    /**do not OR rst_rd**/
    now_buffer_empty[Core_ID] = (active_vid_queue[now_buffer_id][Core_ID].size() == 0);

    ///temporary data, using wire & assign
    int root_core_id = ROOT_ID % CORE_NUM;
    int root_bitmap_id1 = (ROOT_ID / CORE_NUM) / BitMap_Compressed_Length;
    int root_bitmap_id2 = (ROOT_ID / CORE_NUM) % BitMap_Compressed_Length;

    int backend_v_bitmap_id1 = (Backend_Active_V_ID / CORE_NUM) / BitMap_Compressed_Length;
    int backend_v_bitmap_id2 = (Backend_Active_V_ID / CORE_NUM) % BitMap_Compressed_Length;

    ///using wires to find an active vertex for pull mode
    int max_length = BitMap_Compressed_Length;
    int pull_active_bitmap[max_length], pull_active_bitmap_index[max_length];

    for (int i = 0; i < max_length; ++ i) {
        pull_active_bitmap[i] = vis_bitmap_now[Core_ID].v[i];
        pull_active_bitmap_index[i] = i;
    }
    while (max_length > 1) {
        max_length /= 2;
        for (int i = 0; i < max_length; ++ i) {
            pull_active_bitmap_index[i] = (pull_active_bitmap[2 * i] == 0) ? pull_active_bitmap_index[2 * i] : pull_active_bitmap_index[2 * i + 1];
            pull_active_bitmap[i] = pull_active_bitmap[2 * i] & pull_active_bitmap[2 * i + 1];
            /*
            if (vis_bitmap_now[Core_ID].v[0] == 1 && pull_active_bitmap[2 * i + 1] == 1) {
                printf("%d\n", pull_active_bitmap_index[i]);
            } else if (vis_bitmap_now[Core_ID].v[0] == 1 && pull_active_bitmap[0] != 1) {
                printf("error, max_l=%d\n", max_length);
                exit(-1);
            }
            */
        }
    }

    int pull_first_active_vertex_index = pull_active_bitmap_index[0];
    int pull_first_active_vertex_bitmap = pull_active_bitmap[0];

    //Send active vertex to the next pipeline stage
    if (rst_rd || init_bitmap_id[Core_ID] < BitMap_Compressed_NUM) {
        scheduled_vertex_num = 0;

        *Push_Flag = 0;
        *Active_V_ID = 0;
        *Active_V_Value = 0;
        *Active_V_Pull_First_Flag = 0;
        *Active_V_DValid = 0;
        task_num[Core_ID] = 0;
    }
    else {
        if (now_push_flag) {
            scheduled_vertex_num = 0;
            task_num[Core_ID] = 0;

            if (NextStage_Full || now_buffer_empty[Core_ID]) {
                *Push_Flag = 0;
                *Active_V_ID = 0;
                *Active_V_Value = 0;
                *Active_V_Pull_First_Flag = 0;
                *Active_V_DValid = 0;
            }
            else {
                *Push_Flag = now_push_flag;
                *Active_V_ID = active_vid_queue[now_buffer_id][Core_ID].front();
                *Active_V_Value = iteration_id[Core_ID] + 1;
                *Active_V_Pull_First_Flag = 0;
                *Active_V_DValid = 1;
                #if (DEBUG && PRINT_CONS)
                    if (*Active_V_ID == PRINT_ID) {
                        printf("push_flag=%d, active_v_id=%d, active_v_value=%d, active_v_pull_first_flag=%d, active_v_dvalid=%d\n", *Push_Flag, *Active_V_ID, *Active_V_Value, *Active_V_Pull_First_Flag, *Active_V_DValid);
                    }
                #endif

                active_vid_queue[now_buffer_id][Core_ID].pop();
            }
        }
        else {
            if (NextStage_Full) {
                if(Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag == 1 && Backend_Active_V_Updated == 0){
                    *Push_Flag = now_push_flag;
                    *Active_V_ID = Backend_Active_V_ID;
                    *Active_V_Value = iteration_id[Core_ID] + 1;
                    *Active_V_Pull_First_Flag = 0;
                    *Active_V_DValid = 1;
                    printf("clk:%d next stage full, core=%d, task_num=%d\n", clk, Core_ID, task_num[Core_ID]);
                    exit(1);
                } else {
                    *Push_Flag = 0;
                    *Active_V_ID = 0;
                    *Active_V_Value = 0;
                    *Active_V_Pull_First_Flag = 0;
                    *Active_V_DValid = 0;
                    if (Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag) {
                        task_num[Core_ID]--;
                    }
                }
            }
            else {
                //any vertex finishes first edge process and is not updated
                if (Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag == 1 && Backend_Active_V_Updated == 0) {
                    *Push_Flag = now_push_flag;
                    *Active_V_ID = Backend_Active_V_ID;
                    *Active_V_Value = iteration_id[Core_ID] + 1;
                    *Active_V_Pull_First_Flag = 0;
                    *Active_V_DValid = 1;
                    task_num[Core_ID]--;
                }
                else {
                    //pull does not end and any vertex in current bitmap line is not visited
                    if (vis_bitmap_index[Core_ID] < BitMap_Compressed_NUM && pull_first_active_vertex_bitmap == 0 && task_num[Core_ID] < MAX_TASK_NUM) {
                        *Push_Flag = now_push_flag;
                        *Active_V_ID = (vis_bitmap_index[Core_ID] * BitMap_Compressed_Length + pull_first_active_vertex_index) * CORE_NUM + Core_ID;
                        *Active_V_Value = iteration_id[Core_ID] + 1;
                        *Active_V_Pull_First_Flag = 1;
                        *Active_V_DValid = 1;

                        scheduled_vertex_num ++;
                        if (!(Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag)) {
                            task_num[Core_ID]++;
                        }
                        #if (DEBUG && PRINT_CONS)
                            if (*Active_V_ID == PRINT_ID) {
                                printf("clk=%d, push_flag=%d, active_v_id=%d, active_v_value=%d, active_v_pull_first_flag=%d, active_v_dvalid=%d\n", clk, *Push_Flag, *Active_V_ID, *Active_V_Value, *Active_V_Pull_First_Flag, *Active_V_DValid);
                            }
                            for (int i = 0; i < POWER_LAW_V_NUM; i++) {
                                if (*Active_V_ID == VERTEX_NOT_READ[i]) {
                                    printf("Warning: send power-law vertex %d\n", *Active_V_ID);
                                    break;
                                }
                            }
                        #endif
                    }
                    else {
                        *Push_Flag = 0;
                        *Active_V_ID = 0;
                        *Active_V_Value = 0;
                        *Active_V_Pull_First_Flag = 0;
                        *Active_V_DValid = 0;
                        if (Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag) {
                            task_num[Core_ID]--;
                        }
                    }
                }
            }
            //any vertex finishes first edge process and is updated
            if(Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag == 1 && Backend_Active_V_Updated == 1){
                scheduled_vertex_num --;
            }
        }
    }

    ///iteration end management
    if (rst_rd) {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }
    else {
        if (iteration_end[Core_ID]) {
            *Iteration_End = 1;
            *Iteration_End_DValid = 1;
        }
        else {
            *Iteration_End = 0;
            *Iteration_End_DValid = 0;
        }
    }

    ///iteration_id management
    if (rst_rd) {
        iteration_id[Core_ID] = 0;
        iteration_end[Core_ID] = 0;
    }
    else {
        if (iteration_end[Core_ID] == 1) {
            if (Backend_Iteration_End && Backend_Iteration_End_DValid && iteration_flag[Core_ID]) {
                #if (DEBUG)
                    if (Core_ID == PRINT_ID % CORE_NUM) {
                        int next_q_size = 0;
                        for (int i = 0; i < CORE_NUM; i++) {
                            next_q_size += active_vid_queue[next_buffer_id][i].size();
                        }
                        printf("mode=%d, iteration=%d, vertex_updated=%d, next_q_size=%d, clk=%d\n", now_push_flag, iteration_id[Core_ID], vertex_updated, next_q_size, clk);
                    }
                #endif
                iteration_id[Core_ID] ++;
                iteration_end[Core_ID] = 0;
                //memset(task_visit,0,sizeof task_visit);
            }
        }
        else {
            if (now_push_flag) {
                if (now_buffer_empty[Core_ID] && init_bitmap_id[Core_ID] >= BitMap_Compressed_NUM) {
                    iteration_end[Core_ID] = 1;
                }
            }
            else {
                // if (vis_bitmap_index[Core_ID] >= BitMap_Compressed_NUM && scheduled_vertex_num == 0) {
                if (vis_bitmap_index[Core_ID] >= BitMap_Compressed_NUM && task_num[Core_ID] == 0) {
                    iteration_end[Core_ID] = 1;
                }
            }
        }
    }

    if (rst_rd) {
        vis_bitmap_now[Core_ID] = vis_bitmap[Core_ID][0];
        vis_bitmap_index[Core_ID] = 0;
    }
    else {
        if (init_bitmap_id[Core_ID] > 0) {
            if (now_push_flag || (vis_bitmap_index[Core_ID] >= BitMap_Compressed_NUM && Backend_Iteration_End && Backend_Iteration_End_DValid && iteration_flag[Core_ID])) {
                if (!now_push_flag && Core_ID == PRINT_ID % CORE_NUM) {
                    printf("[INFO] %d task do not complete in pull-first mode\n", scheduled_vertex_num);
                }
                vis_bitmap_now[Core_ID] = vis_bitmap[Core_ID][0];
                vis_bitmap_index[Core_ID] = 0;
            }
            else {
                //pull does not end
                if (init_bitmap_id[Core_ID] == BitMap_Compressed_NUM) {
                    if (vis_bitmap_index[Core_ID] < init_bitmap_id[Core_ID]) {
                        // if (Core_ID == 0) printf("%d %d\n", vis_bitmap_index[Core_ID], init_bitmap_id[Core_ID]);
                        if (pull_first_active_vertex_bitmap == 1) {
                            vis_bitmap_now[Core_ID] = vis_bitmap[Core_ID][vis_bitmap_index[Core_ID] + 1];
                            vis_bitmap_index[Core_ID] ++;
                            // if (Core_ID == 0) printf("set_vis_index=%d\n", vis_bitmap_index[Core_ID]);
                        }
                        else {
                            //any vertex in current bitmap line is scheduled
                            if (!NextStage_Full && !(Backend_Active_V_DValid && Backend_Active_V_Pull_First_Flag == 1 && Backend_Active_V_Updated == 0) && task_num[Core_ID] < MAX_TASK_NUM) {
                                vis_bitmap_now[Core_ID].v[pull_first_active_vertex_index] = 1;
                            }
                        }
                    }
                }
                else {
                    vis_bitmap_now[Core_ID] = vis_bitmap[Core_ID][0];
                    vis_bitmap_index[Core_ID] = 0;
                }
            }
        }
    }

    ///Receive active vertex from backend cores, and update the active queue & bitmap
    if (rst_root) {
        /*initialize active_vid_queue*/
        while (!active_vid_queue[now_buffer_id][Core_ID].empty()) active_vid_queue[now_buffer_id][Core_ID].pop();
        init_bitmap_id[Core_ID] = 0;
    }
    else {
        ///first model is push
        // CHECK 加一个初始化标志似乎更好，就不需要保证该队列的初始化只比整体的初始化少一个clk（避免多余push ROOT_ID操作)，只需保证该队列最先完成初始化即可
        if (now_buffer_empty[Core_ID] && root_core_id == Core_ID && now_push_flag && rst_rd) {
            active_vid_queue[now_buffer_id][Core_ID].push(ROOT_ID);
        }
        ///initialize one bitmap line in each cycle.
        if (init_bitmap_id[Core_ID] < BitMap_Compressed_NUM && !rst_rd) {
            //initialize every element of the bitmap line selected in each cycle

            for (int i = 0; i < BitMap_Compressed_Length; ++ i) {
                if ((root_core_id == Core_ID && root_bitmap_id1 == init_bitmap_id[Core_ID] && root_bitmap_id2 == i) ||
                        Init_Bitmap[Core_ID][init_bitmap_id[Core_ID]].v[i]){
                    vis_bitmap[Core_ID][init_bitmap_id[Core_ID]].v[i] = 1;
                }
                else {
                    vis_bitmap[Core_ID][init_bitmap_id[Core_ID]].v[i] = 0;
                }
            }
            init_bitmap_id[Core_ID] ++;
        }
        //update the active queue & bitmap
        else if (!rst_rd) {
            if (Backend_Active_V_DValid && Backend_Active_V_Updated) {
                // do not filter visited vertex in push mode
                if (next_push_flag) {
                    if (Core_ID == 1) {
                        printf("insert v_id=%d\n", Backend_Active_V_ID);
                    }
                    active_vid_queue[next_buffer_id][Core_ID].push(Backend_Active_V_ID);
                }

                vis_bitmap[Core_ID][backend_v_bitmap_id1].v[backend_v_bitmap_id2] = 1;
                #if (DEBUG && PRINT_CONS)
                    if (Core_ID == PRINT_ID % CORE_NUM && Backend_Active_V_ID == PRINT_ID) {
                        printf("v_id=%d, active_bitmap[%d][%d]=1\n", Backend_Active_V_ID, backend_v_bitmap_id1, backend_v_bitmap_id2);
                    }
                #endif
            }
        }
    }

    if (rst_rd) {
        iteration_flag[Core_ID] = 1;
    }
    else {
        if(Backend_Iteration_End && Backend_Iteration_End_DValid){
            iteration_flag[Core_ID] = 0;
        }
        else{
            iteration_flag[Core_ID] = 1;
        }
    }

    // assign
    *Global_Iteration_ID = iteration_id[Core_ID];
}
