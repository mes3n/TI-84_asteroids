#ifndef SHOT_H
#define SHOT_H

#include "../utilities/logic.h"

#define maxNumShots 10

// attributes for the shots
typedef struct Shot {

    // positional attributes
    vector2 velocity;
    vector2 center;

    int shape[2];

} Shot;

extern Shot shots[maxNumShots];

void shotInit(Shot *);
void shotShoot(Shot *, int, int, uint16_t);
void shotMove(Shot *, float);

#endif  // SHOT_H
