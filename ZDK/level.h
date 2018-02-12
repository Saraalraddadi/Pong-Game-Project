
#pragma once

#include "physics.h"
#include "input.h"
#include "timeSpent.h"

/*
 * Level status.
 */
typedef enum {
    COUNTDOWN,
    PLAYING,
    FAILED
} State;

/*
 * Level properties.
 */
typedef struct {
    Paddle player;
    Paddle opponent;
    Object ball;
    Wall walls[4];
    Block blocks[2][100];
    int blockCount, haveBlocks;
    WallFacing playersWall;
    Singularity blackHole;
    State state;
    int countdown;
    int score;
    int lives;
    int timeSpent;
} Level;

/*
 * Make level out of its configuration.
 */
Level mkLevel(
    Paddle player,
    Paddle opponent,
    Object ball,
    WallFacing playersWall,
    Singularity blackHole,
    int haveBlocks
);

/*
 * Given the input and the dime delta, simulate physics on whole level.
 */
Level performStep(float time_delta, Input input, Level level);

/*
 * Return level to the COUNTDOWN state.
 */
Level reset_level(Level level);

Level setTimeSpent(TimeSpent timeSpent, Level level);

void print_level(Level level);
