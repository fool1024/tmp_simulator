#ifndef debug_h
#define debug_h

#include "Accelerator.h"

using namespace std;

class debug_RD_ACTIVE_VERTEX_SINGLE {
public:
    int *_iteration_id;
    int *_init_bitmap_id;
    int *_iteration_end;

    queue<int> (*_active_vid_queue)[CORE_NUM];

    bitmap (*_vis_bitmap)[BitMap_Compressed_NUM + 1];
    int *_vis_bitmap_index;
    bitmap *_vis_bitmap_now;
};

class debug_RD_ACTIVE_VERTEX_OFFSET {
public:
    queue<int> *_push_flag_buffer;
    queue<int> *_v_id_buffer;
    queue<int> *_v_value_buffer;
    queue<int> *_pull_first_flag_buffer;
};

class debug_RD_Offset_Uram {
};

class debug_RD_Active_Vertex_Edge {
public:
    queue<int> *_push_flag_buffer;
    queue<int> *_v_id_buffer;
    queue<int> *_v_value_buffer;
    queue<int> *_v_loffset_buffer;
    queue<int> *_v_roffset_buffer;
    queue<int> *_pull_first_flag_buffer;

    int *_now_loffest;
    int *_init_flag;
};

class debug_Rd_First_Edge_Bram {
public:
    queue<int> *_push_flag_buffer;
    queue<int> *_v_id_buffer;
    queue<int> *_v_value_buffer;
    queue<int> *_edge_buffer;
};

class debug_Generate_HBM_Edge_Rqst {
public:
    queue<int> (*_edge_addr_buffer)[GROUP_CORE_NUM];
    queue<BitVector_16> (*_edge_mask_buffer)[GROUP_CORE_NUM];
    queue<int> (*_push_flag_buffer)[GROUP_CORE_NUM];
    queue<int> (*_v_id_buffer)[GROUP_CORE_NUM];
    queue<int> (*_v_value_buffer)[GROUP_CORE_NUM];
};

class debug_HBM_Interface {
public:
    queue<int> *_edge_addr_buffer;
    queue<Cacheline_16> *_edge_buffer;
};

class debug_Schedule {
public:
    queue<int> *_push_flag_buffer1;
    queue<int> *_v_id_buffer1;
    queue<int> *_v_value_buffer1;
    queue<int> *_edge_buffer1;
    queue<int> *_push_flag_buffer2;
    queue<int> *_v_id_buffer2;
    queue<int> *_v_value_buffer2;
    queue<int> *_edge_buffer2;
};

class debug_Omega_Network {
public:
    queue<int> (*_om1_push_flag_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_id_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_value_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_push_flag_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_id_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_value_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_push_flag_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_id_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_value_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_push_flag_buffer_in2_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_id_buffer_in2_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om1_value_buffer_in2_out2)[OMEGA_SWITCH_NUM];

    queue<int> (*_om2_push_flag_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_id_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_value_buffer_in1_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_push_flag_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_id_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_value_buffer_in1_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_push_flag_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_id_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_value_buffer_in2_out1)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_push_flag_buffer_in2_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_id_buffer_in2_out2)[OMEGA_SWITCH_NUM];
    queue<int> (*_om2_value_buffer_in2_out2)[OMEGA_SWITCH_NUM];
};

class debug_Backend_Core_Process {
public:
    queue<int> *_update_v_id_buffer;
    queue<int> *_pull_first_flag_buffer;
};

class debug_Vertex_BRAM {
public:
    queue<int> *_push_flag_buffer;
    queue<int> *_addr_buffer;
    queue<int> *_value_buffer;
    queue<int> *_pull_first_flag_buffer;
};

void write_array_to_file (string prefix, FILE* fp, int array[], int size);
void write_bitvector_to_file (string prefix, FILE* fp, BitVector_16 array[], int size);

#endif