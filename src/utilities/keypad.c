#include "keypad.h"

void scanKeypad (float dt) {

    kb_key_t key;

    kb_Scan();  // scan keyboard
    key = kb_Data[7];

    // if (key & {pressed_key}) do left option
    ship.acceleration = (key & kb_Up ? 0.6 : 0);
    ship.rotation += (key & kb_Left ? -18*dt : 0);
    ship.rotation += (key & kb_Right ? 18*dt : 0);

    if (key & kb_Down) {
        ship.center.x = randInt(0, WIDTH);
        ship.center.y = randInt(0, HEIGHT);
    }

    key = kb_Data[1];

    // make sure the player can't rapid fire by holding down kb_2nd
    if (!(key & kb_2nd)) {  // if kb_2nd isn't pressed
        shotCooldown = 0;
    }
    else if (shotCooldown > 0) {
        shotCooldown -= 1;
    }
    else if (key & kb_2nd) {
            shotShoot(shots, ship.shape[0], ship.shape[1], ship.rotation);
            shotCooldown = 10;
    }
}
