#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <typeinfo>
#include <ctype.h>
#include <conio.h>
#include "stack_config.h"


#ifdef DOUBLE_T
    typedef double type_data;
    const type_data POISON             = NAN;
    const type_data CANARY_LEFT_DATA   = 0x1BF3334A;
    const type_data CANARY_RIGHT_DATA  = 0xFDF4223A;
    const int CANARY_L_STACK           = 0x1A3C3D7C;
    const int CANARY_R_STACK           = 0xADF44A23;
    #define FORMAT "lg"
#endif


#ifdef INT_T
    typedef int type_data;
    const type_data POISON             = INT_MAX;
    const type_data CANARY_LEFT_DATA   = 0x1BF3334A;
    const type_data CANARY_RIGHT_DATA  = 0xFDF4223A;
    const int CANARY_L_STACK           = 0x1A3C3D7C;
    const int CANARY_R_STACK           = 0xADF44A23;
    #define FORMAT "d"
#endif


#ifdef FLOAT_T
    typedef float type_data;
    const type_data POISON             = NAN;
    const type_data CANARY_LEFT_DATA   = 0x1BF3334A;
    const type_data CANARY_RIGHT_DATA  = 0xFDF4223A;
    const int CANARY_LEFT_STACK        = 0x1A3C3D7C;
    const int CANARY_RIGHT_STACK       = 0xADF44A23;
    #define FORMAT "f"
#endif

#ifdef STRING_T
    typedef char* type_data;
    const type_data POISON             = "LLLLLLLLLLL";
    const type_data CANARY_LEFT_DATA   = "glavz";
    const type_data CANARY_RIGHT_DATA  = "amoqc";
    const int CANARY_L_STACK           = 0x1A3C3D7C;
    const int CANARY_R_STACK           = 0xADF44A23;
    #define FORMAT "s"
#endif

#ifdef CHAR_T
    typedef char type_data;
    const type_data POISON             = 'z';
    const type_data CANARY_LEFT_DATA   = 's';
    const type_data CANARY_RIGHT_DATA  = 'm';
    const int CANARY_L_STACK           = 0x1A3C3D7C;
    const int CANARY_R_STACK           = 0xADF44A23;
    #define FORMAT "c"
#endif

#ifdef LOW_SECURE       // только проверка на Stack == nullptr, stack_construct
    const int low_sec   = 1;
    const int med_sec   = 0;
    const int high_sec  = 0;
#endif

#ifdef MEDIUM_SECURE    // все проверки, кроме хэширования и канареек
    const int low_sec   = 0;
    const int med_sec   = 1;
    const int high_sec  = 0;
#endif

#ifdef HIGH_SECURE      // все проверки
    const int low_sec   = 0;
    const int med_sec   = 0;
    const int high_sec  = 1;
#endif

#define ASSERT_OK                                    \
    if (stack_verify(Stack))                         \
    {                                                \
        stack_dump(Stack);                           \
        return;                                      \
    }

#define START_ACTIONS(a)                             \
    stack_t a = {};

#define Construct(a,b)                               \
        name = #a;                                   \
        name++;                                      \
        stack_construct(a, b, name);

#define ASSERT_POP_OK                                   \
    if (stack_verify(Stack))                            \
    {                                                   \
        stack_dump(Stack);                              \
        return POISON;                                  \
    }

#define CHECK_CAP                                       \
    if( ((FORMAT != "s") && (FORMAT != "c")             \
       && (push_num >= POISON)                          \
       || fpclassify(push_num) != FP_NORMAL))           \
        {                                               \
            stack_dump(Stack);                          \
                                                        \
            return;                                     \
        }

struct stack_t
{
    int canary_left_stack;

    type_data* data       = nullptr;
    char* name            = nullptr;
    int capacity          = 0;         //max size
    int cur_size          = 0;         //size now
    int hash_stack        = 0;

    int canary_right_stack;

};

//-----------------------------------------------------------------------------
//! @brief  Stack Construct
//! @param [in] Stack - pointer to stack_t struct
//! @param [in] max_c - maximum stack size at the time of construction
//! @param [in] name  - name of the stack
//! @author Hollbrok
//! @version 1.2.9
//! @brief
//-----------------------------------------------------------------------------


void stack_construct(stack_t* Stack, int max_c, char* name);

//-----------------------------------------------------------------------------
//! @brief  Stack Destruct
//! @param [in] Stack - pointer to stack_t struct
//! @author Hollbrok
//! @version 0.1.5
//! @brief
//-----------------------------------------------------------------------------

void stack_destruct(stack_t* Stack);

//-----------------------------------------------------------------------------
//! @brief  Push push_num to the stack
//! @param [in] Stack - pointer to stack_t struct
//! @param [in] push_num - puts this number (or char, string, other) to the stack
//! @author Hollbrok
//! @version 2.3.0
//! @brief
//-----------------------------------------------------------------------------

void push_stack(stack_t* Stack, type_data push_num);

//-----------------------------------------------------------------------------
//! @brief  Outputs all current stack information to a file
//! @param [in] Stack - pointer to stack_t struct
//! @author Hollbrok
//! @version 3.2.1.7
//! @brief
//-----------------------------------------------------------------------------

void stack_dump(stack_t* Stack);


//-----------------------------------------------------------------------------
//! @brief  Increase / decrease memory for storing data on the stack
//! @param [in] Stack - pointer to stack_t struct
//! @author Hollbrok
//! @version 2.2.2
//! @brief
//-----------------------------------------------------------------------------

void add_memory(stack_t* Stack);

//-----------------------------------------------------------------------------
//! @brief  Pops a number off the stack
//! @param [in] Stack - pointer to stack_t struct
//! @brief [out] number - top number on the stack
//! @author Hollbrok
//! @version 1.5.0
//! @brief
//-----------------------------------------------------------------------------

type_data pop_stack(stack_t* Stack);

//-----------------------------------------------------------------------------
//! @brief  Сhecks for the invalid stack
//! @param [in] Stack - pointer to stack_t struct
//! @brief [out] ERROR_STATE - zero if there are no errors, any number other than zero if there is anyone error
//! @author Hollbrok
//! @version 6.1.12
//! @brief
//-----------------------------------------------------------------------------

int stack_verify(stack_t* Stack);

//-----------------------------------------------------------------------------
//! @brief  calculates the hash of the stack
//! @param [in] Stack - pointer to stack_t struct
//! @brief [out] Hash - hash of the stack
//! @author Hollbrok
//! @version 1.0.5
//! @brief
//-----------------------------------------------------------------------------

int hash_stack(stack_t* Stack);

//-----------------------------------------------------------------------------
//! @brief  prints the error message, if there is an error
//! @brief [out] char* - ERROR message
//! @author Hollbrok
//! @version 1.1.1
//! @brief
//-----------------------------------------------------------------------------

char* error_print();

//-----------------------------------------------------------------------------
//! @brief  defines lvl of the security
//! @brief [out] char* - lvl of the security
//! @author Hollbrok
//! @version 1.1.1
//! @brief
//-----------------------------------------------------------------------------

char* define_lvl();

#endif
