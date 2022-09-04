#ifndef GRAPHICS_H
#define GRAPHICS_H

// the position of each color in the palette
#define BLACK 0
#define WHITE 1
#define GRAY 2

// frequency of the timer
#define TIMER_FREQ 32000

#include <graphx.h>

const uint8_t TIMER = 1;  // timer to use, dont change
const uint8_t FPS = 20;  // the FPS to games renders at, can be changed
const uint16_t ticksPerFrame = TIMER_FREQ/FPS;  // how many ticks need to pass per frame

float dt;  // relative time passed since last frame

void capFPS (uint16_t, float *);  // cap the fps
void initGraphics (void);  // initialize graphics routines
void checkCollisions (void);  // check collisions
void draw (void);
void drawMenu (uint16_t, uint8_t);

#endif  // GRAPHICS_H
