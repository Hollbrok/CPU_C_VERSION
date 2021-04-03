#include "stack.h"

int ERROR_STATE                  = 0;
int DOUBLE_CONSTRUCT             = 0;
char* addres                     = "log_stack.txt";

Stack::Stack(const char *name, size_t capacity) :
	canary_left_(( CANARY_L_STACK )),
	data_(nullptr),
	name_(name),
	capacity_(capacity),
	cur_size_(0),
	hash_(0),
	error_state_(0),
	canary_right_(( CANARY_R_STACK ))
{
	assert(this && "You passed nullptr to stack_consturct");

    FILE* res = fopen("log_stack.txt", "wb");
    assert(res && "Can't open log_stack.txt");
    fclose(res);

	try
	{
		data_ = new data_type [capacity_ + 2];
	}
	catch(const char &e)
	{
		printf("Allocation failed\n");
		return;
	}
	catch(...)
	{
        printf("Caught unknown exception.");
	}


    //data_ = (data_type*) calloc(capacity + 2, sizeof(data_type));
	//assert(data_ && "Can't calloc memory for data_\n");

    data_[0] = static_cast<data_type> ( CANARY_LEFT_DATA );
    data_[capacity + 1] = static_cast<data_type> ( CANARY_RIGHT_DATA );

    for(int i = 1; i < capacity + 1; i++)
        data_[i] = POISON;

    data_++;

    hash_ = calc_hash();

}

Stack::~Stack()
{
	assert(this && "Stack is nullptr in ~Stack\n");

    for(int i = -1; i <= capacity_; i++)
    	data_[i] = POISON;

    data_type* pointer =(data_type*) ((char*)data_ - sizeof(data_type));
    delete[] pointer;

    data_ = nullptr;

    cur_size_ = -1;
    capacity_ = -1;

}

void Stack::push(data_type push_num)
{
    assert((push_num != NAN) && "You passed incorrect number to push_stack");
    assert(this && "You passed nullptr to push");

	ASSERT_OK

    FILE* res = fopen("log_stack.txt", "ab");
	assert(res && "can't open log_stack.txt\n");

    if(cur_size_ == capacity_)
    {
		add_memory();
        data_[cur_size_++] = push_num;
        hash_ = calc_hash();
    }
    else
    {
        data_[cur_size_++] = push_num;
        hash_ = calc_hash();
    }

	fclose(res);
    return;
}

void Stack::add_memory()
{
    using namespace my_errors;
	assert(this && "You passed nullptr to add memory");

	ASSERT_OK

	if(capacity_ == 0)
	{
		capacity_ = REAL_START_SIZE;

		data_type* temp_data = data_ - 1;

		try
		{
			data_ = new data_type[capacity_ + 2];
		}
		catch(const char &e)
		{
			printf("Allocation failed\n");
			return;
		}
		catch(...)
        {
            printf("Caught unknown exception.");
        }

		data_[0] = static_cast<data_type> ( CANARY_LEFT_DATA );
    	data_[capacity_ + 1] = static_cast<data_type> ( CANARY_RIGHT_DATA );

    	for(int i = 1; i < capacity_ + 1; i++)
        	data_[i] = POISON;

    	data_++;

		delete[] temp_data;
		hash_ = calc_hash();

	}
	else if (cur_size_ == capacity_)
	{
		capacity_ *= 2;

		data_type* temp_data = data_ - 1;

		try
		{
			data_ = new data_type [capacity_ + 2];
		}
		catch(const char &e)
		{
			printf("Allocation failed\n");
			return;
		}
		catch(...)
        {
            printf("Caught unknown exception.");
        }

		data_[0] = static_cast<data_type> ( CANARY_LEFT_DATA );
    	data_[capacity_ + 1] = static_cast<data_type> ( CANARY_RIGHT_DATA );

		for(int i = 1; i < cur_size_ + 1; i++)
			data_[i] = temp_data[i];
		for(int i = cur_size_ + 1; i < capacity_ + 1; i++)
			data_[i] = POISON;

		data_++;

        hash_ = calc_hash();
		delete[] temp_data;
	}
	else
	{
		printf("Error in add_memory\n");
		error_state_ += REALLOC_ERROR;
	}

}

data_type Stack::pop()
{
	assert(this && "You passed nullptr to pop");

    ASSERT_POP_OK

    if ((cur_size_ <= (capacity_ / REAL_REDUCER + 1)) && (cur_size_ > REAL_START_SIZE))
    {
        reduce_memory();
		cur_size_--;
        data_type temp = data_[cur_size_];
        data_[cur_size_] = POISON;

		hash_ = calc_hash();
        return temp;
    }

    else if (cur_size_ > 0)
    {
		cur_size_--;
        data_type temp = data_[cur_size_];
        data_[cur_size_] = POISON;

        hash_ = calc_hash();
        return temp;
    }
    printf("Return POISON\n");
    return  POISON;
}

void Stack::reduce_memory()
{
    using namespace my_errors;
	assert(this && "You passed nullptr to reduce_memory");

	if(cur_size_ <= (capacity_ / REAL_REDUCER + 1))
	{
        capacity_ /= 2;
		data_type* temp_data = data_ - 1;

		try
		{
			data_ = new data_type [capacity_ + 2];
		}
		catch(const char &e)
		{
			printf("Allocation failed\n");
			return;
		}
		catch(...)
        {
            printf("Caught unknown exception.");
        }

		data_[0] = CANARY_LEFT_DATA;
		data_[capacity_ + 1] = CANARY_RIGHT_DATA;

		for(int i = 0; i < capacity_ + 1; i++)
			data_[i] = temp_data[i];

        data_++;

        hash_ = calc_hash();
		delete[] temp_data;
    }
	else
	{
		printf("Error in reduce_memory\n");
		error_state_ +=	REALLOC_ERROR;
	}
}

uint32_t Stack::calc_hash()
{
    assert(this && "You passed nullptr to calc_hash");

    uint32_t total_hash = 0;

    uint32_t i_1 = 0;
    uint32_t hash_1 = 0;

    while (i_1 != cur_size_)
    {
        hash_1 += data_[i_1++];
        hash_1 += hash_1 << 10;
        hash_1 ^= hash_1 >> 6;
    }

    hash_1 += hash_1 << 3;
    hash_1 ^= hash_1 >> 11;
    hash_1 += hash_1 << 15;

    total_hash += hash_1;


    uint32_t i_2 = 0;
    uint32_t hash_2 = 0;

    while (i_2 != strlen(name_))
    {
        hash_2 += name_[i_2++];
        hash_2 += hash_1 << 10;
        hash_2 ^= hash_1 >> 6;
    }

    hash_2 += hash_2 << 3;
    hash_2 ^= hash_2 >> 11;
    hash_2 += hash_2 << 15;

    total_hash += hash_2;

    return total_hash;
}

void Stack::dump()
{
	assert(this && "You passed nullptr to dump");

    char mass[67] = "******************************************************************";

    FILE* res = fopen("log_stack.txt", "ab");
    assert(res && "can't open file log_stack.txt");
    fprintf(res, "\n%*s\n", 66, mass);

	using namespace my_errors;

    if(error_state_)
    {
        printf("error_state_ = %d\n", error_state_);
		for(int i = 0; i < NUMBER_OF_ERRORS; i++)
			if(get_byte(error_state_, i + 1))
				fprintf(res, "Stack (ERROR #%d : %s) [%p]. \n",
				error_state_, error_print(i + 1), this);
    }
    else
    {
        fprintf(res, "Stack(OK) [%p]. \"%s\"\n", this, name_);

        // fprintf(res, "Security lvl is %s\n", sec_lvl);
        // fprintf(res, "Type of data is %s\n", type_string);
        fprintf(res, "Hash        = %u\n", hash_);
        fprintf(res, "size        = %d\n", cur_size_);
        fprintf(res, "capacity    = %d\n", capacity_);
        int cap = capacity_;
        int cur = cur_size_;

        for(int i = 0; i < cap; i++)
        {
            if(i < cur)
                fprintf(res, "*[%d] data   = %lg\n", i, data_[i]);
            else
                fprintf(res, "*[%d] data   = %lg (POISON)\n", i, POISON);
        }
    }


    fprintf(res, "%*s\n\n", 66, mass);
    fclose(res);

    return;
}

int Stack::verify()
{
    assert(this && "You passed nullptr to verify");
    int hash_st = hash_;

    using namespace my_errors;

    if(ERROR_STATE == MAX_CAPACITY_ERROR)
    {
		error_state_ += MAX_CAPACITY_ERROR;
        return MAX_CAPACITY_ERROR;
    }

    else if((this == nullptr))
    {
        error_state_ += NULL_STACK_PTR;
        return NULL_STACK_PTR;
    }

    else if(hash_st != calc_hash())
    {
        error_state_ += HACK_STACK;
        return HACK_STACK;
    }

    else if(data_ == nullptr)
    {
        error_state_ += NULL_DATA_PTR;
        return NULL_DATA_PTR;
    }

    else if(cur_size_ > capacity_)
    {
        error_state_ += CUR_BIGGER_CAPACITY;
        return CUR_BIGGER_CAPACITY;
    }

    else if(cur_size_ < 0)
    {
        error_state_ += CUR_LESS_ZERO;
        return CUR_LESS_ZERO;

    }
    else if(data_[-1] != CANARY_LEFT_DATA)
	{
        error_state_ += ERROR_DATA_LEFT;
        return ERROR_DATA_LEFT;
    }

    else if(data_[capacity_] != CANARY_RIGHT_DATA)
    {
        error_state_ += ERROR_DATA_RIGHT;
        return ERROR_DATA_RIGHT;
    }

    else if(canary_left_ !=  ( CANARY_L_STACK) )
    {
        error_state_ += ERROR_STACK_LEFT;
        return ERROR_STACK_LEFT;
    }

    else if(canary_right_ != ( CANARY_R_STACK ) )
    {
        error_state_ += ERROR_STACK_RIGHT;
        return ERROR_STACK_RIGHT;
    }

    else
        return false;
}

char* Stack::error_print(int bit_of_error)
{
    switch(bit_of_error)
    {
        case 1:
            return "NULL STACK PTR";
        case 2:
            return "NULL DATA PTR";
        case 3:
            return "CUR BIGGER THAN CAPACITY";
        case 4:
            return "CUR LESS THAN ZERO";
        case 5:
            return "CAPACITY LESS THAN ZERO";;
        case 6:
            return "DOUBLE CONSTRUCT";
        case 7:
            return "ERROR_DATA_LEFT";
        case 8:
            return "ERROR_DATA_RIGHT";
		case 9:
            return "Somebody is trying to hack a stack";
        case 10:
            return "MAX_CAPACITY_ERROR";
        default:
            return "UNKNOWN ERROR IN error_print()\n";

    }
}

auto get_byte(int digit, int number_of_bit) -> bool
{
    return (bool((1 << (number_of_bit - 1))  &  digit));
}
