#include "load_average_info.h"
#include <stdio.h>
#include <stdlib.h>

LoadAverageInfo get_load_average() {
    FILE *fp = fopen(PROC_ROOT "/loadavg", "r");
    if (!fp) {
        fprintf(stderr, "file error\n");
        exit(1);
    }
    LoadAverageInfo la;
    fscanf(fp, "%lf %lf %lf", &la.one_min, &la.five_mins, &la.fifteen_mins);
    return la;
}
