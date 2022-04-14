#include <tice.h>
#include "shot.h"

// move the shots
void shotMove (struct shot * shots, float dt) {
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
