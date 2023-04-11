#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_RD_Active_Vertex_Edge debug_M4;
#endif

extern int clk;
extern int rst_rd;


using namespace std;

void RD_Active_Vertex_Edge_Single(int Core_ID,
                                  int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid,
                                  int Front_Iteration_End, int Front_Iteration_End_DValid,
                                  int Front_Active_V_LOffset, int Front_Active_V_Roffset, int Front_Offset_DValid,
                                  int CombineStage_Full, int BramStage_Full,

                                  int *Stage_Full,
                                  int *Rd_HBM_Edge_Addr, BitVector_16 *Rd_HBM_Edge_Mask, int *HBM_Push_Flag, int *HBM_Active_V_ID, int *HBM_Active_V_Value, int *Rd_HBM_Edge_Valid,
                                  int *Rd_BRAM_Edge_Addr, int *BRAM_Push_Flag, int *BRAM_Active_V_ID, int *BRAM_Active_V_Value, int *Rd_BRAM_Edge_Valid,
                                  int *Iteration_End, int *Iteration_End_DValid);

void RD_Active_Vertex_Edge(int Front_Push_Flag[], int Front_Active_V_ID[], int Front_Active_V_Value[], int Front_Active_V_Pull_First_Flag[], int Front_Active_V_DValid[], 
                           int Front_Iteration_End[], int Front_Iteration_End_DValid[],
                           int Front_Active_V_LOffset[], int Front_Active_V_Roffset[], int Front_Offset_DValid[], int CombineStage_Full[],int BramStage_Full[],

                           int *Stage_Full,
                           int *Rd_HBM_Edge_Addr, BitVector_16 *Rd_HBM_Edge_Mask, int *HBM_Push_Flag, int *HBM_Active_V_ID, int *HBM_Active_V_Value, int *Rd_HBM_Edge_Valid,
                           int *Rd_BRAM_Edge_Addr, int *BRAM_Push_Flag, int *BRAM_Active_V_ID, int *BRAM_Active_V_Value, int *Rd_BRAM_Edge_Valid,
                           int *Iteration_End, int *Iteration_End_DValid) {

    for (int i = 0; i < CORE_NUM; ++ i) {
        RD_Active_Vertex_Edge_Single(i,
                                     Front_Push_Flag[i], Front_Active_V_ID[i], Front_Active_V_Value[i], Front_Active_V_Pull_First_Flag[i], Front_Active_V_DValid[i],
                                     Front_Iteration_End[i], Front_Iteration_End_DValid[i],
                                     Front_Active_V_LOffset[i], Front_Active_V_Roffset[i], Front_Offset_DValid[i],
                                     CombineStage_Full[i], BramStage_Full[i],

                                     &Stage_Full[i],
                                     &Rd_HBM_Edge_Addr[i], &Rd_HBM_Edge_Mask[i], &HBM_Push_Flag[i], &HBM_Active_V_ID[i], &HBM_Active_V_Value[i], &Rd_HBM_Edge_Valid[i],
                                     &Rd_BRAM_Edge_Addr[i], &BRAM_Push_Flag[i], &BRAM_Active_V_ID[i], &BRAM_Active_V_Value[i], &Rd_BRAM_Edge_Valid[i],
                                     &Iteration_End[i], &Iteration_End_DValid[i]);
    }
}


void RD_Active_Vertex_Edge_Single(int Core_ID,
                                  int Front_Push_Flag, int Front_Active_V_ID, int Front_Active_V_Value, int Front_Active_V_Pull_First_Flag, int Front_Active_V_DValid,
                                  int Front_Iteration_End, int Front_Iteration_End_DValid,
                                  int Front_Active_V_LOffset, int Front_Active_V_Roffset, int Front_Offset_DValid,
                                  int CombineStage_Full, int BramStage_Full,

                                  int *Stage_Full,
                                  int *Rd_HBM_Edge_Addr, BitVector_16 *Rd_HBM_Edge_Mask, int *HBM_Push_Flag, int *HBM_Active_V_ID, int *HBM_Active_V_Value, int *Rd_HBM_Edge_Valid,
                                  int *Rd_BRAM_Edge_Addr, int *BRAM_Push_Flag, int *BRAM_Active_V_ID, int *BRAM_Active_V_Value, int *Rd_BRAM_Edge_Valid,
                                  int *Iteration_End, int *Iteration_End_DValid) {

    static queue<int> push_flag_buffer[CORE_NUM], v_id_buffer[CORE_NUM], v_value_buffer[CORE_NUM],
    pull_first_flag_buffer[CORE_NUM],v_loffset_buffer[CORE_NUM], v_roffset_buffer[CORE_NUM];
    static int v_buffer_empty[CORE_NUM];
    static int o_buffer_empty[CORE_NUM];

    static int now_loffset[CORE_NUM];

    static queue_container<int> v_offset_container[CORE_NUM],v_container[CORE_NUM],v_value_container[CORE_NUM];
    static value_container<int> HBM_V_container[CORE_NUM],HBM_Edge_container[CORE_NUM],BRAM_Edge_container[CORE_NUM],BRAM_V_container[CORE_NUM];

    if(clk == 0){
        if( Core_ID == 0 )
        for(int i = 0 ; i<CORE_NUM ; i++){
            v_offset_container[i].combine({&v_loffset_buffer[i],&v_roffset_buffer[i]});
            v_value_container[i].combine({&push_flag_buffer[i], &v_id_buffer[i], &v_value_buffer[i]});
            v_container[i].combine({&push_flag_buffer[i], &v_id_buffer[i], &v_value_buffer[i],&pull_first_flag_buffer[i]});
        }
        /// HBM_Push_Flag, HBM_Active_V_ID, HBM_Active_V_Value
        HBM_V_container[Core_ID].combine({HBM_Push_Flag, HBM_Active_V_ID, HBM_Active_V_Value});
        /*Rd_HBM_Edge_Addr,Rd_HBM_Edge_Valid*/
        HBM_Edge_container[Core_ID].combine({Rd_HBM_Edge_Addr,Rd_HBM_Edge_Valid});
        /*BRAM_Push_Flag, BRAM_Active_V_ID, BRAM_Active_V_Value*/
        BRAM_V_container[Core_ID].combine({BRAM_Push_Flag, BRAM_Active_V_ID, BRAM_Active_V_Value});
        /*Rd_BRAM_Edge_Addr,Rd_BRAM_Edge_Valid*/
        BRAM_Edge_container[Core_ID].combine({Rd_BRAM_Edge_Addr,Rd_BRAM_Edge_Valid});
    }

    #if DEBUG
        debug_M4._push_flag_buffer = push_flag_buffer;
        debug_M4._v_id_buffer = v_id_buffer;
        debug_M4._v_value_buffer = v_value_buffer;
        debug_M4._v_loffset_buffer = v_loffset_buffer;
        debug_M4._v_roffset_buffer = v_roffset_buffer;
        debug_M4._pull_first_flag_buffer = pull_first_flag_buffer;

        debug_M4._now_loffest = now_loffset;
    #endif

    v_buffer_empty[Core_ID] = (v_id_buffer[Core_ID].size() == 0);
    o_buffer_empty[Core_ID] = (v_loffset_buffer[Core_ID].size() == 0);

    if (rst_rd) {
        now_loffset[Core_ID] = 1000000000;

        for (int i = 0; i < CACHELINE_LEN; ++ i) Rd_HBM_Edge_Mask->flag[i] = 0;
        HBM_V_container[Core_ID].assign({0,0,0});
        HBM_Edge_container[Core_ID].assign({0,0});
        // *Rd_HBM_Edge_Addr = 0;
        // *HBM_Push_Flag = 0;
        // *HBM_Active_V_ID = 0;
        // *HBM_Active_V_Value = 0;
        // *Rd_HBM_Edge_Valid = 0;

        BRAM_V_container[Core_ID].assign({0,0,0});
        BRAM_Edge_container[Core_ID].assign({0,0});
        // *Rd_BRAM_Edge_Addr = 0;
        // *BRAM_Push_Flag = 0;
        // *BRAM_Active_V_ID = 0;
        // *BRAM_Active_V_Value = 0;
        // *Rd_BRAM_Edge_Valid = 0;
    }
    else {
        if (o_buffer_empty[Core_ID]) {
            now_loffset[Core_ID] = 1000000000;

            now_loffset[Core_ID] = 1000000000;
            for (int i = 0; i < CACHELINE_LEN; ++ i) Rd_HBM_Edge_Mask->flag[i] = 0;
            HBM_V_container[Core_ID].assign({0,0,0});
            HBM_Edge_container[Core_ID].assign({0,0});
            // *Rd_HBM_Edge_Addr = 0;
            // *HBM_Push_Flag = 0;
            // *HBM_Active_V_ID = 0;
            // *HBM_Active_V_Value = 0;
            // *Rd_HBM_Edge_Valid = 0;

            BRAM_V_container[Core_ID].assign({0,0,0});
            BRAM_Edge_container[Core_ID].assign({0,0});
            // *Rd_BRAM_Edge_Addr = 0;
            // *BRAM_Push_Flag = 0;
            // *BRAM_Active_V_ID = 0;
            // *BRAM_Active_V_Value = 0;
            // *Rd_BRAM_Edge_Valid = 0;
        }
        else {
            if (pull_first_flag_buffer[Core_ID].front()) {
                for (int i = 0; i < CACHELINE_LEN; ++ i) Rd_HBM_Edge_Mask->flag[i] = 0;
                HBM_V_container[Core_ID].assign({0,0,0});
                HBM_Edge_container[Core_ID].assign({0,0});
                // *Rd_HBM_Edge_Addr = 0;
                // *HBM_Push_Flag = 0;
                // *HBM_Active_V_ID = 0;
                // *HBM_Active_V_Value = 0;
                // *Rd_HBM_Edge_Valid = 0;

                if (BramStage_Full) {
                    BRAM_V_container[Core_ID].assign({0,0,0});
                    BRAM_Edge_container[Core_ID].assign({0,0});
                    // *Rd_BRAM_Edge_Addr = 0;
                    // *BRAM_Push_Flag = 0;
                    // *BRAM_Active_V_ID = 0;
                    // *BRAM_Active_V_Value = 0;
                    // *Rd_BRAM_Edge_Valid = 0;
                }
                else {
                    BRAM_V_container[Core_ID].assign(v_value_container[Core_ID]);
                    BRAM_Edge_container[Core_ID].assign({v_id_buffer[Core_ID].front() / CORE_NUM,1});
                    // *Rd_BRAM_Edge_Addr = v_id_buffer[Core_ID].front() / CORE_NUM;
                    // *BRAM_Push_Flag = push_flag_buffer[Core_ID].front();
                    // *BRAM_Active_V_ID = v_id_buffer[Core_ID].front();
                    // *BRAM_Active_V_Value = v_value_buffer[Core_ID].front();
                    // *Rd_BRAM_Edge_Valid = 1;
                    #if (DEBUG && PRINT_CONS)
                        if (*BRAM_Active_V_ID == PRINT_ID) {
                            printf("clk=%d, rd_bram_addr=%d, bram_push_flag=%d, bram_active_v_id=%d, bram_active_v_value=%d, rd_bram_valid=%d\n", clk, *Rd_BRAM_Edge_Addr, *BRAM_Push_Flag, *BRAM_Active_V_ID, *BRAM_Active_V_Value, *Rd_BRAM_Edge_Valid);
                        }
                    #endif

                    v_container[Core_ID].pop();
                    // push_flag_buffer[Core_ID].pop(); 
                    // v_id_buffer[Core_ID].pop(); 
                    // v_value_buffer[Core_ID].pop(); 
                    // pull_first_flag_buffer[Core_ID].pop(); 
                    v_offset_container[Core_ID].pop();
                    // v_loffset_buffer[Core_ID].pop(); 
                    // v_roffset_buffer[Core_ID].pop();
                }
            }
            else {
                if (CombineStage_Full) {
                    for (int i = 0; i < CACHELINE_LEN; ++ i) Rd_HBM_Edge_Mask->flag[i] = 0;
                    HBM_Edge_container[Core_ID].assign({0,0});
                    HBM_V_container[Core_ID].assign({0,0,0});
                    // *Rd_HBM_Edge_Addr = 0;
                    // *HBM_Push_Flag = 0;
                    // *HBM_Active_V_ID = 0;
                    // *HBM_Active_V_Value = 0;
                    // *Rd_HBM_Edge_Valid = 0;
                }
                else {
                    if (push_flag_buffer[Core_ID].front() == 0 && v_loffset_buffer[Core_ID].front() + 1 == v_roffset_buffer[Core_ID].front()) {
                        // pull mode, but not pull first task
                        for (int i = 0; i < CACHELINE_LEN; ++ i) Rd_HBM_Edge_Mask->flag[i] = 0;
                        HBM_Edge_container[Core_ID].assign({0,0});
                        HBM_V_container[Core_ID].assign({0,0,0});
                        // *Rd_HBM_Edge_Addr = 0;
                        // *HBM_Push_Flag = 0;
                        // *HBM_Active_V_ID = 0;
                        // *HBM_Active_V_Value = 0;
                        // *Rd_HBM_Edge_Valid = 0;
                        now_loffset[Core_ID] = 1000000000;

                        v_container[Core_ID].pop();
                        // push_flag_buffer[Core_ID].pop(); 
                        // v_id_buffer[Core_ID].pop(); 
                        // v_value_buffer[Core_ID].pop(); 
                        // pull_first_flag_buffer[Core_ID].pop(); 
                        v_offset_container[Core_ID].pop();
                        // v_loffset_buffer[Core_ID].pop(); 
                        // v_roffset_buffer[Core_ID].pop();
                    }
                    else if (now_loffset[Core_ID] / CACHELINE_LEN > v_roffset_buffer[Core_ID].front() / CACHELINE_LEN){
                        for (int i = 0; i < CACHELINE_LEN; ++ i) {
                            if (i < v_loffset_buffer[Core_ID].front() % CACHELINE_LEN || (i >= v_roffset_buffer[Core_ID].front() % CACHELINE_LEN && v_loffset_buffer[Core_ID].front() / CACHELINE_LEN == v_roffset_buffer[Core_ID].front() / CACHELINE_LEN)){
                                Rd_HBM_Edge_Mask->flag[i] = 0;
                            }
                            else {
                                Rd_HBM_Edge_Mask->flag[i] = 1;
                            }
                        }
                        HBM_Edge_container[Core_ID].assign({v_loffset_buffer[Core_ID].front() / CACHELINE_LEN,(v_loffset_buffer[Core_ID].front() < v_roffset_buffer[Core_ID].front())});
                        HBM_V_container[Core_ID].assign(v_value_container[Core_ID]);
                        // *Rd_HBM_Edge_Addr = v_loffset_buffer[Core_ID].front() / CACHELINE_LEN;
                        // *HBM_Push_Flag = push_flag_buffer[Core_ID].front();
                        // *HBM_Active_V_ID = v_id_buffer[Core_ID].front();
                        // *HBM_Active_V_Value = v_value_buffer[Core_ID].front();
                        // *Rd_HBM_Edge_Valid = (v_loffset_buffer[Core_ID].front() < v_roffset_buffer[Core_ID].front());

                        if (v_loffset_buffer[Core_ID].front() / CACHELINE_LEN == v_roffset_buffer[Core_ID].front() / CACHELINE_LEN) {
                            
                            v_container[Core_ID].pop();
                            // push_flag_buffer[Core_ID].pop(); 
                            // v_id_buffer[Core_ID].pop(); 
                            // v_value_buffer[Core_ID].pop(); 
                            // pull_first_flag_buffer[Core_ID].pop();
                            v_offset_container[Core_ID].pop(); 
                            // v_loffset_buffer[Core_ID].pop(); 
                            // v_roffset_buffer[Core_ID].pop();

                            //now_loffset = v_loffset_buffer[rd_cntr + 1];
                            if (v_loffset_buffer[Core_ID].size() >= 1) {
                                now_loffset[Core_ID] = v_loffset_buffer[Core_ID].front();
                            } else {
                                now_loffset[Core_ID] = 1000000000;
                            }
                        }
                        else {
                            now_loffset[Core_ID] = (v_loffset_buffer[Core_ID].front() / CACHELINE_LEN + 1) * CACHELINE_LEN;
                        }
                    }
                    else {
                        for (int i = 0; i < CACHELINE_LEN; ++ i) {
                            if (i < now_loffset[Core_ID] % CACHELINE_LEN || (i >= v_roffset_buffer[Core_ID].front() % CACHELINE_LEN && now_loffset[Core_ID] / CACHELINE_LEN == v_roffset_buffer[Core_ID].front() / CACHELINE_LEN)){
                                Rd_HBM_Edge_Mask->flag[i] = 0;
                            }
                            else {
                                Rd_HBM_Edge_Mask->flag[i] = 1;
                            }
                        }
                        HBM_V_container[Core_ID].assign(v_value_container[Core_ID]);
                        HBM_Edge_container[Core_ID].assign({now_loffset[Core_ID] / CACHELINE_LEN,(now_loffset[Core_ID] < v_roffset_buffer[Core_ID].front())});
                        // *Rd_HBM_Edge_Addr = now_loffset[Core_ID] / CACHELINE_LEN;
                        // *HBM_Push_Flag = push_flag_buffer[Core_ID].front();
                        // *HBM_Active_V_ID = v_id_buffer[Core_ID].front();
                        // *HBM_Active_V_Value = v_value_buffer[Core_ID].front();
                        // *Rd_HBM_Edge_Valid = (now_loffset[Core_ID] < v_roffset_buffer[Core_ID].front());

                        if (now_loffset[Core_ID] / CACHELINE_LEN == v_roffset_buffer[Core_ID].front() / CACHELINE_LEN) {
                            
                            v_container[Core_ID].pop();
                            // push_flag_buffer[Core_ID].pop(); 
                            // v_id_buffer[Core_ID].pop(); 
                            // v_value_buffer[Core_ID].pop(); 
                            // pull_first_flag_buffer[Core_ID].pop(); 
                            v_offset_container[Core_ID].pop();
                            // v_loffset_buffer[Core_ID].pop(); 
                            // v_roffset_buffer[Core_ID].pop();
                            //now_loffset = v_loffset_buffer[rd_cntr + 1];
                            if (v_loffset_buffer[Core_ID].size() >= 1) {
                                now_loffset[Core_ID] = v_loffset_buffer[Core_ID].front();
                            } else {
                                now_loffset[Core_ID] = 1000000000;
                            }
                        }
                        else {
                            now_loffset[Core_ID] = (now_loffset[Core_ID] / CACHELINE_LEN + 1) * CACHELINE_LEN;
                        }
                    }
                }

                BRAM_Edge_container[Core_ID].assign({0,0});
                BRAM_V_container[Core_ID].assign({0,0,0});
                // *Rd_BRAM_Edge_Addr = 0;
                // *BRAM_Push_Flag = 0;
                // *BRAM_Active_V_ID = 0;
                // *BRAM_Active_V_Value = 0;
                // *Rd_BRAM_Edge_Valid = 0;
            }
        }
    }

    if (!rst_rd && Front_Iteration_End && Front_Iteration_End_DValid && v_buffer_empty[Core_ID] && !Front_Offset_DValid) {
        *Iteration_End = 1;
        *Iteration_End_DValid = 1;
    }
    else {
        *Iteration_End = 0;
        *Iteration_End_DValid = 0;
    }

    if (rst_rd) {
        /*initialize push_flag, v_id, v_value, pull_first_flag buffers*/
        while (!push_flag_buffer[Core_ID].empty())        push_flag_buffer[Core_ID].pop();
        while (!v_id_buffer[Core_ID].empty())             v_id_buffer[Core_ID].pop();
        while (!v_value_buffer[Core_ID].empty())          v_value_buffer[Core_ID].pop();
        while (!pull_first_flag_buffer[Core_ID].empty())  pull_first_flag_buffer[Core_ID].pop();
    }
    else {
        if (Front_Active_V_DValid) { 
            v_container[Core_ID].push({Front_Push_Flag,Front_Active_V_ID,Front_Active_V_Value,Front_Active_V_Pull_First_Flag});           
            // push_flag_buffer[Core_ID].push(Front_Push_Flag); 
            // v_id_buffer[Core_ID].push(Front_Active_V_ID); 
            // v_value_buffer[Core_ID].push(Front_Active_V_Value); 
            // pull_first_flag_buffer[Core_ID].push(Front_Active_V_Pull_First_Flag);
        }
    }

    if (rst_rd) {
        /*initialize v_loffset, v_roffset buffers*/
        while (!v_loffset_buffer[Core_ID].empty())    v_loffset_buffer[Core_ID].pop();
        while (!v_roffset_buffer[Core_ID].empty())    v_roffset_buffer[Core_ID].pop();
    }
    else {
        if (Front_Offset_DValid) {
            v_offset_container[Core_ID].push({Front_Active_V_LOffset,Front_Active_V_Roffset});
            // v_loffset_buffer[Core_ID].push(Front_Active_V_LOffset);
            // v_roffset_buffer[Core_ID].push(Front_Active_V_Roffset);
        }
    }

    //assign stage_full = v_buffer_full
    *Stage_Full = (v_id_buffer[Core_ID].size() >= FIFO_SIZE);
}
