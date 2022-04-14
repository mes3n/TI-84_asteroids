#include "ship.h"

// move the ship
void shipMove (struct ship * ship, float dt) {

    // calculate velocity with trigonometry
    if (ship->acceleration) {
        ship->velocity.x += ship->acceleration * cos(ship->rotation / (180/M_PI));
        ship->velocity.y += ship->acceleration * sin(ship->rotation / (180/M_PI));
    }

    // move ship
    ship->center.x += ship->velocity.x * dt;
    ship->center.y += ship->velocity.y * dt;

    // deaccalerate
    ship->velocity.x *= 0.97;
    ship->velocity.y *= 0.97;

    // make sure ship is within map
    if (ship->center.x < 0)
        ship->center.x += WIDTH;
    if (ship->center.x > WIDTH)
        ship->center.x -= WIDTH;
    if (ship->center.y < 0)
        ship->center.y += HEIGHT;
    if (ship->center.y > HEIGHT)
        ship->center.y -= HEIGHT;

}

void genShipShape (struct ship * ship) {

    rotate(ship->relShape, ship->rotation, ship->nCorners); // rotate ship offset
    for (uint8_t i = 0; i < ship->nCorners*2; i += 2) {
        // add the ship offset to its center
        ship->shape[i] = ship->center.x + ship->relShape[i];
        ship->shape[i+1] = ship->center.y + ship->relShape[i+1];
    }
    // reset the ship offset
    setShipRelShape(ship->relShape, shipWidth, shipHeight, 0, 0);

}

// set the relative shape of the shape based of a center (offX, offY)
void setShipRelShape (float *points, const uint8_t shipWidth, const uint8_t shipHeight,
    const int8_t offX, const int8_t offY) {

    points[0] = -shipHeight*0.5 + offX;
    points[1] = 0 + offY;
    points[2] = shipHeight*0.5 + offX;
    points[3] = shipWidth*0.5 + offY;
    points[4] = 0.425*shipHeight + offX;
    points[5] = 0 + offY;
    points[6] = shipHeight*0.5 + offX;
    points[7] = -shipWidth*0.5 + offY;

}
