#pragma once
#include <vector>


/**
 * Simple struct representing the amount of time units spent the CPU spent in
 * various modes.
 */
struct CpuInfo {
    /**
     * Time spent in user mode.
     */
    unsigned long long user_time;

    /**
     * Time spent in user mode with low priority (nice time).
     */
    unsigned long long nice_time;

    /**
     * Time spent in system mode.
     */
    unsigned long long system_time;

    /**
     * Time spent idling.
     */
    unsigned long long idle_time;

    /**
     * Time waiting for I/O to complete.
     */
    unsigned long long io_wait_time;

    /**
     * Time servicing interrupts.
     */
    unsigned long long irq_time;

    /**
     * Time servicing softirqs.
     */
    unsigned long long softirq_time;

    /**
     * Stolen time, which is the time spent in other operating systems when
     * running in a virtualized environment.
     */
    unsigned long long steal_time;

    /**
     * Time spent running a virtual CPU for guest operating systems under the
     * control of the Linux kernel.
     */
    unsigned long long guest_time;

    /**
     * Time spent running a niced guest (virtual CPU for guest operating systems
     * under the control of the Linux kernel).
     */
    unsigned long long guest_nice_time;

    /**
     * Returns the total amount of time spent idling (in the idle task or waiting
     * for I/O to complete).
     */
    unsigned long long total_idle_time() const {
        return idle_time + io_wait_time;
    }

    /**
     * Returns the total amount of time spent in system time (including the time
     * spent servicing interrupts).
     */
    unsigned long long total_system_time() const {
        return system_time + irq_time + softirq_time;
    }

    /**
     * Returns the total amount of time spent in virtual CPU time for guest
     * operating systems running on the machine.
     */
    unsigned long long total_virtual_time() const {
        return steal_time + guest_time + guest_nice_time;
    }

    /**
     * Returns the total amount of time this CPU has been active, including time
     * spent in all states.
     */
    unsigned long long total_time() const {
        return total_system_time()
            + total_idle_time()
            + total_virtual_time()
            + user_time
            + nice_time;
    }
};

/**
 * Returns a vector with one Cpu entry for each processor present in the system,
 * in the order the system sees them, with an additional entry at the beginning
 * of the vector representing the overal system CPU usage.
 */
std::vector<CpuInfo> get_cpu_info();
