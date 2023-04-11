#ifndef ACC_H

#include <bits/stdc++.h>

const int MAX_CLK = 100000000;

const int MAX_VERTEX_NUM = 1024 * 1024 * 2;
const int MAX_EDGE_NUM = 1024 * 1024 * 32;

const int CACHELINE_LEN = 16;

const int CORE_NUM = 512;

const int PSEUDO_CHANNEL_NUM = 32;
const int GROUP_CORE_NUM = CORE_NUM / PSEUDO_CHANNEL_NUM;
const int MAX_EDGE_ADDR = MAX_VERTEX_NUM * 32 / PSEUDO_CHANNEL_NUM;

const int OMEGA_DEPTH = 9;
const int OMEGA_SWITCH_NUM = CORE_NUM / 2;

typedef struct BitVector_16{
    bool flag[16];
}BitVector_16;

typedef struct Cacheline_16{
    int data[16];
}Cacheline_16;

#endif // ACC_H
