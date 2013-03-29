#pragma once

#define IS_SET(flag, bit)       (((flag) & (bit)) ? true : false)
#define SET_BIT(var, bit)       ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
