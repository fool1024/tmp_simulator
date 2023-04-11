#define main_part
#include "Accelerator.h"

#include <bits/stdc++.h>

using namespace std;

//Predefined Value
int MAX_CYCLE = 30000000;

//Global Parameter
int clk;
int rst_rd;
int rst_root;
int ROOT_ID = 380019;
int vertex_updated = 0;
int MAX_ITERATION = 10;
int M2_max_buffer_size;
int iteration_end_flag = 0;
int Offset_URAM[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1][2]; //22 bits * 2
vector<int> Edge_MEM[PSEUDO_CHANNEL_NUM];
int VERTEX_DEGREE[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1]; //仅用于预处理
bitmap Init_Bitmap[CORE_NUM][BitMap_Compressed_NUM + 1];

int Csr_Offset[MAX_VERTEX_NUM + 1];
vector<int> Ori_Edge_MEM;
int VTX_NUM, EDGE_NUM;

// Pipe pipe[K];




#if DEBUG
int _main(char *off_file, char *edge_file, int root_id, int max_clk)
#else
int main(int argc, char **argv)
#endif
{
    #if DEBUG
        ROOT_ID = root_id;
        MAX_CYCLE = max_clk;
    #endif

    #if (DEBUG)
        Initialize_Input_Graph(off_file, edge_file);
    #else
        Initialize_Input_Graph(argv[1], argv[2]);
    #endif
    Initialize_Offset_Uram(VTX_NUM);
    Initialize_Vertex_bram(ROOT_ID);
    Initialize_Edge_bram(VTX_NUM);
    Initialize_VERTEX_DEGREE(VTX_NUM);
    Initialize_Bitmap(ROOT_ID,VTX_NUM);

    for (clk = 0; clk < MAX_CYCLE; ++ clk) {
        // rst_rd must be reset only one clk after the reseting of rst_root
        rst_root = (clk < 99) ? 1 : 0;
        rst_rd = (clk < 100) ? 1 : 0;

        if(clk % 500 == 0) {
            int local_print_id = PRINT_ID % CORE_NUM;
            // cout << "clk=" << clk << ", updated=" << vertex_updated << ", " << endl;
            printf("clk=%d, updated=%d\n",
                clk, vertex_updated);//, 0, P2_Stage_Full_wr[local_print_id], 0, P4_Stage_Full_wr[local_print_id], P5_Stage_Full_wr[local_print_id], P5_2_Stage_Full_wr[local_print_id], HBM_Interface_Full_wr[local_print_id], P6_Stage_Full1_wr[local_print_id], P6_Stage_Full2_wr[local_print_id], Om1_Stage_Full_wr[local_print_id], Om2_Stage_Full_wr[local_print_id], P7_Source_Core_Full_wr[local_print_id]);
         }
        
        #if DEBUG
            if (!rst_rd && iteration_end_flag) {
                printf("M2_max_buffer_size=%d\n", M2_max_buffer_size);
                return 0;
            }
        #endif
        Global(&pipe_mg);
        Compute_Core(&pipe_mg);
        Noc(&pipe_mg);
        Mem(&pipe_mg);
        pipe_mg.run();
    }

    return 0;
}


