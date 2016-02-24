/**
 * Shadow Recruit top:
 * some great top by Jack Rosenthal
 */
#include "utils/opts.h"
#include "utils/display.h"

int main(int argc, char **argv) {
    opts_init(argc, argv);
    display_init();
    return display_loop();
}
