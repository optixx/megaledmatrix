#ifndef LEDM_H
#define LEDM_H

#include <inttypes.h>

extern uint16_t gamma_table[4];

extern void clear_fb(uint8_t fb[][16]);
extern void scroll_h(uint8_t fb[][16], uint8_t dir);
extern void scroll_v(uint8_t fb[][16], uint8_t dir);
extern void set_px(uint8_t fb[][16], uint8_t x, uint8_t y, uint8_t bright);
extern void clr_px(uint8_t fb[][16], uint8_t x, uint8_t y);

#endif
