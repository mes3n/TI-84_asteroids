#ifndef LOGIC_H
#define LOGIC_H

#include <tice.h>  // variable types and os functions
#include <math.h>  // used for trigonometric functions

// screen width and height
#define WIDTH 320
#define HEIGHT 240

// vector struct
typedef struct vector2 {
    float x;
    float y;
} vector2;

void rotate (float *, int, int);  // rotate an array of points
uint8_t rayCastingCollision (int *, uint8_t, int *, uint8_t);  // check collisions
uint8_t nearby (vector2, vector2, float);  // if two points are within a circle of each other
float square (float);  // return the square of value

#endif  // LOGIC
