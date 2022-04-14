#ifndef SHOT_H
#define SHOT_H

#include "../logic.h"

#define maxNumShots 16

// attributes for the shots
struct shot {

    // positional attributes
    struct vector2 velocity;
    struct vector2 center;

    int shape[2];

} shots[maxNumShots];

void shotMove(struct shot *, float);

#endif  // SHOT_H
