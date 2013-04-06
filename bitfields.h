#pragma once

#define IS_SET(flag, bit)       (((flag) & (bit)) ? TRUE : FALSE)
#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
