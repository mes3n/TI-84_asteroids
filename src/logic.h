#include <tice.h>

// vector struct
struct vector2 {
    float x;
    float y;
};

void rotate (float *, int, int);

void setShipRelShape (float *, const uint8_t, const uint8_t, const int8_t, const int8_t);

uint8_t rayCastingCollision (int *, uint8_t, int *, uint8_t);
