#ifndef STACK_CONFIG_H_INCLUDED
#define STACK_CONFIG_H_INCLUDED

using data_type = double;

constexpr int REAL_MULTIPLIER	= 2;
constexpr int REAL_REDUCER   	= 2 * REAL_MULTIPLIER;
constexpr int REAL_START_SIZE  	= 4;

constexpr int CANARY_L_STACK           	= 0x1A3C3D7C;
constexpr int CANARY_R_STACK       		= 0xADF44A23;

constexpr data_type CANARY_LEFT_DATA    = 0x1BF3334A;
constexpr data_type CANARY_RIGHT_DATA  	= 0xFDF4223A;

const data_type POISON    = NAN;


namespace my_errors
{
	enum Errors
	{
     	NULL_STACK_PTR      = 1,
     	NULL_DATA_PTR       = 2,
     	CUR_BIGGER_CAPACITY = 4,
     	CUR_LESS_ZERO       = 8,
     	ERROR_DATA_LEFT     = 16,
     	ERROR_DATA_RIGHT    = 32,
     	ERROR_STACK_LEFT    = 64,
     	ERROR_STACK_RIGHT   = 128,
     	HACK_STACK          = 256,
     	MAX_CAPACITY_ERROR  = 512,
		REALLOC_ERROR		= 1024
	};

	enum Error_bits
	{
		BIT_NULL_STACK_PTR      = 1,
     	BIT_NULL_DATA_PTR       = 2,
     	BIT_CUR_BIGGER_CAPACITY = 3,
     	BIT_CUR_LESS_ZERO       = 4,
     	BIT_ERROR_DATA_LEFT     = 5,
     	BIT_ERROR_DATA_RIGHT    = 6,
     	BIT_ERROR_STACK_LEFT    = 7,
     	BIT_ERROR_STACK_RIGHT   = 8,
     	BIT_HACK_STACK          = 9,
     	BIT_MAX_CAPACITY_ERROR  = 10,
		BIT_REALLOC_ERROR		= 11,

	 	NUMBER_OF_ERRORS    	= 11,
	};
}




#endif
