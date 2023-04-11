// #include "Accelerator.h"
#ifndef PARA_H
#include "para.h"
#endif
#include <cstdio>
#include <cstdlib>
extern int vertex_updated;
struct BRAM{
    int* bram[CORE_NUM];
    int len;
    BRAM(int length){
        len = length;
        // printf("len:%d\n",len);
        for(int i = 0 ; i<CORE_NUM ; i++){
            bram[i] = new int[len];
        }
    }
    void BRAM_IP(int rst_rd,
                int Tmp_rd_addr[], int Tmp_rd_addr_valid[],
                int Tmp_wr_addr[], int tmp_wr_data[], int Tmp_wr_addr_valid[],

                int *Tmp_bram_data, int *Tmp_bram_data_valid) {
        for (int i = 0; i < CORE_NUM; ++ i) {
            if (rst_rd) {
                Tmp_bram_data[i] = 0;
                Tmp_bram_data_valid[i] = 0;
            }
            else {
                if (Tmp_rd_addr_valid[i]) {
                    if (Tmp_rd_addr[i] == -2) {
                        Tmp_bram_data[i] = -2;
                    } else {
                        Tmp_bram_data[i] = bram[i][Tmp_rd_addr[i]];
                    }
                    Tmp_bram_data_valid[i] = 1;
                } else {
                    Tmp_bram_data[i] = 0;
                    Tmp_bram_data_valid[i] = 0;
                }

                if (Tmp_wr_addr_valid[i]) {
                    #if DEBUG
                        //last_update_cycle = clk;
                    #endif
                    #if (DEBUG && PRINT_CONS)
                        if (Tmp_wr_addr[i] * CORE_NUM + Core_ID == PRINT_ID) {
                            printf("clk=%d, update vertex bram: id=%d, value=%d\n", clk, PRINT_ID, tmp_wr_data);
                        }
                    #endif
                    if (tmp_wr_data[i] == -1) {
                        printf("catch -1\n");
                        exit (-1);
                    }
                    if(bram[i][Tmp_wr_addr[i]] == -1 || tmp_wr_data[i] < bram[i][Tmp_wr_addr[i]]) {
                        vertex_updated++;
                    }
                    bram[i][Tmp_wr_addr[i]] = tmp_wr_data[i];
                }
            }
        }
    }

    ~BRAM(){
        for(int i = 0 ; i<CORE_NUM ; i++){
            delete bram[i];
        }
    }
};

struct URAM{
    int uram[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1];

    URAM(){

    }
    void URAM_IP(int rst_rd,int Core_ID,
                int Tmp_rd_addr, int Tmp_rd_addr_valid,
                int Tmp_wr_addr, int tmp_wr_data, int Tmp_wr_addr_valid,

                int *Tmp_bram_data, int *Tmp_bram_data_valid) {
        if (rst_rd) {
            *Tmp_bram_data = 0;
            *Tmp_bram_data_valid = 0;
        }
        else {
            if (Tmp_rd_addr_valid) {
                if (Tmp_rd_addr == -2) {
                    *Tmp_bram_data = -2;
                } else {
                    *Tmp_bram_data = uram[Core_ID][Tmp_rd_addr];
                }
                *Tmp_bram_data_valid = 1;
            } else {
                *Tmp_bram_data = 0;
                *Tmp_bram_data_valid = 0;
            }

            if (Tmp_wr_addr_valid) {
                #if DEBUG
                    //last_update_cycle = clk;
                #endif
                #if (DEBUG && PRINT_CONS)
                    if (Tmp_wr_addr * CORE_NUM + Core_ID == PRINT_ID) {
                        printf("clk=%d, update vertex bram: id=%d, value=%d\n", clk, PRINT_ID, tmp_wr_data);
                    }
                #endif
                if (tmp_wr_data == -1) {
                    printf("catch -1\n");
                    exit (-1);
                }
                uram[Core_ID][Tmp_wr_addr] = tmp_wr_data;
            }
        }
    }
};

typedef struct Cacheline_16{
    int data[16];
    Cacheline_16() {}
    Cacheline_16(int f) {
        for (int i = 0; i < 16; i++) {
            data[i] = f;
        }
    }
    bool compare(const Cacheline_16 &b) {
        bool res = true;
        for (int i = 0; i < 16; i++) {
            if (data[i] != b.data[i]) res = false;
        }
        return res;
    }
    void show() {
        printf("mask:");
        for (int i = 0; i < 16; i++) {
            printf(" %d", data[i]);
        }
        printf("\n");
    }
}Cacheline_16;

typedef struct bitmap {
    bool v[BitMap_Compressed_Length];
    bitmap() {}
    bitmap(bool f) {
        for (int i = 0; i < BitMap_Compressed_Length; i++) {
            v[i] = f;
        }
    }
    bitmap(const bitmap& b) {
        for (int i = 0; i < BitMap_Compressed_Length; i++) {
            v[i] = b.v[i];
        }
    }
    bool compare(const bitmap& b) {
        bool res = true;
        for (int i = 0; i < BitMap_Compressed_Length; i++) {
            if (v[i] != b.v[i]) res = false;
        }
        return res;
    }
    inline void set(bool f) {
        for (int i = 0; i < BitMap_Compressed_Length; i++) {
            v[i] = f;
        }
    }
    void show() {
        printf("bitmap:");
        for (int i = 0; i < BitMap_Compressed_Length; i++) {
            printf(" %d", v[i] == true);
        }
        printf("\n");
    }
}bitmap;

typedef struct BitVector_16{
    bool flag[16];
    BitVector_16() {}
    BitVector_16(bool f) {
        for (int i = 0; i < 16; i++) {
            flag[i] = f;
        }
    }
    bool compare(const BitVector_16 &b) {
        bool res = true;
        for (int i = 0; i < 16; i++) {
            if (flag[i] != b.flag[i]) res = false;
        }
        return res;
    }
    void show() {
        printf("mask:");
        for (int i = 0; i < 16; i++) {
            printf(" %d", flag[i]);
        }
        printf("\n");
    }
}BitVector_16;
