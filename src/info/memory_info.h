#pragma once


/**
 * Simple struct representing the amount of available and used memory on the
 * system.
 */
struct MemoryInfo {
    /**
     * The total amount of memory installed on the system.
     */
    unsigned long long total_memory;

    /**
     * The amount of memory that is currently available for use.
     */
    unsigned long long free_memory;

    /**
     * Relatively temporary storage for raw disk blocks that shouldn't get
     * tremendously large (20MB or so).
     */
    unsigned long long buffers_memory;

    /**
     * In-memory cache for files read from the disk (the page cache).
     */
    unsigned long long cached_memory;

    /**
     * Total amount of swap space available.
     */
    unsigned long long total_swap;

    /**
     * Amount of swap space that is currently unused.
     */
    unsigned long long free_swap;
};


/**
 * Returns a fully populated MemoryInfo instance, containing information about
 * the system's memory usage and available memory.
 */
MemoryInfo get_memory_info();
