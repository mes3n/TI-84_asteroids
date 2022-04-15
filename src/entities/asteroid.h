#ifndef ASTEROID_H
#define ASTEROID_H

#include "../utilities/logic.h"

#define maxNumAsteroids 40
#define asteroidCorners 8

typedef struct Asteroid {

    // positional attributes
    vector2 velocity;
    vector2 center;

    // shape calculations
    uint8_t radius;
    float relShape[asteroidCorners*2];
    int shape[asteroidCorners*2];

} Asteroid;

extern Asteroid asteroids[maxNumAsteroids];  // declare asteroids variable 

void asteroidInit (Asteroid *, uint8_t, float);
void asteroidMove (Asteroid *, float);
void asteroidSplit (Asteroid *, uint8_t);
uint8_t asteroidAreGone (Asteroid *);

#endif  // ASTEROID_H
