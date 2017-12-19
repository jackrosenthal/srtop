/**
 * Some great top by Jack Rosenthal
 */
#include "opts.h"
#include "display.h"

int main(int argc, char **argv) {
    opts_init(argc, argv);
    display_init();
    return display_loop();
}
