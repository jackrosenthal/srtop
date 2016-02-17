#include "process_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <vector>

using namespace std;

ProcessInfo get_process(int pid, const char* basedir) {
    char *stat_fn = (char *) alloca(strlen(basedir) + 18);
    sprintf(stat_fn, "%s/%d/stat", basedir, pid);
    FILE *stat_fp = fopen(stat_fn, "r");
    if (!stat_fp) {
        fprintf(stderr, "file error: %s\n", stat_fn);
        exit(1);
    }
    ProcessInfo info;
    fscanf(stat_fp, "%d %s %c %d %d %d %d %d %u "\
            "%lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld "\
            "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu "\
            "%d %d %u %u %llu %lu %ld",
            &(info.pid),
            info.comm,
            &(info.state),
            &(info.ppid),
            &(info.pgrp),
            &(info.session),
            &(info.tty_nr),
            &(info.tpgid),
            &(info.flags),
            &(info.minflt),
            &(info.cminflt),
            &(info.majflt),
            &(info.cmajflt),
            &(info.utime),
            &(info.stime),
            &(info.cutime),
            &(info.cstime),
            &(info.priority),
            &(info.nice),
            &(info.num_threads),
            &(info.itrealvalue),
            &(info.starttime),
            &(info.vsize),
            &(info.rss),
            &(info.rsslim),
            &(info.startcode),
            &(info.endcode),
            &(info.startstack),
            &(info.kstkesp),
            &(info.kstkeip),
            &(info.signal),
            &(info.blocked),
            &(info.sigignore),
            &(info.sigcatch),
            &(info.wchan),
            &(info.nswap),
            &(info.cnswap),
            &(info.exit_signal),
            &(info.processor),
            &(info.rt_priority),
            &(info.policy),
            &(info.delayacct_blkio_ticks),
            &(info.guest_time),
            &(info.cguest_time)
        );
    char *statm_fn = (char *) alloca(strlen(basedir) + 19);
    sprintf(statm_fn, "%s/%d/statm", basedir, pid);
    FILE *statm_fp = fopen(statm_fn, "r");
    if (!statm_fp) {
        fprintf(stderr, "file error: %s\n", statm_fn);
        exit(1);
    }
    fscanf(statm_fp, "%ld %ld %ld %ld %ld %ld %ld",
            &(info.size),
            &(info.resident),
            &(info.share),
            &(info.trs),
            &(info.lrs),
            &(info.drs),
            &(info.dt)
      );
    char *cmdline_fn = (char *) alloca(strlen(basedir) + 21);
    sprintf(cmdline_fn, "%s/%d/cmdline", basedir, pid);
    FILE *cmdline_fp = fopen(cmdline_fn, "r");
    if (!cmdline_fp) {
        fprintf(stderr, "file error: %s\n", cmdline_fn);
        exit(1);
    }
    char c;
    while ((c = fgetc(cmdline_fp)) != EOF) {
        if (c) info.command_line.push_back(c);
        else info.command_line.push_back(' ');
    }
    if (!info.command_line.length())
        info.command_line = std::string(info.comm + 1);
    info.command_line.pop_back();

    return info;
}


vector<ProcessInfo> get_all_processes(const char* basedir) {
    // TODO: implement me
    return vector<ProcessInfo>();
}
