#ifndef STACK_CONFIG_H_INCLUDED
#define STACK_CONFIG_H_INCLUDED

constexpr int REAL_MULTIPLIER           = 2;
constexpr int REAL_REDUCER              = 2 * REAL_MULTIPLIER;
constexpr int REAL_ADDER                = 4;
const int POISON_SIZES              = NAN;


enum
{
    NULL_STACK_PTR      = 1,
    NULL_DATA_PTR       = 2,
    CUR_BIGGER_CAPACITY = 3,
    CUR_LESS_ZERO       = 4,
    CAPACITY_LESS_ZERO  = 6,
    CLASSIFY_CAPACITY   = 5,
    CLASSIFY_CUR        = 7,
    ERROR_DATA_LEFT     = 9,
    ERROR_DATA_RIGHT    = 10,
    ERROR_STACK_LEFT    = 11,
    ERROR_STACK_RIGHT   = 12,
    HACK_STACK          = 13,
    MAX_CAPACITY_ERROR  = 14,
};


#endif
