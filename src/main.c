#include "utilities.h"
#include "entities.h"

Ship ship;
Shot shots[maxNumShots];
Asteroid asteroids[maxNumAsteroids];

// TODO.txt
// add "acceleration" for the player's rotation
// see over frame drops - optimize loops
// add "waves" of asteroids spawning
// make sure #includes are placed correctly

void init (uint8_t mode) {
    if (mode == 0) {
        initGraphics();
    }

    shipSpawn(&ship, 0);
    shotInit(shots);
    asteroidInit(asteroids, 6);

}

void move (float dt) {

    shipMove(&ship, dt);
    shotMove(shots, dt);
    asteroidMove(asteroids, dt);

}

int main (void) {

    uint16_t highscore = readVar();

    const uint8_t TIMER = 1;

    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
    timer_Set(TIMER, 0);  // set timer to 0
    float dt;  // time passed since last frame

    const uint8_t FPS = 20;  // customize the fps at will, display max is 60, i think
    const uint16_t ticksPerFrame = 32000/FPS;

    init(0);

    do {  // app loop

        drawMenu(highscore, 0);
        while (!os_GetCSC()) {
            asteroidMove(asteroids, 1);
            drawMenu(highscore, 1);
        }

        do {  // gameloop

            while (timer_Get(TIMER) < ticksPerFrame) {}  // do nothing until 1/{fps} seconds to have passed

            dt = timer_Get(TIMER) / ticksPerFrame;  // get factor for how much past said time it is
            timer_Set(TIMER, 0);  // reset timer

            scanKeypad(dt);

            move(dt);

            checkCollisions();

            draw();

        } while (ship.lives && kb_Data[6] != kb_Clear); // exit if dead or clear was pressed

        if (ship.score > highscore) {
            saveVar(ship.score);
            highscore = ship.score;
        }

        init(1);

    } while (kb_Data[6] != kb_Clear);  // exit if clear is pressed

    gfx_End(); // stop drawing routines
    return 0;  // exit program
}
