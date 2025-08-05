#ifndef __FIELD_H
#define __FIELD_H

#include <stdint.h>
#include "unit.h"

typedef struct field_unit {
    UnitType type;
    uint8_t x;
    uint8_t y;
    uint8_t hp;
} field_unit_t;

void field_add_unit(UnitType type, uint8_t x, uint8_t y);

#endif
