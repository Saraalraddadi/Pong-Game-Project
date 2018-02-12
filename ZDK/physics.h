
#pragma once

#include "metrics.h"

/*
 * Paddle facing: the direction the paddle faces.
 *
 * Its opposite to direction the paddle is located!
 */
typedef enum {
    LEFT_PADDLE, RIGHT_PADDLE
} PaddleFacing;

/*
 * We model paddle as point with some width and thickness 1.
 * And it faces some direction.
 */
typedef struct {
    PaddleFacing facing;
    Object center;
    float width;
} Paddle;

/*
 * Wall facing: logic is the same as for paddles.
 */
typedef enum {
    UP, DOWN, LEFT, RIGHT
} WallFacing;

/*
 * The wall is a line containing "pivot" point and a direction.
 */
typedef struct {
    WallFacing facing;
    Object pivot;
} Wall;

/*
 * The singularity is a point with some mass.
 */
typedef struct {
    Object center;
    float mass;
} Singularity;

/*
 * Block thing has a center and ON/OFF flag.
 */
typedef struct {
    Object center;
    int on;
} Block;

/*
 * Construct given objects out of their components.
 */
Paddle mkPaddle(PaddleFacing facing, Object center, float width);
Wall mkWall(WallFacing facing, Object pivot);
Singularity mkSingularity(Object center, float mass);
Block mkBlock(Object center, int on);

/*
 * Check if object collides with given obstacle.
 */
int doesCollideWithPaddle(Object, Paddle);
int doesCollideWithWall(Object, Wall);
int doesCollideWithBlock(Object, Block);

/*
 * Act AS IF the object collided with given obstale.
 */
Object collideWithPaddle(Object, Paddle);
Object collideWithWall(Object, Wall);
Object collideWithBlock(Object, Block);

/*
 * = If collided, act as if collided.
 */
Object tryCollidingWithPaddle(Object, Paddle);
Object tryCollidingWithWall(Object, Wall);
Object tryCollidingWithBlock(Object, Block *);

/*
 * Recalculate object acceleration to singularity.
 */
Object getAffectedBySingularity(Object, Singularity);

Paddle movePaddleByY(float dy, Paddle paddle);

void print_paddle(Paddle paddle);
void print_wall(Wall wall);
void print_singilarity(Singularity singularity);
