/**
 * Shadow Recruit top
 * Formatting utilities
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fmt.h"
#include "../info/system_info.h"
#include "../info/load_average_info.h"
#include "../info/process_info.h"

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

char * fmt_loadavg_info(LoadAverageInfo la) {
    static char la_buffer[36];
    sprintf(la_buffer, "load average: %.2f %.2f %.2f",
            la.one_min, la.five_mins, la.fifteen_mins);
    return la_buffer;
}

char * fmt_cpuinfo_info(SystemInfo sys, SystemInfo sys_last, size_t cpu_no) {
    static char cpuinfo_buffer[80];
    sprintf(cpuinfo_buffer, "TODO: cpuinfo_info");
    return cpuinfo_buffer;
}

char * fmt_proc_info(SystemInfo sys) {
    static char proc_buffer[80];
    sprintf(proc_buffer, "TODO: proc");
    return proc_buffer;
}

char * fmt_thread_info(SystemInfo sys) {
    static char thread_buffer[80];
    sprintf(thread_buffer, "TODO: thread");
    return thread_buffer;
}

char * fmt_mem_info(SystemInfo sys) {
    static char mem_buffer[80];
    sprintf(mem_buffer, "TODO: mem");
    return mem_buffer;
}

char * fmt_tbl_header() {
    static char tbl_header_buffer[80];
    sprintf(tbl_header_buffer, "%-5s %-7s %-1s %-4s %-8s %s",
            "PID", "Memory", "S", "CPU%", "Time", "Command"
        );
    return tbl_header_buffer;
}

char * fmt_tbl_item(ProcessInfo proc) {
    static char tbl_item_buffer[80];
    sprintf(tbl_item_buffer, "%-5d %-7s %c %-4s %-8s %s",
            proc.pid, fmt_bytes(proc.rss*sysconf(_SC_PAGESIZE)), proc.state,
            "CPU%", fmt_time((proc.utime+proc.stime)/sysconf(_SC_CLK_TCK)), proc.command_line.c_str());
    return tbl_item_buffer;
}

char * fmt_bytes(size_t amount) {
    static char buf[24];
    if (!(amount >> 10)) {
        sprintf(buf, "%luB", amount);
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
