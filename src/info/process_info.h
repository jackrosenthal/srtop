#pragma once
#include <string>
#include <vector>


/**
 * The max cmdline size on Linux.
 */
#define MAX_CMDLINE_LENGTH 4096


/**
 * Data structure containing information about a single process.
 */
struct ProcessInfo {
    /**
     * Total # of pages of memory.
     */
    long size;

    /**
     * Number of resident set (non-swapped) pages (4k).
     */
    long resident;

    /**
     * Number of pages of shared (mmap'd) memory.
     */
    long share;

    /**
     * Text resident set size.
     */
    long trs;

    /**
     * Shared-lib resident set size.
     */
    long lrs;

    /**
     * Data resident set size.
     */
    long drs;

    /**
     * Dirty pages.
     */
    long dt;

    /**
     * The process ID.
     */
    int pid;

    /**
     * The thread group ID.
     */
    int tgid;

    /**
     * The filename of the executable, in parentheses.
     */
    char comm[100];

    /**
     * The full command line that was typed to run the process
     */
    std::string command_line;

    /**
     * One character from the string "RSDZTW" where:
     *   - R is running
     *   - S is sleeping in an interruptible wait
     *   - D is waiting in uninterruptible disk sleep
     *   - Z is zombie
     *   - T is traced or stopped (on a signal)
     *   - W is paging
     */
    char state;

    /**
     * The PID of the parent.
     */
    int ppid;

    /**
     * The process group ID of the process.
     */
    int pgrp;

    /**
     * The session ID of the process.
     */
    int session;

    /**
     * The controlling terminal of the process.
     */
    int tty_nr;

    /**
     * The ID of the foreground process group of the controlling terminal of the
     * process.
     */
    int tpgid;

    /**
     * The kernel flags word of the process.
     */
    unsigned flags;

    /**
     * The number of minor faults the process_info.has made which have not required
     * loading a memory page from disk.
     */
    unsigned long minflt;

    /**
     * The number of minor faults that the process's waited-for children have
     * made.
     */
    unsigned long cminflt;

    /**
     * The number of major faults the process_info.has made which have required loading
     * a memory page from disk.
     */
    unsigned long majflt;

    /**
     * The number of major faults that the process's waited-for children have
     * made.
     */
    unsigned long cmajflt;

    /**
     * Amount of time that this process_info.has been scheduled in user mode, measured
     * in clock ticks (divide by sysconf(_SC_CLK_TCK).
     */
    unsigned long utime;

    /**
     * Amount of time that this process_info.has been scheduled in kernel mode,
     * measured in clock ticks (divide by sysconf(_SC_CLK_TCK).
     */
    unsigned long stime;

    /**
     * Amount of time that this process's waited-for children have been scheduled
     * in user mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK).
     */
    long cutime;

    /**
     * Amount of time that this process's waited-for children have been scheduled
     * in kernel mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK).
     */
    long cstime;

    /**
     * Priority of the process.
     */
    long priority;

    /**
     * The nice value (see setpriority(2)), a value in the range 19 (low priority)
     * to -20 (high priority).
     */
    long nice;

    /**
     * Number of threads in this process (since Linux 2.6). Before kernel 2.6,
     * this field was hard coded to 0 as a placeholder for an earlier removed
     * field.
     */
    long num_threads;

    /**
     * The time in jiffies before the next SIGALRM is sent to the process due to
     * an interval timer. Since kernel 2.6.17, this field is no longer maintained,
     * and is hard coded as 0.
     */
    long itrealvalue;

    /**
     * The time in jiffies the process started after system boot.
     */
    unsigned long long starttime;

    /**
     * Virtual memory size in bytes.
     */
    unsigned long vsize;

    /**
     * Resident Set Size: number of pages the process_info.has in real memory. This is
     * just the pages which count toward text, data, or stack space. This does not
     * include pages which have not been demand-loaded in, or which are swapped
     * out.
     */
    long rss;

    /**
     * Current soft limit in bytes on the rss of the process; see the description
     * of RLIMIT_RSS in getpriority(2).
     */
    unsigned long rsslim;

    /**
     * The address above which program text can run.
     */
    unsigned long startcode;

    /**
     * The address below which program text can run.
     */
    unsigned long endcode;

    /**
     * The address of the start (i.e., bottom) of the stack.
     */
    unsigned long startstack;

    /**
     * The current value of ESP (stack pointer), as found in the kernel stack page
     * for the process.
     */
    unsigned long kstkesp;

    /**
     * The current EIP (instruction pointer).
     */
    unsigned long kstkeip;

    /**
     * The bitmap of pending signals, displayed as a decimal number. Obsolete,
     * because it does not provide information on real-time signals; use
     * /proc/[pid]/status instead.
     */
    unsigned long signal;

    /**
     * The bitmap of blocked signals, displayed as a decimal number. Obsolete,
     * because it does not provide information on real-time signals; use
     * /proc/[pid]/status instead.
     */
    unsigned long blocked;

    /**
     * The bitmap of ignored signals, displayed as a decimal number. Obsolete,
     * because it does not provide information on real-time signals; use
     * /proc/[pid]/status instead.
     */
    unsigned long sigignore;

    /**
     * The bitmap of caught signals, displayed as a decimal number. Obsolete,
     * because it does not provide information on real-time signals; use
     * /proc/[pid]/status instead.
     */
    unsigned long sigcatch;

    /**
     * This is the "channel" in which the process is waiting. It is the address
     * of a system call, and can be looked up in a namelist if you need a textual
     * name. (If you have an up-to-date /etc/psdatabase, then try ps -l to see the
     * WCHAN field in action).
     */
    unsigned long wchan;

    /**
     * Number of pages swapped (not maintained).
     */
    unsigned long nswap;

    /**
     * Cumulative nswap for child processes (not maintained).
     */
    unsigned long cnswap;

    /**
     * Signal to be sent to parent when we die.
     */
    int exit_signal;

    /**
     * CPU number last executed on.
     */
    int processor;

    /**
     * Real-time scheduling priority, a number in the range 1 to 99 for processes
     * scheduled under a real-time policy, or 0, for non-real-time processes
     * (see sched_setscheduler(2)).
     */
    unsigned rt_priority;

    /**
     * Scheduling policy (see sched_setscheduler(2)). Decode using the SCHED_*
     * constants in linux/sched.h.
     */
    unsigned policy;

    /**
     * Aggregated block I/O delays, measured in clock ticks (centiseconds).
     */
    unsigned long long delayacct_blkio_ticks;

    /**
     * Guest time of the process (time spent running a virtual CPU for a guest
     * operating system), measured in clock ticks (divide by sysconf(_SC_CLK_TCK).
     */
    unsigned long guest_time;

    /**
     * Guest time of the process's children, measured in clock ticks (divide by
     * sysconf(_SC_CLK_TCK).
     */
    long cguest_time;

    /**
     * Threads belonging to this process.
     */
    std::vector<ProcessInfo> threads;

    /**
     * The percentage of a single CPU used by this process in the last
     */
    double cpu_percent;

    /**
     * Returns true if this is a kernel thread.
     */
    bool is_kernel_thread() const {
        return pgrp == 0;
    }

    /**
     * Returns true if this is a user-land thread.
     */
    bool is_user_thread() const {
        return pid != tgid;
    }

    /**
     * Returns true is this is either a user or kernel thread.
     */
    bool is_thread() const {
        return is_kernel_thread() || is_user_thread();
    }
};


/**
 * Returns a Process instance representing the process with the given process
 * ID (pid). The data for the process should be loaded from the directory formed
 * by joining the given basedir with the process ID (basedir/pid).
 */
ProcessInfo get_process(int pid, const char* basedir);


/**
 * Returns a vector consisting of one ProcessInfo entry for each process
 * currently present on the system (if passed the root of the /proc file
 * system),  or one entry for each thread in a single process (if passed a
 * /proc/<pid>/task basedir).
 */
std::vector<ProcessInfo> get_all_processes(const char* basedir);

/**
 * Operator less-than for sorting a processes
 * Requires program options to be defined
 */
int operator <(ProcessInfo& a, ProcessInfo& b);
