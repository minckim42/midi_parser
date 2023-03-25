#pragma once
#include <cstdint>

/*
 * To use midi_out(...) without midi_init() is valid. 
 * Although initializing midi device takes short time, 
 * it would be long enough to be recognized by user.
*/
void midi_init();

int midi_out(uint32_t msg);
int midi_out(int channel, int instruction, int val0, int val1 = 0);
void midi_reset();