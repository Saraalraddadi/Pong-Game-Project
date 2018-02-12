
#include <unistd.h>
#include "level.h"
#include "graphics.h"

int main(int argc, char const *argv[]) {

    Level level[4];

    level[0] = mkLevel(
        mkPaddle(LEFT_PADDLE, mkStationaryObject(v( -5, 0)), 3.5),
        mkPaddle(RIGHT_PADDLE,  mkStationaryObject(v(5000, 0)), 3.5),
        mkObject(v(0, 0), v(-1, 0.5), v(0, 0)),
        LEFT, // player is facing left (= is on right side)
        mkSingularity(mkStationaryObject(v(5000, 5)), 0.0),
        0
    );

    level[1] = mkLevel(
        mkPaddle(LEFT_PADDLE, mkStationaryObject(v( -5, 0)), 3.5),
        mkPaddle(RIGHT_PADDLE,  mkStationaryObject(v(5, 0)), 3.5),
        mkObject(v(0, 0), v(-1, 0.5), v(0, 0)),
        LEFT,
        mkSingularity(mkStationaryObject(v(5000, 5)), 0.0),
        0
    );

    level[2] = mkLevel(
        mkPaddle(LEFT_PADDLE, mkStationaryObject(v( -5, 0)), 3.5),
        mkPaddle(RIGHT_PADDLE,  mkStationaryObject(v(5, 0)), 3.5),
        mkObject(v(0, 0), v(-1, 0.5), v(0, 0)),
        LEFT,
        mkSingularity(mkStationaryObject(v(0, 0)), 0.5),
        0
    );

    level[3] = mkLevel(
        mkPaddle(LEFT_PADDLE, mkStationaryObject(v( -5, 0)), 3.5),
        mkPaddle(RIGHT_PADDLE,  mkStationaryObject(v(5, 0)), 3.5),
        mkObject(v(0, 0), v(-1, 0.5), v(0, 0)),
        LEFT,
        mkSingularity(mkStationaryObject(v(5000, 5)), 0.0),
        1
    );

    int levelNumber = 0;

    initGraphics();
    level[0] = initGameSession(level[0]);
    level[0] = showHelp(level[0]);
    while (1) {
        Input input = getInput();
        switch (input) {
            case NEXT_LEVEL:
                levelNumber++;
                levelNumber %= 4;
                level[levelNumber] = initGameSession(level[levelNumber]);
                continue;

            case HELP:
                level[levelNumber] = showHelp(level[levelNumber]);
                continue;

            default:
                break;
        }
        level[levelNumber] = performStep(1, input, level[levelNumber]);
        level[levelNumber] = redrawLevel(level[levelNumber], levelNumber);
        usleep(20000);
    }
    return 0;
}
