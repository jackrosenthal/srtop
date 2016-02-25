/**
 * Shadow Recruit top: Display drawing functions
 */
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <algorithm>
#include "fmt.h"
#include "opts.h"
#include "../info/system_info.h"

/**
 * initialize the display
 */
void display_init() {
    initscr();
    if (has_colors() == TRUE) {
        start_color();
        assume_default_colors(COLOR_WHITE, COLOR_BLACK);
        init_pair(1, COLOR_BLACK, COLOR_WHITE);         /* inverted */
        init_pair(2, COLOR_WHITE, COLOR_RED);           /* alert */
    }
    curs_set(FALSE);
    timeout(100*opts.delay_tenths);
}

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout().
 */
void display_input_handler() {
    char c = getch();
    if (c == 'q') {
        endwin();
        exit(EXIT_SUCCESS);
    }
}

/**
 * Right align text using spaces on a line
 */
void display_fill_ra(const char *text) {
    int row, col, x, y;
    (void)row, (void)y;
    getmaxyx(stdscr, row, col);
    getyx(stdscr, y, x);
    printw("%*.*s", col - x, col - x, text);
}

/**
 * Draw uptime and loadavg to the screen
 */
void display_draw_uptime(SystemInfo& sys) {
    printw("%s,  %s\n", fmt_uptime_info(sys.uptime), fmt_loadavg_info(sys.load_average));
}

/**
 * Draw processors to top of display
 */
void display_draw_cpus(SystemInfo& sys, SystemInfo& sys_last) {
    for (size_t cpu_id = 0; cpu_id < sys.cpus.size(); cpu_id++)
        printw("%s\n", fmt_cpuinfo_info(sys, sys_last, cpu_id));
}

/**
 * Draw process, thread, mem info to display
 */
void display_draw_cinfo(SystemInfo& sys) {
    printw("%s\n", fmt_proc_info(sys));
    printw("%s\n", fmt_thread_info(sys));
    printw("%s\n", fmt_mem_info());
}

/**
 * Draw process table
 */
void display_draw_tbl(SystemInfo& sys) {
    int row, col, x, y, starty;
    getmaxyx(stdscr, row, col);
    attron(COLOR_PAIR(1));
    printw("%s", fmt_tbl_header());
    display_fill_ra("");
    attroff(COLOR_PAIR(1));
    getyx(stdscr, y, x);
    starty = y;
    for (ProcessInfo proc: sys.processes) {
        if (opts.max_proc && y - starty >= opts.max_proc) break;
        if (proc.state == 'R') attron(COLOR_PAIR(2));
        printw("%-*.*s", col, col, fmt_tbl_item(proc));
        attroff(COLOR_PAIR(2));
        if (++y >= row - 1) break;
    }
}

/**
 * Draw bottom status bar
 */
void display_draw_sbar() {
    int row, col;
    getmaxyx(stdscr, row, col);
    move(row-1, 0);
    attron(COLOR_PAIR(1));
    mvprintw(row-1, 0, "Shadow Recruit top - delay: %d tenths, sort-key: %s, max-proc: %d",
            opts.delay_tenths, sortby_keys[(size_t)opts.sort_key], opts.max_proc);
    display_fill_ra("Right Align");
    attroff(COLOR_PAIR(1));
}

/**
 * draw a single instance of the display
 */
void display_draw(SystemInfo& sys, SystemInfo& sys_last) {
    calc_process_cpu(sys, sys_last);
    std::sort(sys.processes.begin(), sys.processes.end());
    wclear(stdscr);
    display_draw_uptime(sys);
    display_draw_cpus(sys, sys_last);
    display_draw_cinfo(sys);
    display_draw_tbl(sys);
    display_draw_sbar();
    refresh();
    display_input_handler();
}

/**
 * draw the display in a loop
 */
int display_loop() {
    SystemInfo sys_last = get_system_info(), sys = sys_last;
    sys_last.processes.clear();
    for (CpuInfo& cpu: sys_last.cpus) {
        cpu.idle_time -= opts.delay_tenths * 10 / sysconf(_SC_CLK_TCK);
    }
    for (;;) {
        display_draw(sys, sys_last);
        sys_last = sys, sys = get_system_info();
    }
    return 0;
}
