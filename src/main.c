#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

#include <math.h>

#include "logic.h"

#define WIDTH 320
#define HEIGHT 240
#define maxNumAsteroids 40
#define maxNumShots 16
#define asteroidCorners 8

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

    // creating the asteroids shape
    uint8_t nCorners;
    float relShape[8];
    int shape[8];

    // survivability
    uint8_t lives;
    int invulnerability;

} ship;

// attributes for the shots
struct shot {

    // positional attributes
    struct vector2 velocity;
    struct vector2 center;

    int shape[2];

} shots[maxNumShots];

struct asteroid {

    // positional attributes
    struct vector2 velocity;
    struct vector2 center;

    // shape calculations
    uint8_t radius;
    float relShape[asteroidCorners*2];
    int shape[asteroidCorners*2];

} asteroids[maxNumAsteroids];

// move the ship
void shipMove (float dt) {

    // calculate velocity with trigonometry
    if (ship.acceleration) {
        ship.velocity.x += ship.acceleration * cos(ship.rotation / (180/M_PI));
        ship.velocity.y += ship.acceleration * sin(ship.rotation / (180/M_PI));
    }

    // move ship
    ship.center.x += ship.velocity.x * dt;
    ship.center.y += ship.velocity.y * dt;

    // deaccalerate
    ship.velocity.x *= 0.97;
    ship.velocity.y *= 0.97;

    // make sure ship is within map
    if (ship.center.x < 0)
        ship.center.x += WIDTH;
    if (ship.center.x > WIDTH)
        ship.center.x -= WIDTH;
    if (ship.center.y < 0)
        ship.center.y += HEIGHT;
    if (ship.center.y > HEIGHT)
        ship.center.y -= HEIGHT;

}

void genShipShape () {

    rotate(ship.relShape, ship.rotation, ship.nCorners); // rotate ship offset
    for (uint8_t i = 0; i < ship.nCorners*2; i += 2) {
        // add the ship offset to its center
        ship.shape[i] = ship.center.x + ship.relShape[i];
        ship.shape[i+1] = ship.center.y + ship.relShape[i+1];
    }
    // reset the ship offset
    setShipRelShape(ship.relShape, shipWidth, shipHeight, 0, 0);

}

// move the shots
void shotMove (float dt) {
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

// initialize variables for asteroids
void asteroidInit (uint8_t amount) {

    for (uint8_t i = 0; i < amount; i++) {
        // random size
        asteroids[i].radius = randInt(20, 25);

        // random center
        asteroids[i].center.x = randInt(0, WIDTH);
        asteroids[i].center.y = randInt(0, HEIGHT);

        // set random speed and direction
        uint8_t speed = randInt(10, 20) / 10;
        uint16_t direction = randInt(0, 360);
        asteroids[i].velocity.x = speed * cos(direction / (180/M_PI));
        asteroids[i].velocity.y = speed * sin(direction / (180/M_PI));

        // create the ships shape
        for (uint8_t j = 0; j < asteroidCorners*2; j += 2) {
            asteroids[i].relShape[j] = randInt(asteroids[i].radius / (j % 9 == 0 ? 4 : 2),
                asteroids[i].radius)*cos(2*M_PI*(j*0.5/asteroidCorners));
            asteroids[i].relShape[j+1] = randInt(asteroids[i].radius / (j % 9 == 0 ? 4 : 2),
                asteroids[i].radius)*sin(2*M_PI*(j*0.5/asteroidCorners));
        }
        // random rotation
        rotate(asteroids[i].relShape, randInt(0, 360), asteroidCorners);
    }

}

// move asteroids and control wheather they are within bounds
void asteroidMove (float dt) {

    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius) {
            asteroids[i].center.x += asteroids[i].velocity.x * dt;
            asteroids[i].center.y += asteroids[i].velocity.y * dt;

            for (uint8_t j = 0; j < asteroidCorners*2; j += 2) {
                asteroids[i].shape[j] = asteroids[i].center.x + asteroids[i].relShape[j];
                asteroids[i].shape[j+1] = asteroids[i].center.y + asteroids[i].relShape[j+1];
            }
            if (asteroids[i].center.x + asteroids[i].radius < 0)
                asteroids[i].center.x += WIDTH + 2*asteroids[i].radius;
            if (asteroids[i].center.x - asteroids[i].radius > WIDTH)
                asteroids[i].center.x -= WIDTH + 2*asteroids[i].radius;
            if (asteroids[i].center.y + asteroids[i].radius < 0)
                asteroids[i].center.y += HEIGHT + 2*asteroids[i].radius;
            if (asteroids[i].center.y - asteroids[i].radius > HEIGHT)
                asteroids[i].center.y -= HEIGHT + 2*asteroids[i].radius;
        }
    }
}

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
                            // ADD THE DEATH (SPLIT) OF ASTEROIDS HERE
                            asteroids[i].radius = 0;

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
    ship.center.x = WIDTH / 2;
    ship.center.y = HEIGHT / 2;

    ship.velocity.x = 0;
    ship.velocity.y = 0;
    ship.rotation = 270;

    ship.nCorners = 4;
    setShipRelShape(ship.relShape, shipWidth, shipHeight, 0, 0);

    for (uint8_t i = 0; i < maxNumShots; i++) {
        shots[i].center.x = -1;
        shots[i].center.y = -1;

        shots[i].velocity.x = 0;
        shots[i].velocity.y = 0;
    }

    asteroidInit(10);

    ship.invulnerability = 100;

}

void move (float dt) {

    shipMove(dt);
    shotMove(dt);
    asteroidMove(dt);

    genShipShape();

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

        while (timer_Get(TIMER) < ticksPerFrame) {}

        dt = timer_Get(TIMER) / ticksPerFrame;
        timer_Set(TIMER, 0);

        kb_Scan();
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
