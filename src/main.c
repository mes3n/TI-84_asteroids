#include <tice.h>
#include "utilities/graphics.h"
#include "utilities/keypad.h"
#include "gfx/gfx.h"

#include "utilities/logic.h"  // also includes math.h libary

#include "entities.h"

Ship ship;
Shot shots[maxNumShots];
Asteroid asteroids[maxNumAsteroids];

// TODO.txt
// add "acceleration" for the player's rotation

void init (uint8_t mode) {
    if (mode == 0) {
        gfx_Begin(); // start drawing routine
        gfx_SetDrawBuffer(); // set drawing to buffer

        gfx_SetPalette(global_palette, sizeof_global_palette, 0);

        gfx_SetColor(WHITE); // set draw color to white
        gfx_SetTextFGColor(WHITE);  // set text color to white
        gfx_SetTextBGColor(BLACK);  // set background to black
        gfx_SetTextTransparentColor(BLACK);  // set all black to transparent
    }

    shipSpawn(&ship, 0);
    shotInit(shots);
    asteroidInit(asteroids, 6);

}

void move (float dt) {

    shipMove(&ship, dt);
    shotMove(shots, dt);
    asteroidMove(asteroids, dt);

    genShipShape(&ship);

}

int main (void) {

    const uint8_t TIMER = 1;

    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
    timer_Set(TIMER, 0);  // set timer to 0
    float dt;  // time passed since last frame

    const uint8_t FPS = 30;
    const uint16_t ticksPerFrame = 32000/FPS;

    init(0);

    do {  // app loop

        drawMenu();
        while (!os_GetCSC()) {}

        do {  // gameloop

            while (timer_Get(TIMER) < ticksPerFrame) {}  // do nothing until 1/{fps} seconds to have passed

            dt = timer_Get(TIMER) / ticksPerFrame;  // get factor for how much past said time it is
            timer_Set(TIMER, 0);  // reset timer

            scanKeypad(dt);

            move(dt);

            checkCollisions();

            draw();

        } while (ship.lives && kb_Data[6] != kb_Clear); // exit if dead or clear was pressed

        init(1);

    } while (kb_Data[6] != kb_Clear);  // exit if clear is pressed

    gfx_End(); // stop drawing routines
    return 0;  // exit program
}
