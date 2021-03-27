#include "cpu.h"

static int SECOND_PRINT            = 0;

auto get_bytecode(FILE* text, Bytecode* byte_struct) -> void
{
    fseek(text, 0, SEEK_END);
    int file_length = ftell(text);
    fseek(text, 0, SEEK_SET);

    file_length++;

    char* buffer_char = (char*) calloc(file_length, sizeof(char));
    assert(buffer_char);

    fread(buffer_char, sizeof(char), file_length, text);

    int space_counter = 0;

    for(int index = 0; index < file_length; index++)
        if(buffer_char[index] == ' ')
            space_counter++;

    byte_struct->bytecode_capacity = space_counter;

    byte_struct->data = (double*) calloc(byte_struct->bytecode_capacity, sizeof(double));
    assert(byte_struct->data);

    //byte_struct->unknown_command = (char*) calloc(MAX_SIZE_COMMAND, sizeof(char));
    //assert(byte_struct->unknown_command);
    char* temp_pointer = buffer_char;
    int ass_cur_size = 0;
    while (*temp_pointer)
        byte_struct->data[ass_cur_size++] = get_number(&temp_pointer);

    free(buffer_char);
    buffer_char = nullptr;
    byte_struct->bytecode_capacity = ass_cur_size;
    return;
}

auto get_number(char** buffer) -> double
{
    ignore_spaces(buffer);
    double number = atof(*buffer);

    while (isdigit(**buffer) || (**buffer == '.') || (**buffer == '-'))
        (*buffer)++;

    ignore_spaces(buffer);
    return number;
}

auto ignore_spaces(char** buffer) -> void
{
    while (isspace(**buffer))
        (*buffer)++;
}

auto CPU(Bytecode* byte_struct, stack_t* Stack, stack_t* Stack_call) -> void
{
    assert(Stack);
    assert(Stack_call);
    assert(byte_struct);

    struct Rix rix_struct = {};

    using namespace my_commands;

    char* OP = (char*) calloc(OP_SIZE, sizeof(char));
    assert(OP);

	bool FIND_HLT = 0;

	check_assem_id

    else for (int i = 1; !FIND_HLT; i++)
    {
		int command = static_cast<int>(byte_struct->data[i]);

        if (get_byte(command, BIT_PUSH))	// (command == CMD_PUSH) // PUSH
        {
			byte_struct->error_state += cmd_push_exe(command, i, Stack, &rix_struct, byte_struct, OP);
			i++;
        }

        else if(get_byte(command, BIT_POP))	//(command == CMD_POP) //POP
        {
			byte_struct->error_state += cmd_pop_exe(command, i, Stack, &rix_struct, byte_struct, OP);
            i++;
        }

        else
        {
            switch(command)
            {
                case CMD_HLT:
                    FIND_HLT = TRUE;
                    break;
                case CMD_ADD:
					stack_size_check(2, not enough numbers in the stack. Exit..\n)
                    else
                        push_stack(Stack, pop_stack(Stack) + pop_stack(Stack));
                    break;
                case CMD_MUL:
					stack_size_check(2, not enough numbers to mul\n)
                    else
                    {
                        printf("in mul\n");
                        push_stack(Stack, pop_stack(Stack) * pop_stack(Stack));
                    }
                    break;
                case CMD_DIV:
                {
					stack_size_check(2, not enough numbers to div\n)
                    else
                    {
                        double x1 = pop_stack(Stack);
                        double x2 = pop_stack(Stack);
                        printf("x1 = [%lg], x2 = [%lg]\n", x1, x2);
                        push_stack(Stack, x2 / x1);
                    }
                    break;
                }
                case CMD_SUB:
                {
					stack_size_check(2, not enough numbers to sub\n)
                    {
                        double x1 = pop_stack(Stack);
                        double x2 = pop_stack(Stack);
                        push_stack(Stack, x2 - x1);
                    }
                    break;
                }
                case CMD_SIN:
                    push_stack(Stack, sin(pop_stack(Stack)));
                    break;
                case CMD_COS:
                    push_stack(Stack, cos(pop_stack(Stack)));
                    break;
                case CMD_POW:
                {
					stack_size_check(2, no numbers to pow\n)
                    {
                        double x1 = pop_stack(Stack);
                        double x2 = pop_stack(Stack);
                        push_stack(Stack, pow(x2, x1));
                    }
                    break;
                }
                case CMD_SQRT:
                    push_stack(Stack, sqrt(pop_stack(Stack)));
                    break;
                case CMD_IN:
                {
                    printf("Enter number:");
                    double x1;
                    scanf("%lg", &x1);
                    push_stack(Stack, x1);

                    break;
                }
                case CMD_OUT:
                {
					stack_size_check(1, not enough space in Stack to out. Exit..\n)
                    else
                    {
                        double x1 = pop_stack(Stack);

                        if(!SECOND_PRINT)
                        {
                            FILE* result = fopen("results[for user].txt", "wb");
                            SECOND_PRINT = TRUE;
                            fprintf(result, "[%lg]\n", x1);
                            fclose(result);
                        }

                        else
                        {
                            FILE* result = fopen("results[for user].txt", "a");
                            assert(result);
                            fprintf(result, "[%lg]\n", x1);
                            fclose(result);
                        }

                        push_stack(Stack, x1);
                    }
                    break;
                }
                case CMD_DEL:
                    pop_stack(Stack);
                    break;
                case CMD_LN:
                    push_stack(Stack, log(pop_stack(Stack)));
                    break;
                case CMD_LOG10:
                    push_stack(Stack, log10(pop_stack(Stack)));
                    break;
                case CMD_LOG2:
                    push_stack(Stack, log2(pop_stack(Stack)));
                    break;
                case CMD_CALL:
                    push_stack(Stack_call, i);
                    i = static_cast<int>(byte_struct->data[i + 1]);
                    break;
                case CMD_RET:
                    if(Stack_call->cur_size < 1)
                    {
                        printf("no number in stack_call to return\n");
                        byte_struct->error_state += ERROR_STK_CALL_SIZE;
                        break;
                    }

                    i = static_cast<int>(pop_stack(Stack_call)) + 1;

                    break;
				case CMD_LABEL:
                    break;
				case CMD_JE:
				case CMD_JAB:
				case CMD_JAE:
				case CMD_JBE:
				case CMD_JA:
 				case CMD_JB:
				case CMD_JMP:
				{
					byte_struct->error_state += cmd_compair_exe(command, Stack, byte_struct, &i);
					break;
				}

				case CMD_ABS:
					stack_size_check(1, not enough numbers to abs\n)
                    else
                        push_stack(Stack, abs(pop_stack(Stack)));
                    break;
                case CMD_DRAW:
					draw_screen(OP);
                    break;
                case CMD_FILL:
					fill_screen(OP);
                    break;
                case CMD_CIRC:
					draw_circ(OP, Stack, byte_struct);
                    break;
                case CMD_KOPM:
                    draw_kopm();
                    break;
                case CMD_CAT:
					draw_cat();
                    break;
                case CMD_MEM:
                    draw_mem();
                    break;
                default:
					byte_struct->error_state += ERROR_UNKNOWN_COM;
					byte_struct->unknown_command = command;

					break;
            }
        }
    }


	determine_status(byte_struct, Stack);
    free(OP);
    OP = nullptr;
}

auto bytecode_destruct(Bytecode* byte_struct) -> void
{
    assert(byte_struct);

    free(byte_struct->data);
    byte_struct->data = nullptr;

    //free(byte_struct->unknown_command);
    //byte_struct->unknown_command = nullptr;

    byte_struct->bytecode_capacity = 0;
}

auto print_for_user(stack_t* Stack) -> void
{
    FILE* result = fopen("results[for user].txt", "wb");

    if(Stack->cur_size > 0)
    {
        int Size = Stack->cur_size;
        int max_order = 0;

        while(Size > 9)
        {
            Size /= 10;
            max_order++;
        }

        for(int i = 0; i < Stack->cur_size; i++)
        {

            int order  = 0;
            int i_copy = i + 1;

            while(i_copy > 9)
            {
                i_copy /= 10;
                order++;
            }

            switch(order)
            {
                case 0:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }

                case 1:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 1; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }

                case 2:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 2; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                case 3:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 3; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                case 4:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 4; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "w");
                    fprintf(error, "Too many numbers in stack\n");
                    fclose(error);
                }
            }
        }
    }
    else
		fprintf(result, "Stack size is below than 1\n"
                         "Currently size of Stack is %d", Stack->cur_size);

    fclose(result);
}

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;

    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

auto define_date() -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}

auto is_equal(double a, double b) -> bool
{
	double diff = fabs(a - b);

	if (diff <= absEpsilon)
		return true;

	return diff <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

auto fill_screen(char* OP) -> void
{
	system("cls");
    txCreateWindow(SIZEX, SIZEY);
    txSetDefaults();

    for (int y = 0; y < SIZEY; y++)
        for (int x = 0; x < SIZEX; x++)
    		txSetPixel(x, y, RGB(OP[0], OP[1], OP[2]));

	return;
}

auto draw_screen(char* OP) -> void
{
	system("cls");
    txCreateWindow(SIZEX, SIZEY);
    txSetDefaults();

    for (int y = 0; y < SIZEY; y++)
        for (int x = 0; x < SIZEX; x++)
            txSetPixel(x, y, RGB(OP[3 * (SIZEX * y + x)], OP[3 * (SIZEX * y + x) + 1], OP[3 * (SIZEX * y + x) + 2]));

	return;
}

auto draw_circ(char* OP, stack_t* Stack, Bytecode* byte_struct) -> void
{
	system("cls");

    using namespace my_commands;
	stack_size_check(3, not enough numbers to circ\n)
	else
	{
		int Radius   = static_cast<int>(pop_stack(Stack));
    	int y_center = static_cast<int>(pop_stack(Stack));
    	int x_center = static_cast<int>(pop_stack(Stack));

		for (int y = 0; y < SIZEY; y++)
        	for (int x = 0; x < SIZEX; x++)
            	if (( pow((x - x_center), 2) + pow((y - y_center), 2) ) < pow(Radius, 2))
                	txSetPixel(x, y, RGB(OP[3], OP[4], OP[5]));
	}

	return;
}

auto draw_kopm() -> void
{
	system("cls");
    txCreateWindow(SIZEX, SIZEY);
    txSetDefaults();

    txSetFillColor(RGB(100, 100, 230));
    txFloodFill(5, 5);
    txTextOut(100, 145, "KOPM ONE LOVE");

    txSetColor(RGB(0, 0, 0), 1);
    txLine(175, 160, 165, 145);
    txLine(175, 160, 185, 145);
    txArc(165, 140, 175, 150, 0, 182);
    txArc(175, 140, 185, 150, 0, 182);

    txSetColor(RGB(237, 19, 19), 1);
    txLine(215, 140, 205, 125);
    txLine(215, 140, 225, 125);
    txArc(205, 120, 215, 130, 0, 182);
    txArc(215, 120, 225, 130, 0, 182);

    txSetColor(RGB(48, 217, 65), 1);
    txLine(115, 100, 105, 85);
    txLine(115, 100, 125, 85);
    txArc(105, 80, 115, 90, 0, 182);
    txArc(115, 80, 125, 90, 0, 182);

	return;
}

auto draw_cat() -> void
{
	system("cls");
    txCreateWindow(SIZEX, SIZEY);
    txSetDefaults();

    txSetFillColor(RGB(0, 100, 167));
    txFloodFill(5 , 5);

    txSetFillColor(RGB(125, 125, 125)); // öâåò òåëà
    txEllipse(100, 70, 180, 180);       // ðèñóåì òåëî

    txSetFillColor(RGB(0, 0, 0));       // ÷åðíûé öâåò
    txEllipse(130, 90, 160, 150);       // ðèñóåâ âíóòðè òóëîâèùà

    txSetFillColor(RGB(154, 148, 148)); // öâåò ãîëîâû
    txCircle(140, 45, 25);              // ãîëîâà

    txSetColor(RGB(0, 0, 0), 1);        // öâåò è òîëùèíà óñèêîâ
    txLine(140, 55, 170, 45);           // ñàìè óñèêè
    txLine(140, 55, 175, 53);
    txLine(140, 55, 181, 61);
    txLine(140, 55, 120, 45);
    txLine(140, 55, 115, 53);
    txLine(140, 55, 109, 61);

    txSetFillColor(RGB(255, 255, 255)); // Öâåò ãëàç (áåëûé)
    txEllipse(130, 35, 141, 50);        // ãëàç Ë
    txEllipse(145, 35, 156, 50);        // ãëàç Ï

    txLine(120, 30, 125, 10);           // óõî Ë
    txLine(125, 10, 133, 23);

    txSetFillColor(RGB(154, 148, 148)); // öâåò çàëèâêè óõà
    txFloodFill(125, 12);               // çàëèâàåì óõî

    txLine(150, 22, 158, 10);           // óõî Ï
    txLine(158, 10, 160, 32);

    txSetFillColor(RGB(154, 148, 148)); // öâåò çàëèâêè óõà
    txFloodFill(157, 17);               // çàëèâàåì óõî

    txSetFillColor(RGB(0, 0, 0));       // öâåò çàëèâêè çðà÷êà
    txEllipse(132, 39, 137, 48);        // çðà÷îê Ë
    txEllipse(147, 39, 152, 48);        // çðà÷îê Ï
    txTextOut(180, 45, "Meow-meow!");

	return;
}

auto draw_mem() -> void
{
	system("cls");
    txCreateWindow(SIZEX, SIZEY);
    txSetDefaults();

    HDC mem = txLoadImage("Mem.bmp");
    assert(mem);
    txBitBlt(txDC(), 0, 0, SIZEX, SIZEY, mem, 0, 0);

    srand(time(nullptr));
    int number =  rand() % 6;

	using namespace memes_names;

    switch(number)
    {
        case DED_CODESTYLE:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "This guy didn't");
            txTextOut(6, 123, " write the code");

            txTextOut(160, 110, "Everything is ");
            txTextOut(160, 123, "in one file here.");

            txTextOut(8, 262, "This has no func-");
            txTextOut(8, 275, "tions at all!");

            txTextOut(160, 262, "Well fuck, let's ");
            txTextOut(160, 275, "drop this project");

            break;
        case DED_ILAB_CLASSIC:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "No one answered");
            txTextOut(6, 123, "\"yes\" or \"understand\"");

            txTextOut(160, 110, "No questions asked");
            txTextOut(160, 123, "at all");

            txTextOut(8, 262, "Sunday is a ");
            txTextOut(8, 275, "very busy day");

            txTextOut(160, 262, "Most likely I will ");
            txTextOut(160, 275, "have to cancel ILAB");

            break;
        case CAT:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "Poltorashka ran away");
            txTextOut(6, 123, " when I stroked her");

            txTextOut(160, 110, "Physical education 0 ");
            txTextOut(160, 123, "visits for Semak");

            txTextOut(8, 262, "BRS is not much ");
            txTextOut(8, 275, "more than visits");

            txTextOut(160, 262, "I go to the academy, ");
            txTextOut(160, 275, "and I'll stroke the cats");

            break;
        case BOMONKA:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "I entered to the");
            txTextOut(6, 123, " MIPT to get drunk");

            txTextOut(160, 110, "These guys didnt call");
            txTextOut(160, 123, "me to nk on friday");

            txTextOut(8, 262, "These smokes have gone ");
            txTextOut(8, 275, "without you for the umpteenth time");

            txTextOut(160, 262, "Well fuck, I'd ra-");
            txTextOut(160, 275, "ther go to the Bomonka");

            break;
        case DED_HOHLOV:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "I went to ded");
            txTextOut(6, 123, "to coding");

            txTextOut(160, 110, "There is not enough");
            txTextOut(160, 123, " time for matan");

            txTextOut(8, 262, "I almost never had time");
            txTextOut(8, 275, "to go to physos");

            txTextOut(160, 262, "Well fuck, I'll ");
            txTextOut(160, 275, "transfer to Khokhlov");

            break;
        case CODING_BUGS:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "You are writing");
            txTextOut(6, 123, " a project on ILAB");

            txTextOut(160, 110, "There are a couple");
            txTextOut(160, 123, " of hundred bugs");

            txTextOut(8, 262, "For some reason, nothing");
            txTextOut(8, 275, " else works there.");

            txTextOut(160, 262, "Well fuck it, ");
            txTextOut(160, 275, "I'm leaving with ILAB-");

            break;
        default:
            txSetColor(RGB(0, 0, 0));

            txTextOut(6, 110, "ERROR");
            txTextOut(6, 123, "ERROR");

            txTextOut(160, 110, "ERROR");
            txTextOut(160, 123, "ERROR");

            txTextOut(8, 262, "ERROR");
            txTextOut(8, 275, "ERROR");

            txTextOut(160, 262, "ERROR");
            txTextOut(160, 275, "ERROR");
            break;

    }

	return;
}

auto get_byte(int digit, int number_of_bit) -> bool
{
    return (bool((1 << (number_of_bit - 1))  &  digit));
}

auto cmd_push_exe(int command, int i, stack_t* Stack, Rix* rix_struct, Bytecode* byte_struct, char* OP) -> ERROR_STATE
{
    using namespace my_commands;

	if (!get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER))
			push_stack(Stack, byte_struct->data[i + 1]);
		else if (!get_byte(command, BIT_NUMBER))
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
                //printf("���� � ���� rax = [%lg]\n", rix_struct->rax);
				push_stack(Stack, rix_struct->rax);
			}
			else if (get_byte(rix_number, BIT_RBX))
				push_stack(Stack, rix_struct->rbx);
			else if (get_byte(rix_number, BIT_RCX))
				push_stack(Stack, rix_struct->rcx);
			else if (get_byte(rix_number, BIT_RDX))
				push_stack(Stack, rix_struct->rdx);
		}
	}
				//* Проверки на значение в rix, чтобы не было OP[-1]
	else if (get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			double *temp_pointer = (double*) (OP + static_cast<int>(byte_struct->data[i + 1]));
        	push_stack(Stack, *temp_pointer);
		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rax));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rbx));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rcx));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rdx));
            	push_stack(Stack, *temp_pointer);
            }
		}
	}
		//* Проверки на значение в rix, чтобы не было OP[-1]
	else if (get_byte(command, BIT_C_OP) && !get_byte(command, BIT_D_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			char *temp_pointer = (char*) (OP + static_cast<int>(byte_struct->data[i + 1]));
            push_stack(Stack, *temp_pointer);
		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rax));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rbx));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rcx));
            	push_stack(Stack, *temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rdx));
            	push_stack(Stack, *temp_pointer);
            }
		}
	}
	else
		return ERROR_CMD_PUSH;

	return 0;
}

auto cmd_pop_exe(int command, int i, stack_t *Stack, Rix *rix_struct, Bytecode *byte_struct, char *OP) -> ERROR_STATE
{
    using namespace my_commands;

	stack_size_check(1, not enough numbers to pop\n)

    else if (!get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER))
			pop_stack(Stack);
		else if (!get_byte(command, BIT_NUMBER))
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
                //printf("rax before = [%lg]\n", rix_struct->rax);
				rix_struct->rax = pop_stack(Stack);
				//printf("rax after = [%lg]\n", rix_struct->rax);
			}
			else if (get_byte(rix_number, BIT_RBX))
				rix_struct->rbx = pop_stack(Stack);
			else if (get_byte(rix_number, BIT_RCX))
				rix_struct->rcx = pop_stack(Stack);
			else if (get_byte(rix_number, BIT_RDX))
				rix_struct->rdx = pop_stack(Stack);
		}
	}

	else if (get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			double* temp_pointer = (double*) (OP + static_cast<int>(byte_struct->data[i + 1]));
            *temp_pointer = static_cast<double> (pop_stack(Stack));

		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
                //if(rix_struct->rax < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rax));
                    *temp_pointer      = static_cast<double> (pop_stack(Stack));
                }
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
                //if(rix_struct->rbx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rbx));
                    *temp_pointer      = static_cast<double> (pop_stack(Stack));
                }
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
                //if(rix_struct->rcx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rcx));
                    *temp_pointer      = static_cast<double> (pop_stack(Stack));
                }
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
                //if(rix_struct->rdx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rdx));
                    *temp_pointer      = static_cast<double> (pop_stack(Stack));
                }
            }
		}
	}

	else if (!get_byte(command, BIT_D_OP) && get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			char* temp_pointer = static_cast<char*> (OP + static_cast<int>(byte_struct->data[i + 1]));
            *temp_pointer = static_cast<char> (pop_stack(Stack));

		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(byte_struct->data[i + 1]);

            if(rix_struct->rdx < 0)
            {
                //printf("BAAAD rdx in pop = %lg\n", rix_struct->rdx);
                //EXIT_CONDITION = true;
            }

			else if (get_byte(rix_number, BIT_RAX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rax));
            	*temp_pointer      = static_cast<char> (pop_stack(Stack));
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rbx));
            	*temp_pointer      = static_cast<char> (pop_stack(Stack));
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rcx));
            	*temp_pointer      = static_cast<char> (pop_stack(Stack));
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
                //printf("POP rdx = %lg\n", rix_struct->rdx);
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rdx));
            	*temp_pointer      = static_cast<char> (pop_stack(Stack));
            }
		}
	}
	else
		return ERROR_CMD_POP;

	return 0;
}

auto cmd_compair_exe(int command, stack_t* Stack, Bytecode* byte_struct, int *i) -> ERROR_STATE
{
    using namespace my_commands;
    //printf("in compair_exe i = %d\n", *i);

	if((Stack->cur_size < 2) && !get_byte(command, BIT_JUMP))
    {
        //printf("not enough numbers to do contidional jump or common jump\n");
        //EXIT_CONDITION = true;
    }
    else if (get_byte(command, BIT_JUMP))
    {
        (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
    }
	else
	{
        double x1 = pop_stack(Stack);
        double x2 = pop_stack(Stack);

        if (get_byte(command, BIT_EQUAL))
        {
            //printf("BIT EQUAL\n");
            if (get_byte(command, BIT_BELOW) && !get_byte(command, BIT_ABOVE)) 		// <=
            {
                //printf("JBE\n");
                if (x2 <= x1)
                    (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
                else (*i)++;
            }
            else if (!get_byte(command, BIT_BELOW) && get_byte(command, BIT_ABOVE)) // >=
            {
                if (x2 >= x1)
                    (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
                else (*i)++;
            }
            else if (!get_byte(command, BIT_BELOW) && !get_byte(command, BIT_ABOVE)) // ==
            {
                if (is_equal(x2,  x1))
                    (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
                else (*i)++;
            }
            else if  (get_byte(command, BIT_BELOW) && get_byte(command, BIT_ABOVE)) // !=
            {
                if (!is_equal(x2, x1))
                    (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
                else (*i)++;
			}
        }
        else if (get_byte(command, BIT_ABOVE))
        {
            if (x2 > x1)
                (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
            else (*i)++;
        }
        else if (get_byte(command, BIT_BELOW))
        {
            if (x2 < x1)
                (*i) = static_cast<int>(byte_struct->data[(*i) + 1]);
            else (*i)++;
        }
        else
			return ERROR_COMPAIR;
    }
	return 0;
}

auto determine_status(Bytecode* byte_struct, stack_t* Stack) -> void
{
	using namespace my_commands;

	FILE* error = fopen("[!]DUMP_CPU.txt", "w");
	assert(error);

    fprintf(error, "\tData of DUMP in CPU : %s (dd/mm/yy)\n\n", define_date());

	if(!byte_struct->error_state)
		print_good(error);
	else
		for(int i = 0; i < NUMBER_OF_ERRORS; i++)
			if(get_byte(byte_struct->error_state, i + 1))
				print_error(error, i + 1);

    if(Stack->cur_size == 0)
        fprintf(error, "Stack is empty\n");
	else
	{
        fprintf(error, "Numbers in Stack:\n");
        for(int i = 0; i < Stack->cur_size; i++)
            fprintf(error, "%d.[%lg]\n", i + 1, Stack->data[i]);
    }
    fclose(error);
	return;
}

auto print_good(FILE* error) -> void
{
	assert(error);

	fprintf(error, "CPU ended without any errors\n");

	return;
}

auto print_error(FILE* error, int error_bit) -> void
{
    using namespace my_commands;
	switch (error_bit)
    {
        case ID_ID:
            fprintf(error, "Unknown CPU ID. Unfortunately, this processor can only process the code of a certain assembler\n");
            break;
        case ID_STACK_SIZE:
            fprintf(error, "At some point in the program, you tried to take more from the stack than it can\n");
            fprintf(error, "For example: Stack size is 1, but u tried to do \"add\" or \"mul\", etc. \n");
            break;
        case ID_UNKNOWN_COM:
            fprintf(error, "The processor did not recognize one of the commands\n");
            break;
        case ID_CMD_PUSH:
            fprintf(error, "Something went wrong in processing command \"push\"\n");
            break;
        case ID_CMD_POP:
            fprintf(error, "Something went wrong in processing command \"pop\"\n");
            break;
        case ID_COMPAIR:
            fprintf(error, "Something went wrong in any type of compair. For example jbe, jab, jump, etc.\n");
            break;
        case ID_STK_CALL_SIZE:
            fprintf(error, "No number in stack_call to return\n");
            break;
        default:
            fprintf(error, "Error in ERRORS. IT means that program has unknown error\n");
            break;
	}
}

