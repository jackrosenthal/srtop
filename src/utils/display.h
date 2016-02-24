/**
 * Shadow Recruit top: Display drawing functions
 */
#pragma once
#include "../info/system_info.h"

/**
 * initialize the display
 */
void display_init();

/**
 * handle input, like 'q' to quit
 */
void display_input_handler();

/**
 * Right align text using spaces on a line
 */
void display_fill_ra(const char *text);

/**
 * Draw uptime and loadavg to the screen
 */
void display_draw_uptime(SystemInfo& sys);

/**
 * Draw processors to top of display
 */
void display_draw_cpus(SystemInfo& sys, SystemInfo& sys_last);

/**
 * Draw process, thread, mem info to display
 */
void display_draw_cinfo(SystemInfo& sys);

/**
 * Draw process table
 */
void display_draw_tbl(SystemInfo& sys);

/**
 * Draw bottom status bar
 */
void display_draw_sbar();

/**
 * draw a single instance of the display
 */
void display_draw(SystemInfo& sys, SystemInfo& sys_last);

/**
 * draw the display in a loop
 */
int display_loop();
