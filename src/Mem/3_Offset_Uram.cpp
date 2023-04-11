#include "Accelerator.h"
#if DEBUG
    #include "debug.h"
    debug_RD_Offset_Uram debug_M3;
#endif

extern int clk;
extern int rst_rd;
extern int Offset_URAM[CORE_NUM][MAX_VERTEX_NUM / CORE_NUM + 1][2];

using namespace std;

void Offset_Uram_Single(int Core_ID,
                        int Front_RD_Active_V_Offset_Addr, int Front_Active_V_DValid,

                        int *Uram_LOffset, int *Uram_ROffset, int *Uram_DValid);

void RD_Offset_Uram(int Front_RD_Active_V_Offset_Addr[], int Front_Active_V_DValid[],

                    int *Active_V_LOffset, int *Active_V_ROffset,int *Active_V_DValid) {

    for (int i = 0; i < CORE_NUM; ++ i) {
        Offset_Uram_Single(i,
                           Front_RD_Active_V_Offset_Addr[i], Front_Active_V_DValid[i],

                           &Active_V_LOffset[i], &Active_V_ROffset[i], &Active_V_DValid[i]);
    }
}


void Offset_Uram_Single(int Core_ID,
                        int Front_RD_Active_V_Offset_Addr, int Front_Active_V_DValid,

                        int *Uram_LOffset, int *Uram_ROffset, int *Uram_DValid) {
    #if DEBUG
        // no debug signal
    #endif
    if (rst_rd) {
        *Uram_LOffset = 0;
        *Uram_ROffset = 0;
        *Uram_DValid = 0;
    }
    else {
        if (Front_Active_V_DValid) {
            *Uram_LOffset = Offset_URAM[Core_ID][Front_RD_Active_V_Offset_Addr][0];
            *Uram_ROffset = Offset_URAM[Core_ID][Front_RD_Active_V_Offset_Addr][1];
            *Uram_DValid = 1;
            #if (DEBUG && PRINT_CONS)
                if (Core_ID == (PRINT_ID % CORE_NUM) && Front_RD_Active_V_Offset_Addr == (PRINT_ID / CORE_NUM) && Front_Active_V_DValid == 1) {
                    printf("clk=%d, off_addr=%d, loff=%d, roff=%d, off_valid=%d\n", clk, Front_RD_Active_V_Offset_Addr, *Uram_LOffset, *Uram_ROffset, *Uram_DValid);
                    // printf("core_id=%d, front_addr=%d, loff=%d, roff=%d\n", Core_ID, Front_RD_Active_V_Offset_Addr, Offset_URAM[0][0][0], Offset_URAM[0][0][1]);
                }
            #endif
        }
        else {
            *Uram_LOffset = 0;
            *Uram_ROffset = 0;
            *Uram_DValid = 0;
        }
    }
}
