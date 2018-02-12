
#include "timeSpent.h"

int toSeconds(TimeSpent timeSpent) {
    return timeSpent.sec + 60 * timeSpent.min;
}
