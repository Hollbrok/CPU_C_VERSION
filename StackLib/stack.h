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

#define ASSERT_OK            	\
	if (verify())		 		\
	{                        	\
		printf("Error in verify\n");\
		dump();   				\
		return;              	\
	}

#define ASSERT_POP_OK        	\
	if (verify()) 				\
	{                        	\
		printf("Error in pop\n");\
		dump();   				\
		return POISON;       	\
	}

typedef unsigned int uint32_t;

class Stack
{
private:
	int canary_left_;

	data_type *data_;       //= nullptr;
	const char *name_;      //= nullptr;
	size_t capacity_;       //= 0;         //max size
	size_t cur_size_;       //= 0;         //size now
	uint32_t hash_;		    //= 0;
	int error_state_;

	int canary_right_;

	int verify();
	uint32_t calc_hash();
	char *error_print(int bit_of_error);
	void add_memory();
	void reduce_memory();

public:
	Stack(const char *name = "Default", size_t capacity = 0);
	~Stack();

	void dump();
	void push(data_type push_num);
	data_type pop();

};

auto get_byte(int digit, int number_of_bit) -> bool;

#endif
