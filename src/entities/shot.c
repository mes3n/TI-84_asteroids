#include "shot.h"

void shotInit (Shot * shots) {

    for (uint8_t i = 0; i < maxNumShots; i++) {
        shots[i].center.x = -1;
        shots[i].center.y = -1;

        shots[i].velocity.x = 0;
        shots[i].velocity.y = 0;
    }
}

// shoot the a shot from ships tip
void shotShoot (Shot * shots, int origin_x, int origin_y, uint16_t direction) {

    for (uint8_t i = 0; i < maxNumShots; i++) {
        if (shots[i].center.x == -1 && shots[i].center.y == -1) {  // find empty

            // set shot center to the ship's tip
            shots[i].center.x = origin_x;
            shots[i].center.y = origin_y;

            // give a velocity of 7 in the direction of the ship
            uint8_t speed = 7;
            shots[i].velocity.x = speed * cos(direction / (180/M_PI));
            shots[i].velocity.y = speed * sin(direction / (180/M_PI));

            return;
        }
    }
}

// move the shots
void shotMove (Shot * shots, float dt) {
    for (uint8_t i = 0; i < maxNumShots; i++) {
        if (shots[i].center.x != -1 || shots[i].center.y != -1) {

            // move shot
            shots[i].center.x += shots[i].velocity.x * dt;
            shots[i].center.y += shots[i].velocity.y * dt;

            // make sure shot is within map
            if (shots[i].center.x < 0 || shots[i].center.x > WIDTH ||
                shots[i].center.y < 0 || shots[i].center.y > HEIGHT) {
                    shots[i].center.x = -1;
                    shots[i].center.y = -1;
                    shots[i].velocity.x = 0;
                    shots[i].velocity.y = 0;
            }
            shots[i].shape[0] = shots[i].center.x;
            shots[i].shape[1] = shots[i].center.y;
        }
    }
}
