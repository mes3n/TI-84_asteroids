#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include "gfx/gfx.h"

#include <math.h>

#define WIDTH 320
#define HEIGHT 240
#define numAsteroids 10
#define asteroidCorners 7

const uint8_t shipWidth = 10;
const uint8_t shipHeight = 20;

struct vector2 {
    float x;
    float y;
};

struct ship {

    struct vector2 velocity;
    struct vector2 center;

    float speed;
    uint16_t rotation;
    float acceleration;

    uint8_t nCorners;
    float relShape[8];
    int shape[8];

    uint8_t lives;
    int invulnerability;

} ship;

struct asteroid {

    struct vector2 velocity;
    struct vector2 center;

    uint8_t radius;
    uint8_t nCorners;
    float relShape[asteroidCorners*2];
    int shape[asteroidCorners*2];

} asteroids[numAsteroids];


void rotate (float *points, int rotation, int nCorners) {
    // rotates array passed to points around center by rotation degrees with nCorners amount of corners

    float theta = rotation / (180/M_PI);

    for (uint8_t i = 0; i < nCorners*2; i += 2) {
        float x = -1*(points[i])*cos(theta) - (points[i+1])*sin(theta);
        float y = -1*(points[i])*sin(theta) + (points[i+1])*cos(theta);

        points[i] = x;
        points[i+1] = y;
    }

}

void setShipRelShape (float *points, const uint8_t shipWidth, const uint8_t shipHeight,
    const int8_t offX, const int8_t offY) {

    points[0] = -shipHeight/2 + offX;
    points[1] = 0 + offY;
    points[2] = shipHeight/2 + offX;
    points[3] = shipWidth/2 + offY;
    points[4] = 0.425*shipHeight + offX;
    points[5] = 0 + offY;
    points[6] = shipHeight/2 + offX;
    points[7] = -shipWidth/2 + offY;

}

void shipMove () {

    if (ship.acceleration) {
        ship.velocity.x += ship.acceleration * cos(ship.rotation / (180/M_PI));
        ship.velocity.y += ship.acceleration * sin(ship.rotation / (180/M_PI));
    }

    ship.center.x += ship.velocity.x;
    ship.center.y += ship.velocity.y;

    ship.velocity.x *= 0.97;
    ship.velocity.y *= 0.97;

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

    rotate(ship.relShape, ship.rotation, ship.nCorners);
    for (uint8_t i = 0; i < ship.nCorners*2; i += 2) {
        ship.shape[i] = ship.center.x + ship.relShape[i];
        ship.shape[i+1] = ship.center.y + ship.relShape[i+1];
    }
    setShipRelShape(ship.relShape, shipWidth, shipHeight, 0, 0);

}

void asteroidInit () {

    for (uint8_t i = 0; i < numAsteroids; i++) {
        asteroids[i].radius = randInt(20, 25);
        asteroids[i].nCorners = asteroidCorners;

        asteroids[i].center.x = randInt(0, WIDTH);
        asteroids[i].center.y = randInt(0, HEIGHT);

        uint8_t speed = randInt(10, 20) / 10;
        uint16_t direction = randInt(0, 360);
        asteroids[i].velocity.x = speed * cos(direction / (180/M_PI));
        asteroids[i].velocity.y = speed * sin(direction / (180/M_PI));

        for (uint8_t j = 0; j < asteroids[i].nCorners*2; j += 2) {
            asteroids[i].relShape[j] = randInt(asteroids[i].radius / 2,
                asteroids[i].radius)*cos(2*M_PI*(j*0.5/asteroids[i].nCorners));
            asteroids[i].relShape[j+1] = randInt(asteroids[i].radius / 2,
                asteroids[i].radius)*sin(2*M_PI*(j*0.5/asteroids[i].nCorners));
        }
        rotate(asteroids[i].relShape, randInt(0, 60), asteroids[i].nCorners);
    }

}

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

/*
uint8_t rayCastingCollision_og (int *points, uint8_t corners_points, int *polygon, uint8_t corners_polygon) {
    uint8_t intersects = 0;

    for (uint8_t i = 0; i < corners_points*2; i += 2) {
        for (uint8_t j = 0; j < corners_polygon*2; j += 2) {
            int16_t x1 = polygon[j];
            int16_t y1 = polygon[j + 1];
            int16_t x2 = polygon[j == 0 ? (corners_polygon*2)-2 : j - 2];
            int16_t y2 = polygon[j == 0 ? (corners_polygon*2)-1 : j - 1];

            if (x1 - x2 != 0 && y1 - y2 != 0) {
                float k = ((y2 - y1)/(x2 - x1));
                float x = ((points[i + 1] - y1)/k) + x1;

                if (x < points[i]) {
                    if (x2 < x1) {
                        if (x2 < x && x < x1) intersects += 1;
                    }
                    if (x1 < x2) {
                        if (x1 < x && x < x2) intersects += 1;
                    }
                }
            }
        }
    }

    return (intersects % 2);  // return 1 if odd n intersects <=> inside polygon
}
*/

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

            /*if (y1 - y2 != 0) {
                float x;
                if (x1 - x2 == 0) {
                    x = x1; // or x2
                }
                else {
                    float k = ((y2 - y1)/(x2 - x1));
                    x = ((points[i + 1] - y1)/k) + x1;
                }

                if (x < points[i]) {
                    if (x2 < x1) {
                        if (x2 < x && x < x1) intersects += 1;
                    }
                    if (x1 < x2) {
                        if (x1 < x && x < x2) intersects += 1;
                    }
                    if (x1 == x2) {
                        intersects += 1;
                    }
                }
            }*/

            if (((y1 > py) != (y2 > py)) && (px < (x2 - x1) * (py - y1) / (y2 - y1) + x1))
                intersects += 1;

        }
        if (intersects % 2 == 1) return 1;
        else intersects = 0;
    }
    return 0;
}

void draw () {

    gfx_ZeroScreen();

    gfx_SetColor(255);

    gfx_Polygon(ship.shape, ship.nCorners);
    for (uint8_t i = 0; i < numAsteroids; i++) {
        gfx_Polygon(asteroids[i].shape, asteroids[i].nCorners);
    }

    /*
    int miniShipShape[8] = {
        4, 2,
        6, 10,
        4, 3,
        2, 10
    };*/
    for (uint8_t i = 0; i < ship.lives; i++) {
        int miniShipShape[8] = {
            5 + 0 + 8*i, 8 + -6,
            5 + 3 + 8*i, 8 + 6,
            5 + 0 + 8*i, 8 + 5,
            5 + -3 + 8*i, 8 + 6
        };
        gfx_Polygon(miniShipShape, 4);
    }

    gfx_SwapDraw();

}

int main (void) {

    kb_key_t key;
    gfx_Begin();
    gfx_SetDrawBuffer();

    ship.lives = 3;
    ship.center.x = WIDTH / 2;
    ship.center.y = HEIGHT / 2;

    ship.velocity.x = 0;
    ship.velocity.y = 0;
    ship.rotation = 270;

    ship.nCorners = 4;
    setShipRelShape(ship.relShape, shipWidth, shipHeight, 0, 0);

    asteroidInit();

    ship.invulnerability = 100;

    do {

        kb_Scan();
        key = kb_Data[7];

        ship.acceleration = (key & kb_Up ? 0.6 : 0);
        ship.rotation += (key & kb_Left ? -18 : 0);
        ship.rotation += (key & kb_Right ? 18 : 0);

        shipMove();

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

    } while (ship.lives && kb_Data[6] != kb_Clear);

    gfx_End();
    return 0;
}
