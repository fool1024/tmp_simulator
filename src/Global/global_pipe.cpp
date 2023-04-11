
extern int clk;
extern int rst_rd;
#include "Accelerator.h"

void Read_Active_Vertex_Offset_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                            int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}

void RD_Active_Vertex_Edge_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                            int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}

void Generate_HBM_Edge_Rqst_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                         int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}

void Schedule_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                           int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}


void Backend_Core_Global_Signal(int Front_Global_Iteration_ID[], Powerlaw_Vid_Set Front_Global_Powerlaw_V_ID[], Powerlaw_Vvisit_Set Front_Global_Powerlaw_V_Visit[],

                                int *Global_Iteration_ID, Powerlaw_Vid_Set *Global_Powerlaw_V_ID, Powerlaw_Vvisit_Set *Global_Powerlaw_V_Visit) {
    static int local_Global_Iteration_ID[CORE_NUM];
    static Powerlaw_Vid_Set local_Global_Powerlaw_V_ID[CORE_NUM];
    static Powerlaw_Vvisit_Set local_Global_Powerlaw_V_Visit[CORE_NUM];

    for (int i = 0; i < CORE_NUM; i++) {
        if (rst_rd) {
            Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            Global_Iteration_ID[i] = local_Global_Iteration_ID[i];
            Global_Powerlaw_V_ID[i].set(local_Global_Powerlaw_V_ID[i]);
            Global_Powerlaw_V_Visit[i].set(local_Global_Powerlaw_V_Visit[i]);
        }

        if (rst_rd) {
            local_Global_Iteration_ID[i] = 0;
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].v[j] = VERTEX_NOT_READ[j];
                local_Global_Powerlaw_V_Visit[i].visit[j] = false;
            }
        } else {
            local_Global_Iteration_ID[i] = Front_Global_Iteration_ID[i];
            for (int j = 0; j < POWER_LAW_V_NUM; j++) {
                local_Global_Powerlaw_V_ID[i].set(Front_Global_Powerlaw_V_ID[i]);
                local_Global_Powerlaw_V_Visit[i].set(Front_Global_Powerlaw_V_Visit[i]);
            }
        }
    }
}

