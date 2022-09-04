#ifndef FILE_H
#define FILE_H

#include <fileioc.h>

char * location = ti_W;  // the location of the varialbe to save score to

void saveVar (float);
float readVar (void);

#endif  // FILE_H
