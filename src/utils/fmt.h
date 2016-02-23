#pragma once
#include "../info/system_info.h"
#include "../info/load_average_info.h"

char * fmt_uptime_info(double uptime);
char * fmt_loadavg_info(LoadAverageInfo la);
char * fmt_cpuinfo_info(SystemInfo sys, SystemInfo sys_last, size_t cpu_no);
char * fmt_proc_info(SystemInfo sys);
char * fmt_thread_info(SystemInfo sys);
char * fmt_mem_info();
char * fmt_tbl_header();
char * fmt_tbl_item(ProcessInfo proc);
char * fmt_bytes(unsigned long long amount);
char * fmt_time(int seconds);
