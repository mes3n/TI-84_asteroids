#ifndef KEYPAD_H
#define KEYPAD_H

#include <keypadc.h>

kb_key_t key;  // hold kb_Data

float x;  // get "time" elapsed since kb_Left or kb_Right was pressed
uint8_t shotCooldown;

void scanKeypad (float);

#endif  // KEYPAD_H
