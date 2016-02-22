/**
 * Shadow Recruit top:
 * some great top by Jack Rosenthal
 */

#include <stdlib.h>
#include <getopt.h>
#include <ncurses.h>
#include <string.h>

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

int main(int argc, char **argv) {
    opts_init(argc, argv);
    initscr();
    curs_set(FALSE);
    timeout(100*opts.delay_tenths);

    int tick = 1;

    while (true) {
        wclear(stdscr);

        // Display the counter using printw (an ncurses function)
        printw("Behold, the number:\n%d", tick++);

        // Redraw the screen.
        refresh();

        // End the loop and exit if Q is pressed
        exit_if_user_presses_q();
    }

    // ncurses teardown
    endwin();

    return EXIT_SUCCESS;
}
