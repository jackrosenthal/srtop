/**
 * Shadow Recruit top: opts.h
 * Header for options parsing
 */
#pragma once

/**
 * this contains the user's options after running opts_init
 */
enum sortby {PID, CPU, MEM, TIME};
struct progopts {
    int delay_tenths;
    enum sortby sort_key;
    int max_proc;
};
extern struct progopts opts;

/**
 * print help
 */
void opts_help(char **argv, int exit_status);

/**
 * initialize opts from argc and argv
 */
void opts_init(int argc, char **argv);
