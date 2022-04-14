#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

#include "logic.h"  // also includes math.h libary

#include "entities/asteroid.h"
#include "entities/shot.h"
#include "entities/ship.h"

// TODO.txt
// add "acceleration" for the player's rotation


void shoot () {

    for (uint8_t i = 0; i < maxNumShots; i++) {
        if (shots[i].center.x == -1 && shots[i].center.y == -1) {

            // set shot center to the ship's tip
            shots[i].center.x = ship.shape[0];
            shots[i].center.y = ship.shape[1];

            // give a velocity of 7 in the direction of the ship
            uint8_t speed = 7;
            shots[i].velocity.x = speed * cos(ship.rotation / (180/M_PI));
            shots[i].velocity.y = speed * sin(ship.rotation / (180/M_PI));

            return;
        }
    }
}


void checkCollisions () {

    if (ship.invulnerability <= 0) {
        for (uint8_t i = 0; i < maxNumAsteroids; i++) {
            if (asteroids[i].radius) {

                if ((ship.center.x - asteroids[i].center.x)*(ship.center.x - asteroids[i].center.x)
                  + (ship.center.y - asteroids[i].center.y)*(ship.center.y - asteroids[i].center.y)
                  <= (shipHeight/2 + 25)*(shipHeight/2 + 25)) {

                    if (rayCastingCollision(ship.shape, ship.nCorners, asteroids[i].shape, asteroidCorners) ||
                        rayCastingCollision(asteroids[i].shape, asteroidCorners, ship.shape, ship.nCorners)) {
                        ship.invulnerability = 100;
                        ship.center.x = WIDTH / 2;
                        ship.center.y = HEIGHT / 2;
                        ship.velocity.x = 0;
                        ship.velocity.y = 0;
                        ship.lives -= 1;
                        break;
                    }
                }
            }
        }
    }
    else {
        ship.invulnerability -= 1;
    }

    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius) {

            for (uint8_t ii = 0; ii < maxNumShots; ii++) {
                if (shots[ii].center.x != -1 || shots[ii].center.y != -1) {

                    if ((shots[ii].center.x - asteroids[i].center.x)*(shots[ii].center.x - asteroids[i].center.x)
                    +   (shots[ii].center.y - asteroids[i].center.y)*(shots[ii].center.y - asteroids[i].center.y)
                    <=  (asteroids[i].radius*asteroids[i].radius)) {

                        if (rayCastingCollision(shots[ii].shape, 1, asteroids[i].shape, asteroidCorners)) {
                            ship.score += 25 - asteroids[i].radius;

                            if (asteroids[i].radius > 15) asteroidSplit(asteroids, i);
                            else asteroids[i].radius = 0;

                            shots[ii].center.x = -1;
                            shots[ii].center.y = -1;
                        }
                    }
                }
            }
        }
    }
}

void init () {

    ship.lives = 3;
    ship.score = 0;

    ship.center.x = WIDTH / 2;
    ship.center.y = HEIGHT / 2;

    ship.velocity.x = 0;
    ship.velocity.y = 0;
    ship.rotation = 270;

    ship.nCorners = 4;

    for (uint8_t i = 0; i < maxNumShots; i++) {
        shots[i].center.x = -1;
        shots[i].center.y = -1;

        shots[i].velocity.x = 0;
        shots[i].velocity.y = 0;
    }

    asteroidInit(asteroids, 10);

    ship.invulnerability = 100;

}

void move (float dt) {

    shipMove(&ship, dt);
    shotMove(shots, dt);
    asteroidMove(asteroids, dt);

    genShipShape(&ship);

}

void draw () {

    gfx_ZeroScreen(); // fill screen with first color in global pallette (black)

    gfx_SetColor(255); // set draw color to white

    gfx_Polygon(ship.shape, ship.nCorners); // draw ship
    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius)
            gfx_Polygon(asteroids[i].shape, asteroidCorners); // draw asteroids
    }
    for (uint8_t i = 0; i < maxNumShots; i++) {
        if (shots[i].center.x != -1 || shots[i].center.y != -1) {
            gfx_Line(shots[i].shape[0], shots[i].shape[1],
                shots[i].shape[0] - shots[i].velocity.x,
                shots[i].shape[1] - shots[i].velocity.y);
        }
    }

    /*
    int miniShipShape[8] = {
        4, 2,
        6, 10,
        4, 3,
        2, 10
    };
    */

    for (uint8_t i = 0; i < ship.lives; i++) {
        int miniShipShape[8] = {
            5 + 0 + 8*i, 8 + -6,
            5 + 3 + 8*i, 8 + 6,
            5 + 0 + 8*i, 8 + 5,
            5 + -3 + 8*i, 8 + 6
        }; // small ship shape offset by center (8*i, 8)
        gfx_Polygon(miniShipShape, 4); // draw small ships
    }

    gfx_SetTextFGColor(255);  // set text color to white
    gfx_SetTextBGColor(0);  // set background to black
    gfx_SetTextTransparentColor(0);  // set all black to transparent

    gfx_SetTextXY(5 + 3 + 8*ship.lives, 8 - 2);  // slightly right off lives
    gfx_PrintUInt(ship.score, 4);  // print the "text"

    gfx_SwapDraw(); // show the buffer

}

int main (void) {

    const uint8_t TIMER = 1;

    kb_key_t key;  // variable to hold keypad input
    gfx_Begin(); // start drawing routine
    gfx_SetDrawBuffer(); // set drawing to buffer

    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
    timer_Set(TIMER, 0);
    float dt;

    const uint8_t fps = 30;
    const uint16_t ticksPerFrame = 32000/fps;

    init();

    uint8_t shotCooldown = 10;

    do {

        while (timer_Get(TIMER) < ticksPerFrame) {}  // wait for 1/60 seconds to have passed

        dt = timer_Get(TIMER) / ticksPerFrame;  // get factor for how much past said time it is
        timer_Set(TIMER, 0);  // reset timer

        kb_Scan();  // scan keyboard
        key = kb_Data[7];

        ship.acceleration = (key & kb_Up ? 0.6 : 0);
        ship.rotation += (key & kb_Left ? -18*dt : 0);
        ship.rotation += (key & kb_Right ? 18*dt : 0);

        key = kb_Data[1];

        // make sure the player can't rapid fire by holding down kb_2nd
        if (!(key & kb_2nd)) {
            shotCooldown = 0;
        }
        else if (shotCooldown) {
            shotCooldown -= 1;
        }
        else if (key & kb_2nd) {
                shotCooldown = 10;
                shoot();
        }

        move(dt);

        checkCollisions();

        draw();

    } while (ship.lives && kb_Data[6] != kb_Clear); // exit if clear is pressed or all lives are lost

    gfx_End(); // stop drawing routine
    return 0;
}
