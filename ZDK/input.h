
#pragma once

/*
 * Since we don't want to compare nameless chars, we will make separate
 * type for inputs.
 */
typedef enum {
    NEXT_LEVEL, GO_UP, GO_DOWN, IDLE, HELP
} Input;

/*
 * Retrieve user's input (IDLE on no key pressed)
 */
Input getInput();
