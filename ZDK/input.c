
#include "input.h"

extern char get_char();

Input getInput() {
    switch (get_char()) {
        case 'l':
        case 'L':
            return NEXT_LEVEL;

        case 'w':
        case 'W':
            return GO_UP;

        case 's':
        case 'S':
            return GO_DOWN;

        case 'h':
        case 'H':
            return HELP;

        default:
            return IDLE;
    }
}
