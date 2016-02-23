/**
 * Shadow Recruit top:
 * some great top by Jack Rosenthal
 */

#include <stdlib.h>
#include <getopt.h>
#include <ncurses.h>
#include <string.h>
#include "info/system_info.h"
#include "utils/fmt.h"

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}

/**
 * this contains the user's options after running opts_init
 */
enum sortby {PID, CPU, MEM, TIME};
static struct progopts {
    int delay_tenths;
    enum sortby sort_key;
} opts;

/**
 * print help
 */
void opts_help(char **argv, int exit_status) {
    printf("Shadow Recruit top:\n"
           "Usage: %s [OPTION ...]\n"
           "\n"
           "option          default     description\n"
           "======          =======     ===========\n"
           "-d --delay      10          tenths of seconds to delay between updates\n"
           "-s --sort-key   CPU         sort by PID, CPU, MEM or TIME\n"
           "-h --help                   this message\n",
           argv[0]
      );
    exit(exit_status);
}

/**
 * initialize opts from argc and argv
 */
void opts_init(int argc, char **argv) {
    const static struct option longopts[] = {
        {"delay", required_argument, NULL, 'd'},
        {"sort-key", required_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    opts.delay_tenths = 10;
    opts.sort_key = CPU;
    int i, optchar;
    while ((optchar = getopt_long(argc, argv, "d:s:h", longopts, &i)) != -1) {
        switch (optchar) {
            case 0:
                break;
            case 'd':
                sscanf(optarg, "%d", &opts.delay_tenths);
                break;
            case 's':
                if (!strcmp(optarg, "PID")) {
                    opts.sort_key = PID;
                    break;
                }
                if (!strcmp(optarg, "CPU")) {
                    opts.sort_key = CPU;
                    break;
                }
                if (!strcmp(optarg, "MEM")) {
                    opts.sort_key = MEM;
                    break;
                }
                if (!strcmp(optarg, "TIME")) {
                    opts.sort_key = TIME;
                    break;
                }
                printf("Unknown option for sort-key!\n\n");
                opts_help(argv, 2);
            case 'h':
                opts_help(argv, 0);
            default:
                printf("Unknown option\n\n");
                opts_help(argv, 2);
        }
    }
}

/**
 * A simple ncurses "macro" to fill the rest of a line with spaces.
 */
void fill_ln() {
    if (has_colors() == FALSE) {
        printw("\n");
        return;
    }
    int row, col, x, y;
    (void)row, (void)y;
    getmaxyx(stdscr, row, col);
    getyx(stdscr, y, x);
    for (int i = x; i < col; i++) printw(" ");
}

int main(int argc, char **argv) {
    opts_init(argc, argv);
    int row, col;
    initscr();
    if (has_colors() == TRUE) {
        start_color();
        assume_default_colors(COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
    }
    curs_set(FALSE);
    timeout(100*opts.delay_tenths);
    SystemInfo sys_last = get_system_info();
    for (SystemInfo sys = sys_last;; sys_last = sys, sys = get_system_info()) {
        wclear(stdscr);
        getmaxyx(stdscr, row, col);
        printw("%s,  %s\n", fmt_uptime_info(sys.uptime), fmt_loadavg_info(sys.load_average));
        for (size_t cpu_id = 0; cpu_id < sys.cpus.size(); cpu_id++)
            printw("%s\n", fmt_cpuinfo_info(sys, sys_last, cpu_id));
        printw("%s\n", fmt_proc_info(sys));
        printw("%s\n", fmt_thread_info(sys));
        printw("%s\n", fmt_mem_info(sys));
        attron(COLOR_PAIR(1));
        printw("%s", fmt_tbl_header());
        fill_ln();
        attroff(COLOR_PAIR(1));
        for (ProcessInfo proc: sys.processes)
            printw("%s\n", fmt_tbl_item(proc));
        const char keys[4][5] = {"PID", "CPU", "MEM", "TIME"};
        move(row-1, 0);
        clrtoeol();
        attron(COLOR_PAIR(1));
        mvprintw(row-1, 0, "Shadow Recruit top - delay: %d tenths, sort-key: %s",
                opts.delay_tenths, keys[(size_t)opts.sort_key]);
        fill_ln();
        attroff(COLOR_PAIR(1));
        refresh();
        exit_if_user_presses_q();
    }
    endwin();

    return EXIT_SUCCESS;
}
