#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


double get_uptime() {
    FILE *fp = fopen(PROC_ROOT "/uptime", "r");
    if (!fp) {
        perror("uptime info");
        exit(1);
    }
    double uptime;
    fscanf(fp, "%lf", &uptime);
    fclose(fp);
    return uptime;
}


SystemInfo get_system_info() {
    SystemInfo info;
    FILE *fp = fopen(PROC_ROOT "/stat", "r");
    char item[25];
    while(!feof(fp) && !ferror(fp)) {
        fscanf(fp, "%s ", item);
        if (!strcmp(item, "procs_running")) {
            fscanf(fp, "%u", &(info.num_running));
        }
        fscanf(fp, "%*[^\n]");
    }
    fclose(fp);
    info.load_average = get_load_average();
    info.cpus = get_cpu_info();
    info.processes = get_all_processes(PROC_ROOT);
    info.uptime = get_uptime();
    info.num_processes = info.processes.size();
    info.num_user_threads = 0;
    info.num_kernel_threads = 0;
    for (ProcessInfo proc: info.processes) {
        for (ProcessInfo th: proc.threads) {
            if (th.is_user_thread()) info.num_user_threads++;
            else if (th.is_kernel_thread()) info.num_kernel_threads++;
        }
    }
    info.num_threads = info.num_user_threads + info.num_kernel_threads;
    return info;
}
