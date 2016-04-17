#pragma once

#define BIT_MASK_N(n) (1 << (n))

#define AND_BIT_N(value, n) ((value) & BIT_MASK_N(n))

#define SET_BIT_N(value, n) ((value) | BIT_MASK_N(n))
#define CLEAR_BIT_N(value, n) ((value) & ~BIT_MASK_N(n))
