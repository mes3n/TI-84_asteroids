#include "../gfx/gfx.h"
#include "logic.h"
#include "../entities.h"

#include "graphics.h"

void capFPS (uint16_t ticksPerFrame, float * dt) {

    while (timer_Get(TIMER) < ticksPerFrame) {}  // do nothing until 1/{fps} seconds to have passed

    *dt = timer_Get(TIMER) / ticksPerFrame;  // get factor for how much past said time it is
    timer_Set(TIMER, 0);  // reset timer
}

void initGraphics (void) {

    gfx_Begin(); // start drawing routine
    gfx_SetDrawBuffer(); // set drawing to buffer

    // insert {black, white, gray} into first part of pallette
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetColor(WHITE); // set draw color to white
    gfx_SetTextFGColor(WHITE);  // set text color to white
    gfx_SetTextBGColor(BLACK);  // set background to black
    gfx_SetTextTransparentColor(BLACK);  // set all black to transparent

    // timer_Enable(which timer, frequency, -, count up)
    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
    timer_Set(TIMER, 0);  // set timer to 0

}

void checkCollisions (void) {

    if (ship.invulnerability <= 0) {  // ship is vulnerable
        for (uint8_t i = 0; i < maxNumAsteroids; i++) {  // look through array of asteroids
            if (asteroids[i].radius) {  // if asteroid exists
                if (nearby(ship.center, asteroids[i].center, shipHeight/2 + asteroids[i].radius)) {

                    if (rayCastingCollision(ship.shape, ship.nCorners, asteroids[i].shape, asteroidCorners) ||
                        rayCastingCollision(asteroids[i].shape, asteroidCorners, ship.shape, ship.nCorners)) {
                        shipSpawn(&ship, 1);
                        ship.lives -= 1;  // one life is lost
                        break;
                    }
                }
            }
        }
    }
    else {
        ship.invulnerability -= 1;  // count down on invulnerability
    }

    for (uint8_t is = 0; is < maxNumShots; is++) {
        if (shots[is].center.x != -1 || shots[is].center.y != -1) {  // if shot exists

            for (uint8_t ia = 0; ia < maxNumAsteroids; ia++) {
                if (asteroids[ia].radius) {  // if asteroid exists

                    if (nearby(shots[is].center, asteroids[ia].center, asteroids[ia].radius)) {

                        if (rayCastingCollision(shots[is].shape, 1, asteroids[ia].shape, asteroidCorners)) {
                            ship.score += 25 - asteroids[ia].radius;

                            if (asteroids[ia].radius > 15)  // split asteroid only if big enough
                                asteroidSplit(asteroids, ia);
                            else
                                asteroids[ia].radius = 0;  // remove asteroid

                            // remove shot
                            shots[is].center.x = -1;
                            shots[is].center.y = -1;
                        }
                    }
                }
            }
        }
    }
}


void draw (void) {

    gfx_ZeroScreen(); // fill screen with first color in global pallette (black)

    gfx_Polygon(ship.shape, ship.nCorners); // draw ship
    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius)  // asteroid exists
            gfx_Polygon(asteroids[i].shape, asteroidCorners); // draw asteroids
    }
    for (uint8_t i = 0; i < maxNumShots; i++) {
        if (shots[i].center.x != -1 || shots[i].center.y != -1) {  // shot exists
            // quickfix to make sure shot is in the right direction
            gfx_Line(shots[i].shape[0], shots[i].shape[1],
                shots[i].shape[0] - shots[i].velocity.x,
                shots[i].shape[1] - shots[i].velocity.y);
        }
    }

    for (uint8_t i = 0; i < ship.lives; i++) {
        int minishipShape[8] = {
            5 + 0 + 8*i, 8 + -6,
            5 + 3 + 8*i, 8 + 6,
            5 + 0 + 8*i, 8 + 5,
            5 + -3 + 8*i, 8 + 6
        }; // small ship shape around the coordinates (5, 8) offset by (8*i, 8)
        gfx_Polygon(minishipShape, 4); // draw small ships
    }

    gfx_SetTextXY(5 + 3 + 8*ship.lives, 8 - 2);  // slightly right of lives
    gfx_PrintUInt(ship.score, 4);  // print the score

    gfx_SwapDraw(); // show the buffer

}

void drawMenu (uint16_t highscore, uint8_t mode) {

    gfx_ZeroScreen();  // fill black

    if (mode == 0) {
        asteroidInit(asteroids, 10, 1);
        for (uint8_t i = 0; i < 5; i++) {
            asteroidSplit(asteroids, i);
        }  // create medium asteroids
        for (uint8_t i = 0; i < 7; i++) {
            asteroidSplit(asteroids, i);
        }  // create small asteroids
        asteroidMove(asteroids, 50);
    }  // init the asteroids and split some, then move them far from origin

    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius) {
            gfx_Polygon(asteroids[i].shape, asteroidCorners); // draw asteroids
            if (mode == 0) gfx_BlitBuffer();  // show the drawing of the asteroids
        }
    }

    gfx_SetTextScale(3, 3);  // scale text by height*3 and width*3

    // center "ASTEROIDS" string
    uint16_t x_pos = (WIDTH - gfx_GetStringWidth("ASTEROIDS")) * 0.5;

    // draw foreground in ligther color and background string in darker color
    gfx_SetTextFGColor(GRAY);
    gfx_PrintStringXY("ASTEROIDS", x_pos + 1, HEIGHT * 0.2);
    gfx_SetTextFGColor(WHITE);
    gfx_PrintStringXY("ASTEROIDS", x_pos - 1, HEIGHT * 0.2);

    gfx_SetTextScale(1, 1);  // normal scale

    char score[6];  // hold the characters of the score
    real_t tmp_real = os_FloatToReal(highscore);  // convert score to real variable
    os_RealToStr(score, &tmp_real, 6, 0, -1);  // convert real variable to chars

    // center of "highscore - $score"
    x_pos = (WIDTH - (gfx_GetStringWidth("highscore - ") + gfx_GetStringWidth(score))) * 0.5;
    gfx_PrintStringXY("highscore - ", x_pos, HEIGHT * 0.2 + 30);
    gfx_PrintStringXY(score, x_pos + gfx_GetStringWidth("highscore - "), HEIGHT * 0.2 + 30);

    // print creater name
    x_pos = (WIDTH - gfx_GetStringWidth("created by mes3n")) * 0.5;
    gfx_PrintStringXY("created by mes3n", x_pos, HEIGHT - 10);
    gfx_SwapDraw();

}
