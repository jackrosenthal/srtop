#include "load_average_info.h"
#include <stdio.h>
#include <stdlib.h>

LoadAverageInfo get_load_average() {
    FILE *fp = fopen(PROC_ROOT "/loadavg", "r");
    if (!fp) {
        perror(PROC_ROOT "/loadavg file error");
        exit(1);
    }
    LoadAverageInfo la;
    fscanf(fp, "%lf %lf %lf", &(la.one_min), &(la.five_mins), &(la.fifteen_mins));
    fclose(fp);
    return la;
}
