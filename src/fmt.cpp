/**
 * Shadow Recruit top
 * Formatting utilities
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fmt.h"
#include "system_info.h"
#include "load_average_info.h"
#include "process_info.h"
#include "memory_info.h"

char * fmt_uptime_info(double uptime) {
    static char uptime_buffer[80];
    int seconds = (int)uptime % 60;
    int minutes = ((int)(uptime/60)) % 60;
    int hours = ((int)(uptime/3600)) % 24;
    int days = uptime/86400;
    sprintf(uptime_buffer, "up %d day%s, %02d:%02d:%02d",
            days, days==1?"":"s", hours, minutes, seconds);
    return uptime_buffer;
}

char * fmt_loadavg_info(LoadAverageInfo& la) {
    static char la_buffer[36];
    sprintf(la_buffer, "load average: %.2f %.2f %.2f",
            la.one_min, la.five_mins, la.fifteen_mins);
    return la_buffer;
}

char * fmt_cpuinfo_info(SystemInfo& sys, SystemInfo& sys_last, size_t cpu_no) {
    static char cpuinfo_buffer[80];
    double percent_user =(sys.cpus[cpu_no].user_time
                   - sys_last.cpus[cpu_no].user_time)
                   / (double)(sys.cpus[cpu_no].total_time() -
                 sys_last.cpus[cpu_no].total_time()) * 100;
    double percent_krnl =(sys.cpus[cpu_no].total_system_time()
                   - sys_last.cpus[cpu_no].total_system_time())
                   / (double)(sys.cpus[cpu_no].total_time() -
                 sys_last.cpus[cpu_no].total_time()) * 100;
    double percent_idle =(sys.cpus[cpu_no].total_idle_time()
                   - sys_last.cpus[cpu_no].total_idle_time())
                   / (double)(sys.cpus[cpu_no].total_time() -
                 sys_last.cpus[cpu_no].total_time()) * 100;
    sprintf(cpuinfo_buffer, "Cpu%c: %5.1f%% user, %5.1f%% kernel, %5.1f%% idle",
            cpu_no?'0'+(char)cpu_no-1:'T', percent_user, percent_krnl, percent_idle
        );
    return cpuinfo_buffer;
}

char * fmt_proc_info(SystemInfo& sys) {
    static char proc_buffer[80];
    sprintf(proc_buffer, "%d total processes, %d running",
            sys.num_processes, sys.num_running);
    return proc_buffer;
}

char * fmt_thread_info(SystemInfo& sys) {
    static char thread_buffer[80];
    sprintf(thread_buffer, "%d total threads, %d user, %d kernel",
            sys.num_threads, sys.num_user_threads, sys.num_kernel_threads);
    return thread_buffer;
}

char * fmt_mem_info() {
    static char mem_buffer[80];
    MemoryInfo mem = get_memory_info();
    sprintf(mem_buffer, "MEM:  total %s, used %s, avail %s",
            fmt_bytes(mem.total_memory<<10),
            fmt_bytes((mem.total_memory - mem.free_memory)<<10),
            fmt_bytes(mem.free_memory<<10)
        );
    return mem_buffer;
}

char * fmt_tbl_header() {
    static char tbl_header_buffer[80];
    sprintf(tbl_header_buffer, "%-5s %-7s %-1s %-5s %-8s %s",
            "PID", "Memory", "S", "CPU%", "Time", "Command"
        );
    return tbl_header_buffer;
}

char * fmt_tbl_item(ProcessInfo& proc) {
    static char tbl_item_buffer[256];
    sprintf(tbl_item_buffer, "%-5d %-7s %c %-5.1f %-8s %.220s",
            proc.pid, fmt_bytes(proc.rss*sysconf(_SC_PAGESIZE)), proc.state,
            proc.cpu_percent, fmt_time((proc.utime+proc.stime)/sysconf(_SC_CLK_TCK)), proc.command_line.c_str());
    return tbl_item_buffer;
}

char * fmt_bytes(unsigned long long amount) {
    static size_t state;
    static char buffers[16][24];
    char *buf = buffers[state++%16];
    if (!(amount >> 10)) {
        sprintf(buf, "%lluB", amount);
        return buf;
    }
    if (!(amount >> 20)) {
        sprintf(buf, "%.1fK", (double)amount/0x400);
        return buf;
    }
    if (!(amount >> 30)) {
        sprintf(buf, "%.1fM", (double)amount/0x100000);
        return buf;
    }
    sprintf(buf, "%.1fG", (double)amount/0x40000000);
    return buf;
}

char * fmt_time(int seconds) {
    static char buf[10];
    sprintf(buf, "%02d:%02d:%02d", seconds/3600, (seconds/60) % 60, seconds % 60);
    return buf;
}
