#include "debug.h"

using namespace std;

void write_array_to_file (string prefix, FILE* fp, int array[], int size) {
    fprintf(fp, "%s", prefix.c_str());
    for (int i = 0; i < size; i++) {
        fprintf(fp, " %d", array[i]);
    }
    fprintf(fp, "\n");
}

void write_bitvector_to_file (string prefix, FILE* fp, BitVector_16 array[], int size) {
    fprintf(fp, "%s ", prefix.c_str());
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 16; j++) {
            fprintf(fp, "%d", array[i].flag[j] == true);
        }
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
}