
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "cab202_timers.h"
#include "cab202_graphics.h"
#include <curses.h>
#include "graphics.h"
#include "level.h"

void initGraphics() {
    setup_screen();
    fit_screen_to_window();
	override_screen_size(getmaxx(stdscr), getmaxy(stdscr) - ((getmaxy(stdscr) - 1) % 2));
}

/*
 * The var to store the moment of time, when the help screen is closed.
 */
int currentInterval = 0;

/*
 * Get current full interval from the start of the level.
 */
TimeSpent getTimeSpent(Level level) {
    int overall = level.timeSpent + (time(NULL) - currentInterval);

    TimeSpent timeSpent;
    timeSpent.min = overall / 60;
    timeSpent.sec = overall % 60;
    return timeSpent;
}

Level showHelp(Level level) {
    float width  = screen_width() - 1,
          height = screen_height() - 1;

    clear_screen();

    draw_line(0, 0, width, 0, '*');
    
    draw_line(0, 2, width, 2, '*');
    draw_line(0, 0, 0, height, '*');
    draw_line(width, 0, width, height, '*');
    draw_line(0, height, width, height, '*');
    draw_string(width / 2 - 2, 1, "HELP");
    draw_string(3, 4, "w - move paddle up");
    draw_string(3, 5, "s - move paddle down");
    draw_string(3, 6, "l - next level");
    draw_string(3, 7, "h - show this screen again");
    draw_string(3, 9, "(press any key to continue)");
    draw_string(3, 11, "Author: Sara Alraddadi");
    draw_string(3, 12, "StudentID: N8779333");
    int i = 0;
    for (;i<height;i++) {
        draw_char(i, width+1,'\n');
    }
    show_screen();

    /*
     * Store current full interval.
     */
    level.timeSpent += (time(NULL) - currentInterval);
    wait_char();

    /*
     * Do no count time spent in showing help.
     */
    currentInterval = time(NULL);
    return level;
}

int showGameOver() {
    float width  = screen_width() - 1,
          height = screen_height() - 1;

    clear_screen();

    draw_line(0, 0, width, 0, '*');
    draw_line(0, 2, width, 2, '*');
    draw_line(0, 0, 0, height, '*');
    draw_line(width, 0, width, height, '*');
    draw_line(0, height, width, height, '*');
    draw_string(width / 2 - 2, 1, "SORRY");
    draw_string(width / 2 - 10, height / 2, "BUT THE LEVEL IS OVER!");
    draw_string(width / 2 - 5, height / 2 + 1, "Play again (y/n)?");

    show_screen();
    while (1) {
        switch (wait_char()) {
            case 'y':
            case 'Y':
                return 1;

            case 'n':
            case 'N':
                return 0;

            default:
                continue;
        }
    }
}

Level initGameSession(Level level) {
    level = reset_level(level);
    currentInterval = time(NULL);

    float width  = screen_width() - 1,
          height = screen_height() - 1;

    /*
     * Place walls where they should be.
     */
    level.walls[UP]    = mkWall(UP,    mkStationaryObject(v(  0, -height / 2 + 3)));
    level.walls[DOWN]  = mkWall(DOWN,  mkStationaryObject(v(  0,  height / 2 - 1)));
    level.walls[LEFT]  = mkWall(LEFT,  mkStationaryObject(v( width / 2 - 1,   0)));
    level.walls[RIGHT] = mkWall(RIGHT, mkStationaryObject(v(-width / 2 + 1,   0)));

    /*
     * If opponent is not far, far away, place him on his side in the center.
     */
    if (fabs(modulus(level.opponent.center.pos)) < 1000)
        level.opponent.center.pos = v(-width / 2 + 3, 0);

    /*
     * Place player where he should be.
     */
    level.player.center.pos = v( width / 2 - 3, 0);

    float preWidth = (height - 4) / 4;

    /*
     * Limit paddle width of 7 cells.
     */
    if (preWidth > 3.5)
        preWidth = 3.5;

    level.player.width = preWidth;
    level.opponent.width = preWidth;

    int hd6 = height / 6;
    int wd4 = width  / 4;

    if (level.haveBlocks) {
        level.blockCount = width / 2;
        for (int i = 0; i < width / 2; i++) {
            level.blocks[0][i] = mkBlock(mkStationaryObject(v(i - wd4, hd6)), 1);
            level.blocks[1][i] = mkBlock(mkStationaryObject(v(i - wd4, -hd6)), 1);
        }
    } else {
        level.blockCount = 0;
    }

    return level;
}

void redrawPaddle(Paddle paddle, char brush);

void draw_block(Block block);

Level redrawLevel(Level level, int levelNumber) {

    int width = screen_width() - 1,
        height = screen_height() - 1;

    clear_screen();

    /*
     * Draw singularity.
     */
    draw_char(
        width  / 2 + level.blackHole.center.pos.x,
        height / 2 + level.blackHole.center.pos.y,
        '*'
    );


    for (int i = 0; i < level.blockCount; i++) {
        draw_block(level.blocks[1][i]);
        draw_block(level.blocks[0][i]);
    }
	



	
	draw_char(width / 2 + level.ball.pos.x, height / 2 + level.ball.pos.y, 'o');
	switch (level.state) {
        case COUNTDOWN:
            /*
             * Display countdown.
             */
            draw_string(width / 2 - 6, height / 2, "COUNTDOWN: ");
            draw_int(width / 2 + 6, height / 2, level.countdown / 50 + 1);
            break;

        case FAILED:
            if (!showGameOver()) {
                exit(0);
            }
            else {
                level = showHelp(level);           
            }
            break;

        default:
            break;
    }

    draw_line(0, 0, width, 0, '*');
    draw_line(0, 2, width, 2, '*');
    draw_line(0, 0, 0, height, '*');
    draw_line(width, 0, width, height, '*');
    draw_line(0, height, width, height, '*');

    draw_string(2, 1, "Lives:");
    draw_int(1 + 7, 1, level.lives);

    draw_string(10, 1, "* Score:");
    draw_int(10 + 9, 1, level.score);

    draw_string(21, 1, "* Time:   :  ");

    draw_string(41, 1, "* Level: ");
    draw_int(50, 1, levelNumber + 1);

    TimeSpent timeSpent = getTimeSpent(level);

    draw_int(21 + 13 - 4, 1, timeSpent.min);
    draw_int(21 + 17 - 6, 1, timeSpent.sec);

    /*
     * Do not count time while countdown is present.
     */
    if (level.state == PLAYING)
        level = setTimeSpent(timeSpent, level);
    currentInterval = time(0);
	
	redrawPaddle(level.player, '|');
    redrawPaddle(level.opponent, '|');
    show_screen();

    return level;
}

void draw_block(Block block) {
    if (!block.on)
        return;

    int width = screen_width() - 1,
        height = screen_height() - 1;

    Vector at = block.center.pos;

    draw_char(width / 2 + at.x, height / 2 + at.y, '=');
}

void redrawPaddle(Paddle paddle, char brush) {
    int width = screen_width() - 1,
        height = screen_height() - 1;

    draw_line(
        width  / 2 + paddle.center.pos.x,
        height / 2 + paddle.center.pos.y,
        width  / 2 + paddle.center.pos.x,
        height / 2 + paddle.center.pos.y + paddle.width,
        brush
    );
    draw_line(
        width  / 2 + paddle.center.pos.x,
        height / 2 + paddle.center.pos.y - paddle.width,
        width  / 2 + paddle.center.pos.x,
        height / 2 + paddle.center.pos.y,
        brush
    );
}
