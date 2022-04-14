#ifndef LOGIC_H
#define LOGIC_H

#include <tice.h>
#include <math.h>

#define WIDTH 320
#define HEIGHT 240

// vector struct
struct vector2 {
    float x;
    float y;
};

void rotate (float *, int, int);
uint8_t rayCastingCollision (int *, uint8_t, int *, uint8_t);
uint8_t nearby (struct vector2, struct vector2, float);
float square (float);

#endif  // LOGIC
