
#pragma once

/*
 * for convenient use of timer.
 */
typedef struct {
    int min;
    int sec;
} TimeSpent;

/*
 * for convenience when we set time at level struct.
 */
int toSeconds(TimeSpent timeSpent);
