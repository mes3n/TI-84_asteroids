#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

#include <math.h>

#define WIDTH 320
#define HEIGHT 240
#define numAsteroids 10
#define maxNumAsteroids 40
#define numShots 10
#define asteroidCorners 7

const uint8_t shipWidth = 10;
const uint8_t shipHeight = 20;

// vector struct
struct vector2 {
    float x;
    float y;
};

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

} shots[numShots];

struct asteroid {

    // positional attributes
    struct vector2 velocity;
    struct vector2 center;

    // shape calculations
    uint8_t radius;
    uint8_t nCorners;
    float relShape[asteroidCorners*2];
    int shape[asteroidCorners*2];

} asteroids[maxNumAsteroids];

// rotates array passed to points around center by rotation
// degrees with nCorners amount of corner
void rotate (float *points, int rotation, int nCorners) {

    float theta = rotation / (180/M_PI);  // convert from degrees to radians

    // rotates the list of points with a matrix
    for (uint8_t i = 0; i < nCorners*2; i += 2) {
        float x = -1*(points[i])*cos(theta) - (points[i+1])*sin(theta);
        float y = -1*(points[i])*sin(theta) + (points[i+1])*cos(theta);

        points[i] = x;
        points[i+1] = y;
    }

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

// move the ship
void shipMove () {

    // calculate velocity with trigonometry
    if (ship.acceleration) {
        ship.velocity.x += ship.acceleration * cos(ship.rotation / (180/M_PI));
        ship.velocity.y += ship.acceleration * sin(ship.rotation / (180/M_PI));
    }

    // move ship
    ship.center.x += ship.velocity.x;
    ship.center.y += ship.velocity.y;

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
void shotMove () {
    for (uint8_t i = 0; i < numShots; i++) {
        if (shots[i].center.x != -1 && shots[i].center.y != -1) {

            // move shot
            shots[i].center.x += shots[i].velocity.x;
            shots[i].center.y += shots[i].velocity.y;

            // make sure shot is within map
            if (shots[i].center.x < 0 || shots[i].center.x > WIDTH ||
                shots[i].center.y < 0 || shots[i].center.y > HEIGHT) {
                    shots[i].center.x = -1;
                    shots[i].center.y = -1;
                    shots[i].velocity.x = 0;
                    shots[i].velocity.y = 0;
            }
        }
    }
}

// initialize variables for asteroids
void asteroidInit () {

    for (uint8_t i = 0; i < numAsteroids; i++) {
        // random size
        asteroids[i].radius = randInt(20, 25);
        asteroids[i].nCorners = asteroidCorners;

        // random center
        asteroids[i].center.x = randInt(0, WIDTH);
        asteroids[i].center.y = randInt(0, HEIGHT);

        // set random speed and direction
        uint8_t speed = randInt(10, 20) / 10;
        uint16_t direction = randInt(0, 360);
        asteroids[i].velocity.x = speed * cos(direction / (180/M_PI));
        asteroids[i].velocity.y = speed * sin(direction / (180/M_PI));

        // create the ships shape
        for (uint8_t j = 0; j < asteroids[i].nCorners*2; j += 2) {
            asteroids[i].relShape[j] = randInt(asteroids[i].radius / 2,
                asteroids[i].radius)*cos(2*M_PI*(j*0.5/asteroids[i].nCorners));
            asteroids[i].relShape[j+1] = randInt(asteroids[i].radius / 2,
                asteroids[i].radius)*sin(2*M_PI*(j*0.5/asteroids[i].nCorners));
        }
        // random rotation
        rotate(asteroids[i].relShape, randInt(0, 60), asteroids[i].nCorners);
    }

}

// move asteroids and control wheather they are within bounds
void asteroidMove () {

    for (uint8_t i = 0; i < numAsteroids; i++) {
        asteroids[i].center.x += asteroids[i].velocity.x;
        asteroids[i].center.y += asteroids[i].velocity.y;

        for (uint8_t j = 0; j < asteroids[i].nCorners*2; j += 2) {
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

// collision detection for a point and a polygon
uint8_t rayCastingCollision (int *points, uint8_t corners_points, int *polygon, uint8_t corners_polygon) {
    uint8_t intersects = 0;

    for (uint8_t i = 0; i < corners_points*2; i += 2) {
    	int16_t px = points[i];
        int16_t py = points[i+1];
        for (uint8_t j = 0; j < corners_polygon*2; j += 2) {
            int16_t x1 = polygon[j];
            int16_t y1 = polygon[j + 1];
            int16_t x2 = polygon[j == 0 ? (corners_polygon*2)-2 : j - 2];
            int16_t y2 = polygon[j == 0 ? (corners_polygon*2)-1 : j - 1];

            // idk gave up and copied it
            if (((y1 > py) != (y2 > py)) && (px < (x2 - x1) * (py - y1) / (y2 - y1) + x1))
                intersects += 1;

        }
        // if intersects is odd the ray started inside the polygon
        // this means the point is inside the polygon
        if (intersects % 2 == 1) return 1;
            else intersects = 0; // reset
    }
    return 0;
}

void shoot () {

    for (uint8_t i = 0; i < numShots; i++) {
        if (shots[i].center.x == -1 && shots[i].center.y == -1) {

            // set shot center to the ship's tip
            shots[i].center.x = ship.shape[0];
            shots[i].center.y = ship.shape[1];

            // give twice the velocity of ship
            uint8_t speed = 7;
            shots[i].velocity.x = speed * cos(ship.rotation / (180/M_PI));
            shots[i].velocity.y = speed * sin(ship.rotation / (180/M_PI));

            return;
        }
    }

}

void draw () {

    gfx_ZeroScreen(); // fill screen with first color in global pallette (black)

    gfx_SetColor(255); // set draw color to white

    gfx_Polygon(ship.shape, ship.nCorners); // draw ship
    for (uint8_t i = 0; i < numAsteroids; i++) {
        gfx_Polygon(asteroids[i].shape, asteroids[i].nCorners); // draw asteroids
    }
    for (uint8_t i = 0; i < numShots; i++) {
        if (shots[i].center.x != -1 && shots[i].center.y != -1) {
            gfx_Line(shots[i].center.x, shots[i].center.y,
                shots[i].center.x - shots[i].velocity.x,
                shots[i].center.y - shots[i].velocity.y);
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

    kb_key_t key;  // variable to hold keypad input
    gfx_Begin(); // start drawing routine
    gfx_SetDrawBuffer(); // set drawing to buffer

    ship.lives = 3;
    ship.center.x = WIDTH / 2;
    ship.center.y = HEIGHT / 2;

    ship.velocity.x = 0;
    ship.velocity.y = 0;
    ship.rotation = 270;

    ship.nCorners = 4;
    setShipRelShape(ship.relShape, shipWidth, shipHeight, 0, 0);

    for (uint8_t i = 0; i < numShots; i++) {
        shots[i].center.x = -1;
        shots[i].center.y = -1;

        shots[i].velocity.x = 0;
        shots[i].velocity.y = 0;
    }
    uint8_t shotCooldown = 10;

    asteroidInit();

    ship.invulnerability = 100;

    do {

        kb_Scan();
        key = kb_Data[7];

        ship.acceleration = (key & kb_Up ? 0.6 : 0);
        ship.rotation += (key & kb_Left ? -18 : 0);
        ship.rotation += (key & kb_Right ? 18 : 0);

        key = kb_Data[1];

        // make sure the player can't rapid fire
        if (shotCooldown) {
            shotCooldown -= 1;
        }
        else {
            if (key & kb_2nd) {
                shotCooldown = 10;
                shoot();
            }
        }

        shipMove();

        shotMove();

        asteroidMove();

        genShipShape();

        if (ship.invulnerability <= 0) {
            for (uint8_t i = 0; i < numAsteroids; i++) {
                if ((ship.center.x - asteroids[i].center.x)*(ship.center.x - asteroids[i].center.x)
                  + (ship.center.y - asteroids[i].center.y)*(ship.center.y - asteroids[i].center.y)
                  <= (shipHeight/2 + 25)*(shipHeight/2 + 25)) {
                    if (rayCastingCollision(ship.shape, ship.nCorners, asteroids[i].shape, asteroids[i].nCorners)) {
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
        else {
            ship.invulnerability -= 1;
        }

        draw();

    } while (ship.lives && kb_Data[6] != kb_Clear); // exit if clear is pressed or all lives are lost

    gfx_End(); // stop drawing routine
    return 0;
}
