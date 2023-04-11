#include"RAM.h"
struct PipeLine{

    struct BRAM* bram;
    bool bram_exist;

    void init(){
        bram_exist = false;
        bram = nullptr;
    }

    void single_run(){}

    void run(){
        //process the input

        //single node run
        for(int i = 0; i < CORE_NUM; ++ i){
            single_run();
        }
    }

    void visit_bram(int Core_ID,
                    int Tmp_rd_addr, int Tmp_rd_addr_valid,
                    int Tmp_wr_addr, int tmp_wr_data, int Tmp_wr_addr_valid,

                    int *Tmp_bram_data, int *Tmp_bram_data_valid){
        if(bram_exist){
            bram->BRAM_IP(Core_ID,
                        Tmp_rd_addr, Tmp_rd_addr_valid,
                        Tmp_wr_addr, tmp_wr_data, Tmp_wr_addr_valid,
                        Tmp_bram_data, Tmp_bram_data_valid);
        }
    }

    void connect(struct PipeLine& PL){
        
    }
};

struct Read_Active_Vertex_Offset{
    struct read_active_vertex_offset_input read_active_vertex_offset_input_reg;
    struct read_active_vertex_offset_output read_active_vertex_offset_output_reg;

    void single_run(){

    }

    void run(){
        //process the input

        //single node run
        for(int i = 0; i < CORE_NUM; ++ i){
            single_run();
        }
    }

};
