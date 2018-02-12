
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "level.h"
#include "physics.h"
#include "utils.h"

Level performPhysicsSimulation(float time_delta, Level level);

Level adjustPlayerPosition(float dy, Level level);

/*
 * Move player paddle if need and go to simulation.
 */
Level performStep(float time_delta, Input input, Level level) {
    if (level.state == PLAYING)
        switch (input) {
            case GO_UP:
                level = adjustPlayerPosition(-1.5, level);
                break;

            case GO_DOWN:
                level = adjustPlayerPosition(1.5, level);
                break;

            default:;
        }

    return performPhysicsSimulation(time_delta, level);
}

Level refitOpponentPosition(Level level);
Paddle clampPaddlePosition(Paddle paddle, Level level);

Level performPhysicsSimulation(float time_delta, Level level) {
    switch (level.state) {
        case FAILED:
            level = reset_level(level);
            break;

        case COUNTDOWN:
            level.countdown--;
            if (!level.countdown || !level.lives)
                level.state = PLAYING;
            break;

        default: {
            Object ball = level.ball;

            /*
             * Simulate ball movement.
             */
            ball = tick(time_delta, ball);

            if (doesCollideWithPaddle(ball, level.player))
                level.score++;

            /*
             * Bounce the ball if required.
             */
            ball = tryCollidingWithPaddle(ball, level.player);
            ball = tryCollidingWithPaddle(ball, level.opponent);

            /*
             * Check if a life is lost.
             */
            if (doesCollideWithWall(ball, level.walls[level.playersWall])) {
                Level result = reset_level(level);
                result.lives = level.lives - 1;
                if (!level.lives) {
                    result.state = PLAYING;
                    result.countdown = 0;
                }
                return result;
            }

            if (!level.lives) {
                level.state = FAILED;
                return level;
            }

            /*
             * Bounce off walls, if applicable.
             */
            for (int i = 0; i < 4; i++)
                ball = tryCollidingWithWall(ball, level.walls[i]);

            for (int i = 0; i < level.blockCount; i++) {
                ball = tryCollidingWithBlock(ball, &level.blocks[0][i]);
                ball = tryCollidingWithBlock(ball, &level.blocks[1][i]);
            }

            if (level.timeSpent > 5) {
				if (ball.pos.x + 0.5 >= level.blackHole.center.pos.x ||
					ball.pos.x - 0.5 <= level.blackHole.center.pos.x ||
					ball.pos.y + 0.5 >= level.blackHole.center.pos.y ||
					ball.pos.y - 0.5 <= level.blackHole.center.pos.y  ) {
						ball = getAffectedBySingularity(ball, level.blackHole);
					}
			}
                

            level.ball = ball;
            level = refitOpponentPosition(level);

            /*
             * Check that paddles remain inside borders.
             */
            level.opponent = clampPaddlePosition(level.opponent, level);
            level.player = clampPaddlePosition(level.player, level);
        }
    }
    return level;
}

/*
 * Limit paddle position so it can't go outside playing area.
 */
Paddle clampPaddlePosition(Paddle paddle, Level level) {
    float bottom = level.walls[UP]  .pivot.pos.y + paddle.width;
    float top    = level.walls[DOWN].pivot.pos.y - paddle.width;
    paddle.center.pos.y = clamp(bottom, paddle.center.pos.y, top);
    return paddle;
}

/*
 * Make opponent paddle track ball.
 */
Level refitOpponentPosition(Level level) {
    level.opponent.center.pos.y = level.ball.pos.y;
    return level;
}

Level mkLevel(
    Paddle player,
    Paddle opponent,
    Object ball,
    WallFacing playersWall,
    Singularity blackHole,
    int haveBlocks
) {
    Level level = (Level) {
        player = player,
        opponent = opponent,
        ball = ball,
    };
    level.blackHole = blackHole;
    level.state = COUNTDOWN;
    level.playersWall = playersWall;
    level.haveBlocks = haveBlocks;
    return reset_level(level);
}

Level reset_level(Level level) {
    srand(time(0));

    /*
     * Generate random direction for the ball.
     */
    Vector dir = v((rand() % 50 + 50) / 50.0, (rand() % 60 - 30) / 50.0);

    dir = scaleV(dir, 1 / (0.01 + modulus(dir)));

    /*
     * Place the ball [back] to the center.
     */
    level.ball = mkStationaryObject(v(0, 0));
    level.ball.vel = dir;

    level.countdown = 150;
    level.score = 0;
    level.lives = 3;
    level.state = COUNTDOWN;

    for (int i = 0; i < level.blockCount; i++) {
        level.blocks[0][i].on = level.blocks[1][i].on = 1;
    }

    level.timeSpent = 0;

    return level;
}

Level adjustPlayerPosition(float dy, Level level) {
    level.player = movePaddleByY(dy, level.player);
    return level;
}

Level setTimeSpent(TimeSpent timeSpent, Level level) {
    level.timeSpent = toSeconds(timeSpent);
    return level;
}

void print_level(Level level) {
    switch (level.state) {
        case COUNTDOWN:
            printf("Level: COUNTDOWN - %d\n", level.countdown);
            break;

        case FAILED:
            printf("Level: Failed.\n");
            break;

        default:
            printf("Level:\n - Player: ");
            print_paddle(level.player);
            printf("\n - Ball: ");
            print_vector(level.ball.pos);

            printf("\n - Lives: %d", level.lives);
            printf("\n - Score: %d", level.score);
            printf("\n");
    }
}
