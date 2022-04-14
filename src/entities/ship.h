#ifndef SHIP_H
#define SHIP_H

#include <tice.h>

#include "../logic.h"

const uint8_t shipWidth = 10;
const uint8_t shipHeight = 20;

// attributes for the ship
struct ship {

    // for drawing and calculation position
    struct vector2 velocity;
    struct vector2 center;

    // calculating position and velocity
    float speed;
    uint16_t rotation;
    float acceleration;

    // creating the ship's shape
    uint8_t nCorners;
    float relShape[8];
    int shape[8];

    // survivability
    uint8_t lives;
    int invulnerability;

    // score
    uint16_t score;

} ship;


void shipMove(struct ship *, float);
void genShipShape(struct ship *);
void setShipRelShape(float *, const uint8_t, const uint8_t, const int8_t, const int8_t);

#endif  // SHIP_H
