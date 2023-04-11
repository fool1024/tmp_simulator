#include "Accelerator.h"
// #include "MEM.h"
#define STORE_DATA 0

using namespace std;

extern int Offset_URAM[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1][2];
extern vector<int> Edge_MEM[PSEUDO_CHANNEL_NUM];
// extern int Edge_MEM[PSEUDO_CHANNEL_NUM][MAX_EDGE_ADDR];
extern BRAM vtx_bram;
extern BRAM edge_bram;
extern int Csr_Offset[MAX_VERTEX_NUM + 1];
extern vector<int> Ori_Edge_MEM;
extern int VTX_NUM;
extern int EDGE_NUM;
extern int VERTEX_DEGREE[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1];
extern bitmap Init_Bitmap[CORE_NUM][BitMap_Compressed_NUM + 1];

vector<pair<int, int>> no_self_edge_vec;
vector<int> edge_sort_degree;

void Initialize_Input_Graph(char *off_file, char *edge_file) {
    ifstream in_off,in_list;
    in_off.open(off_file);
    in_list.open(edge_file);
    if(!in_off) {
       cout << "[ERROR] Failed to open " << off_file << endl;
       exit(-1);
    }
    if(!in_list) {
        cout << "[ERROR] Failed to open " << edge_file << endl;
        exit(-1);
    }
    VTX_NUM = -1;
    EDGE_NUM = 0;
    for (int i = 0; i < PSEUDO_CHANNEL_NUM; i++) {
        Edge_MEM[i].clear();
    }
    Ori_Edge_MEM.clear();
    edge_sort_degree.clear();
    no_self_edge_vec.clear();
    // delete self-edge
    int idx = 0;
    while(in_off >> Csr_Offset[idx]) {
        idx++;
        VTX_NUM++;
    }
    int delete_edge = 0;
    for (int vid = 0; vid < VTX_NUM; vid++) {
        for (int off = Csr_Offset[vid]; off < Csr_Offset[vid + 1]; off++) {
            int dst_id = 0;
            in_list >> dst_id;
            if (vid == dst_id) {
                delete_edge++;
                continue; // skip self-edge
            }
            no_self_edge_vec.push_back(pair<int, int>(vid, dst_id));
        }
    }
    cout << "delete_edge=" << delete_edge << endl;
    EDGE_NUM = no_self_edge_vec.size();

    int cu_vid = 0, loff = 0, roff = 0;
    for (int i = 0; i < EDGE_NUM; i++) {
        while (cu_vid != no_self_edge_vec[i].first) {
            Csr_Offset[cu_vid] = loff;
            loff = roff;
            cu_vid++;
            // cout << cu_vid << " " << no_self_edge_vec[i].first << endl;
            if (cu_vid > no_self_edge_vec[i].first) exit(-1);
        }
        Ori_Edge_MEM.push_back(no_self_edge_vec[i].second);
        roff++;
    }
    Csr_Offset[cu_vid] = loff;
    Csr_Offset[cu_vid + 1] = roff;
    auto compare = [](const int v1, const int v2) -> bool {
        return (Csr_Offset[v1 + 1] - Csr_Offset[v1]) > (Csr_Offset[v2 + 1] - Csr_Offset[v2]);
    };
    for (int i = 0; i < VTX_NUM; i++) {
        sort(&Ori_Edge_MEM[Csr_Offset[i]], &Ori_Edge_MEM[Csr_Offset[i + 1]], compare);
        for (int off = Csr_Offset[i]; off < Csr_Offset[i + 1]; off++) {
            Edge_MEM[(i % CORE_NUM) / GROUP_CORE_NUM].push_back(Ori_Edge_MEM[off]);
        }
    }
    #if STORE_DATA
        // write file
        const string prefix = "./edge_channel_";
        for (int i = 0; i < PSEUDO_CHANNEL_NUM; i++) {
            FILE* edge_fp;
            string cu_filename = prefix;
            cu_filename += to_string(i);
            cu_filename += ".txt";
            edge_fp = fopen(cu_filename.c_str(), "w");
            printf("channel_%d_size=%d\n", i, Edge_MEM[i].size());
            for (int j = 0; j < Edge_MEM[i].size(); j++) {
                fprintf(edge_fp, "%d\n", Edge_MEM[i][j]);
            }
            fclose(edge_fp);
        }
    #endif

    in_off.close();
    in_list.close();
    // fclose(fp);
    printf("Initialize Graph Complete\n");
}

void Initialize_Offset_Uram(int V_Num) {
    // for (int i = 0; i < V_Num; i++) {
    //     Offset_URAM[i % CORE_NUM][i / CORE_NUM][0] = Csr_Offset[i];
    //     Offset_URAM[i % CORE_NUM][i / CORE_NUM][1] = Csr_Offset[i + 1] - 1;
    // }
    for (int i = 0; i < V_Num; i++) {
        if (i % GROUP_CORE_NUM == 0 && i / CORE_NUM == 0) {
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][0] = 0;
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][1] = Csr_Offset[i + 1] - Csr_Offset[i];
        } else if (i % GROUP_CORE_NUM == 0) {
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][0] = Offset_URAM[(i - CORE_NUM + GROUP_CORE_NUM - 1) % CORE_NUM][(i - CORE_NUM + GROUP_CORE_NUM - 1) / CORE_NUM][1];
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][1] = Csr_Offset[i + 1] - Csr_Offset[i] + Offset_URAM[i % CORE_NUM][i / CORE_NUM][0];
        } else {
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][0] = Offset_URAM[(i - 1) % CORE_NUM][(i - 1) / CORE_NUM][1];
            Offset_URAM[i % CORE_NUM][i / CORE_NUM][1] = Csr_Offset[i + 1] - Csr_Offset[i] + Offset_URAM[i % CORE_NUM][i / CORE_NUM][0];
        }
    }
    printf("Initialize Offset Complete\n");
}

void Initialize_Vertex_bram(int root_id){
    for(int i = 0 ; i<CORE_NUM ; i++){
        for(int j = 0 ; j<vtx_bram.len ; j++){
            vtx_bram.bram[i][j] = -1;
        }
    }
    vtx_bram.bram[root_id%CORE_NUM][root_id/CORE_NUM] = 0;
    printf("Initialize Bram Complete\n");
}

void Initialize_Edge_bram(int V_Num) {
    for (int i = 0; i < V_Num; i++) {
        int Loff = Offset_URAM[i % CORE_NUM][i / CORE_NUM][0], Roff = Offset_URAM[i % CORE_NUM][i / CORE_NUM][1];
        if (Loff == Roff) {
            edge_bram.bram[i % CORE_NUM][i / CORE_NUM] = -1;
        } else {
            int max_edge = Edge_MEM[i % CORE_NUM / GROUP_CORE_NUM][Loff];
            int max_degree = VERTEX_DEGREE[max_edge % CORE_NUM][max_edge / CORE_NUM];
            for (int j = Loff; j < Roff; j++) {
                int edge = Edge_MEM[i % CORE_NUM / GROUP_CORE_NUM][j];
                if (VERTEX_DEGREE[edge % CORE_NUM][edge / CORE_NUM] > max_degree) {
                    max_edge = edge;
                    max_degree = VERTEX_DEGREE[edge % CORE_NUM][edge / CORE_NUM];
                }
            }
            edge_bram.bram[i % CORE_NUM][i / CORE_NUM] = max_edge;
        }
    }
    printf("\033[32m[INFO]\033[m Initialize Edge Bram Complete\n");
    return;
}

void Initialize_VERTEX_DEGREE(int V_Num) {
    for (int i = 0; i < V_Num; i++) {
        int Loff = Offset_URAM[i % CORE_NUM][i / CORE_NUM][0], Roff = Offset_URAM[i % CORE_NUM][i / CORE_NUM][1];
        VERTEX_DEGREE[i % CORE_NUM][i / CORE_NUM] = Roff - Loff;
    }
}

void Initialize_Bitmap(int root_id, int V_Num) {
    for (int i = 0; i < 1024 * 1024; i++) {
        if (i < V_Num && i != root_id && VERTEX_DEGREE[i % CORE_NUM][i / CORE_NUM]) {
            Init_Bitmap[i % CORE_NUM][(i / CORE_NUM) / BitMap_Compressed_Length].v[(i / CORE_NUM) % BitMap_Compressed_Length] = 0;
        } else {
            Init_Bitmap[i % CORE_NUM][(i / CORE_NUM) / BitMap_Compressed_Length].v[(i / CORE_NUM) % BitMap_Compressed_Length] = 1;
        }
    }
}