#include "cpu_info.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

vector<CpuInfo> get_cpu_info() {
    FILE *fp = fopen(PROC_ROOT "/stat", "r");
    if (!fp) {
        perror("get_cpu_info");
        exit(1);
    }
    vector<CpuInfo> info;
    while (!feof(fp) && !ferror(fp)) {
        char name[10];
        fscanf(fp, "%9[^ ] ", name);
        if (name[0] != 'c' || name[1] != 'p' || name[2] != 'u')
            break;
        info.push_back(CpuInfo());
        unsigned long long *p = (unsigned long long *)&info.back();
        for (int i = 0; i < 10; i++) {
            fscanf(fp, "%llu ", p);
            p++;
        }
    }
    fclose(fp);
    return info;
}
