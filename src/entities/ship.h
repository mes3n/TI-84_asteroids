#ifndef SHIP_H
#define SHIP_H

#include "../utilities/logic.h"

const uint8_t shipWidth = 10;
const uint8_t shipHeight = 20;

// attributes for the ship
typedef struct Ship {

    // for drawing and calculation position
    vector2 velocity;
    vector2 center;

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

} Ship;

extern Ship ship;

void shipSpawn(Ship *, uint8_t);
void shipMove(Ship *, float);
void genShipShape(Ship *);
void setShipRelShape(float *, const uint8_t, const uint8_t, const int8_t, const int8_t);

#endif  // SHIP_H
