#include "asteroid.h"

// initialize variables for asteroids
void asteroidInit (struct asteroid * asteroids, uint8_t amount) {

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
void asteroidMove (struct asteroid * asteroids, float dt) {

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


void asteroidSplit (struct asteroid * asteroids, uint8_t id) {

    float speedIncrease = 1.2;
    float sizeDecrease = 0.75;

    for (uint8_t i = 0; i < maxNumAsteroids; i++) {

        if (!asteroids[i].radius) {

            // inherit half radius
            asteroids[i].radius = asteroids[id].radius * sizeDecrease;

            // inherit same center
            asteroids[i].center.x = asteroids[id].center.x;
            asteroids[i].center.y = asteroids[id].center.y;

            // same speed but offset by delta direction
            uint16_t deltaDirection = randInt(20, 50);
            float velX = asteroids[id].velocity.x*cos(deltaDirection / (180/M_PI)) - asteroids[id].velocity.y*sin(deltaDirection / (180/M_PI));
            float velY = asteroids[id].velocity.y*cos(deltaDirection / (180/M_PI)) + asteroids[id].velocity.x*sin(deltaDirection / (180/M_PI));
            asteroids[i].velocity.x = speedIncrease * velX;
            asteroids[i].velocity.y = speedIncrease * velY;

            // create the ships shape
            for (uint8_t j = 0; j < asteroidCorners*2; j++) {
                asteroids[i].relShape[j] = asteroids[id].relShape[j] * sizeDecrease;
            }
            // random rotation
            rotate(asteroids[i].relShape, randInt(0, 360), asteroidCorners);

            break;
        }
    }

    // half the previous radius
    asteroids[id].radius *= sizeDecrease;

    // same speed but offset by delta direction
    uint16_t deltaDirection = -1 * randInt(20, 50);
    float velX = asteroids[id].velocity.x*cos(deltaDirection / (180/M_PI)) - asteroids[id].velocity.y*sin(deltaDirection / (180/M_PI));
    float velY = asteroids[id].velocity.y*cos(deltaDirection / (180/M_PI)) + asteroids[id].velocity.x*sin(deltaDirection / (180/M_PI));
    asteroids[id].velocity.x = speedIncrease * velX;
    asteroids[id].velocity.y = speedIncrease * velY;

    // create the ships shape
    for (uint8_t j = 0; j < asteroidCorners*2; j++) {
        asteroids[id].relShape[j] = asteroids[id].relShape[j] * sizeDecrease;
    }
    // random rotation
    rotate(asteroids[id].relShape, randInt(0, 360), asteroidCorners);

}
