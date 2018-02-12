
#pragma once

#include "level.h"

void initGraphics();

Level initGameSession(Level);

Level redrawLevel(Level, int);

Level showHelp(Level);

int showGameOver();
