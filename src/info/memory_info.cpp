#include "memory_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>

MemoryInfo get_memory_info() {
    FILE *fp = fopen(PROC_ROOT "/meminfo", "r");
    if (!fp) {
        perror("get_memory_info");
        exit(1);
    }
    std::unordered_map<std::string, unsigned long long> mi_map;
    char item[17];
    unsigned long long item_val;
    while (!ferror(fp) && !feof(fp)) {
        fscanf(fp, "%16[^:]: %llu%*[^\n]\n", item, &item_val);
        mi_map[std::string(item)] = item_val;
    }
    fclose(fp);
    return (MemoryInfo){
        .total_memory = mi_map["MemTotal"],
        .free_memory = mi_map["MemFree"],
        .buffers_memory = mi_map["Buffers"],
        .cached_memory = mi_map["Cached"],
        .total_swap = mi_map["SwapTotal"],
        .free_swap = mi_map["SwapFree"]
    };
}
