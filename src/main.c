#include "utilities.h"
#include "entities.h"

// define following variables
Ship ship;
Shot shots[maxNumShots];
Asteroid asteroids[maxNumAsteroids];


void init () {

    shipSpawn(&ship, 0);
    shotInit(shots);
    asteroidInit(asteroids, 5, 1);

}

void move (float dt) {

    shipMove(&ship, dt);
    shotMove(shots, dt);
    asteroidMove(asteroids, dt);

}

int main (void) {

    uint16_t highscore = readVar();  // previous highscore
    uint8_t wave = 0;  // count the waves

    initGraphics();  // initialize graphic related processes

    do {  // app loop

        drawMenu(highscore, 0);  // init asteroids and draw menu
        while (!os_GetCSC()) {
            asteroidMove(asteroids, 1);  // move asteroids to improve menu
            drawMenu(highscore, 1);  // only draw menu
        }

        init();  // init the entities

        do {  // gameloop

            capFPS(ticksPerFrame, &dt);  // cap fps value set in graphics.h

            scanKeypad(dt);

            move(dt);  // move entities

            checkCollisions();  // from graphics.h

            draw();  // from graphics.h

            if (asteroidAreGone(asteroids)) {  // if all asteroids are gone
                wave += 1;  // next wave
                // create more and faster asteroids based of linear equations
                asteroidInit(asteroids, 3*wave + 5, 0.3*wave + 1);
                ship.invulnerability = 70;  // add invulnerability
            }

        } while (ship.lives && kb_Data[6] != kb_Clear); // exit if dead or clear was pressed

        if (ship.score > highscore) {  // see if highscore was beaten
            saveVar(ship.score);  // in that case, save it
            highscore = ship.score;  // set highscore variable to new highscore
        }

    } while (kb_Data[6] != kb_Clear);  // exit if clear is pressed

    gfx_End(); // stop drawing routines
    return 0;  // exit program
}
