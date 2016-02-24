/**
 * Shadow Recruit top:
 * some great top by Jack Rosenthal
 */

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <unordered_map>
#include <algorithm>
#include "info/system_info.h"
#include "utils/fmt.h"
#include "utils/opts.h"

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below.
 */
void input_handler() {
    char c = getch();
    if (c == 'q') {
        endwin();
        exit(EXIT_SUCCESS);
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
    printw("%*s", col - x, "");
}

int main(int argc, char **argv) {
    opts_init(argc, argv);
    int row, col, x, y;
    initscr();
    if (has_colors() == TRUE) {
        start_color();
        assume_default_colors(COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);         /* inverted */
        init_pair(2, COLOR_WHITE, COLOR_RED);           /* alert */
    }
    curs_set(FALSE);
    timeout(100*opts.delay_tenths);
    SystemInfo sys_last = get_system_info();
    sys_last.processes.empty();
    for (SystemInfo sys = get_system_info();; sys_last = sys, sys = get_system_info()) {

        /* Calculate cpu_percent for each process */
        std::unordered_map<int, ProcessInfo *> lproc;
        for (ProcessInfo& proc: sys_last.processes)
            lproc[proc.pid] = &proc;
        for (ProcessInfo& proc: sys.processes) {
            if (lproc.count(proc.pid)) {
                proc.cpu_percent = (double)((proc.utime + proc.stime) -
                    (lproc[proc.pid]->utime + lproc[proc.pid]->stime))
                    / (sys.cpus[1].total_time() - sys_last.cpus[1].total_time()) * 100;
            }
            else proc.cpu_percent = 0;
        }

        /* Sort processes by sort key */
        std::sort(sys.processes.begin(), sys.processes.end());

        wclear(stdscr);
        getmaxyx(stdscr, row, col);
        printw("%s,  %s\n", fmt_uptime_info(sys.uptime), fmt_loadavg_info(sys.load_average));
        for (size_t cpu_id = 0; cpu_id < sys.cpus.size(); cpu_id++)
            printw("%s\n", fmt_cpuinfo_info(sys, sys_last, cpu_id));
        printw("%s\n", fmt_proc_info(sys));
        printw("%s\n", fmt_thread_info(sys));
        printw("%s\n", fmt_mem_info());
        attron(COLOR_PAIR(1));
        printw("%s", fmt_tbl_header());
        fill_ln();
        attroff(COLOR_PAIR(1));
        getyx(stdscr, y, x);
        int starty = y;
        for (ProcessInfo proc: sys.processes) {
            if (opts.max_proc && y - starty >= opts.max_proc) break;
            if (proc.state == 'R') attron(COLOR_PAIR(2));
            printw("%-*.*s", col, col, fmt_tbl_item(proc));
            attroff(COLOR_PAIR(2));
            if (++y >= row - 1) break;
        }
        const char keys[4][5] = {"PID", "CPU", "MEM", "TIME"};
        move(row-1, 0);
        attron(COLOR_PAIR(1));
        mvprintw(row-1, 0, "Shadow Recruit top - delay: %d tenths, sort-key: %s, max-proc: %d",
                opts.delay_tenths, keys[(size_t)opts.sort_key], opts.max_proc);
        fill_ln();
        attroff(COLOR_PAIR(1));
        refresh();
        input_handler();
    }
    endwin();

    return EXIT_SUCCESS;
}
