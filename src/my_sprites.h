#ifndef __MY_SPRITES_H
#define __MY_SPRITES_H

#include <stdint.h>

typedef struct my_metasprite
{
    uint8_t frame;
    uint8_t blinking_countdown;
    bool flipX;
    uint8_t props;
    uint8_t x;
    uint8_t y;
} my_metasprite_t;

#endif
