#include "../gfx/gfx.h"
#include "logic.h"
#include "../entities.h"

#include "graphics.h"

void initGraphics (void) {

    gfx_Begin(); // start drawing routine
    gfx_SetDrawBuffer(); // set drawing to buffer

    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    gfx_SetColor(WHITE); // set draw color to white
    gfx_SetTextFGColor(WHITE);  // set text color to white
    gfx_SetTextBGColor(BLACK);  // set background to black
    gfx_SetTextTransparentColor(BLACK);  // set all black to transparent

}

void checkCollisions (void) {

    if (ship.invulnerability <= 0) {
        for (uint8_t i = 0; i < maxNumAsteroids; i++) {
            if (asteroids[i].radius) {
                if (nearby(ship.center, asteroids[i].center, shipHeight/2 + asteroids[i].radius)) {

                    if (rayCastingCollision(ship.shape, ship.nCorners, asteroids[i].shape, asteroidCorners) ||
                        rayCastingCollision(asteroids[i].shape, asteroidCorners, ship.shape, ship.nCorners)) {
                        shipSpawn(&ship, 1);  // passing as a pointer
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

                    if (nearby(shots[ii].center, asteroids[i].center, asteroids[i].radius)) {

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


void draw (void) {

    gfx_ZeroScreen(); // fill screen with first color in global pallette (black)

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

    for (uint8_t i = 0; i < ship.lives; i++) {
        int minishipShape[8] = {
            5 + 0 + 8*i, 8 + -6,
            5 + 3 + 8*i, 8 + 6,
            5 + 0 + 8*i, 8 + 5,
            5 + -3 + 8*i, 8 + 6
        }; // small ship shape offset by center (8*i, 8)
        gfx_Polygon(minishipShape, 4); // draw small ships
    }

    gfx_SetTextXY(5 + 3 + 8*ship.lives, 8 - 2);  // slightly right off lives
    gfx_PrintUInt(ship.score, 4);  // print the "text"

    gfx_SwapDraw(); // show the buffer

}

void drawMenu (uint16_t highscore, uint8_t mode) {

    gfx_ZeroScreen();

    if (mode == 0) {
        asteroidInit(asteroids, 10);
        for (uint8_t i = 0; i < 5; i++) {
            asteroidSplit(asteroids, i);
        }
        asteroidMove(asteroids, 50);
    }

    for (uint8_t i = 0; i < maxNumAsteroids; i++) {
        if (asteroids[i].radius) {
            gfx_Polygon(asteroids[i].shape, asteroidCorners); // draw asteroids
            if (mode == 0) gfx_BlitBuffer();
        }
    }

    gfx_SetTextScale(3, 3);

    uint16_t x_pos = (WIDTH - gfx_GetStringWidth("ASTEROIDS")) * 0.5;

    gfx_SetTextFGColor(GRAY);
    gfx_PrintStringXY("ASTEROIDS", x_pos + 1, HEIGHT * 0.2);
    gfx_SetTextFGColor(WHITE);
    gfx_PrintStringXY("ASTEROIDS", x_pos - 1, HEIGHT * 0.2);

    gfx_SetTextScale(1, 1);

    char score[6];  // 20
    real_t tmp_real = os_FloatToReal(highscore);
    os_RealToStr(score, &tmp_real, 6, 0, -1);

    x_pos = (WIDTH - (gfx_GetStringWidth("highscore - ") + gfx_GetStringWidth(score))) * 0.5;
    gfx_PrintStringXY("highscore - ", x_pos, HEIGHT * 0.2 + 30);
    gfx_PrintStringXY(score, x_pos + gfx_GetStringWidth("highscore - "), HEIGHT * 0.2 + 30);



    gfx_SwapDraw();


}
