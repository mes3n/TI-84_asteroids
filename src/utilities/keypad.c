#include "../entities.h"
#include "keypad.h"

void scanKeypad (float dt) {

    kb_Scan();  // scan keyboard
    key = kb_Data[7];  // look at one part of the scan

    // if (key & {pressed_key}) do left option
    ship.acceleration = (key & kb_Up ? 0.6 : 0);

    // if either BOTH or NONE of kb_Left and kb_Right are pressed
    if (((key & kb_Left) && (key & kb_Right)) || (!(key & kb_Left) && !(key & kb_Right))) {
        x = 0.5;
    }
    // if ONE of kb_Left or kb_Right is pressed
    else if ((key & kb_Left) || (key & kb_Right)) {
        x += 0.1;
    }
    // increase rotation more as more time has passed
    ship.rotation += (key & kb_Left ? -18*dt*x : 0);
    ship.rotation += (key & kb_Right ? 18*dt*x : 0);

    // set a random position for the ship
    if (key & kb_Down) {
        ship.center.x = randInt(0, WIDTH);
        ship.center.y = randInt(0, HEIGHT);
    }


    key = kb_Data[1];  // look at another part of the scan

    // make sure the player can't rapid fire by holding down kb_2nd
    if (!(key & kb_2nd)) {  // if kb_2nd isn't pressed
        shotCooldown = 0;
    }
    else if (shotCooldown > 0) {  // if there is cooldown and kb_2nd was pressed
        shotCooldown -= 1;
    }
    else if (key & kb_2nd) {  // no cooldown and kb_2nd was pressed
            // shotShoot(shots, origin_x, origin_y, direction)
            shotShoot(shots, ship.shape[0], ship.shape[1], ship.rotation);
            shotCooldown = 10;
    }
}
