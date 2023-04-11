#ifndef PARA_H
#include "para.h"
#endif
#include <cstdio>


struct Powerlaw_Vid_Set {
    int v[POWER_LAW_V_NUM];
    void set(Powerlaw_Vid_Set& pvs) {
        for (int i = 0; i < POWER_LAW_V_NUM; i++) {
            v[i] = pvs.v[i];
        }
    }
};
struct Powerlaw_Vvisit_Set {
    bool visit[POWER_LAW_V_NUM];
    Powerlaw_Vvisit_Set() {
        for (int i = 0; i < POWER_LAW_V_NUM; i++) {
            visit[i] = false;
        }
    }
    void set(Powerlaw_Vvisit_Set& pvs) {
        for (int i = 0; i < POWER_LAW_V_NUM; i++) {
            visit[i] = pvs.visit[i];
        }
    }
};