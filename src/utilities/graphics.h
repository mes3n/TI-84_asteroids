#ifndef GRAPHICS_H
#define GRAPHICS_H

#define BLACK 0
#define WHITE 1
#define GRAY 2

#include <graphx.h>
#include "../gfx/gfx.h"

#include "../entities.h"
#include "logic.h"

void initGraphics (void);
void checkCollisions (void);
void draw (void);
void drawMenu (uint16_t, uint8_t);

#endif  // GRAPHICS_H
