#ifndef ASTEROID_H
#define ASTEROID_H

#include "../logic.h"

#define maxNumAsteroids 40
#define asteroidCorners 8

struct asteroid {

    // positional attributes
    struct vector2 velocity;
    struct vector2 center;

    // shape calculations
    uint8_t radius;
    float relShape[asteroidCorners*2];
    int shape[asteroidCorners*2];

} asteroids[maxNumAsteroids];

void asteroidInit (struct asteroid *, uint8_t);
void asteroidMove (struct asteroid *, float);
void asteroidSplit (struct asteroid *, uint8_t);

#endif  // ASTEROID_H
