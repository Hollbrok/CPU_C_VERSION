#include "assem.h"

static int NEW_COMMAND_ERROR = 0; // If unknown command -- leave from assembling
static int IS_LAST_COMMAND_PUSH = 0;
static int IS_LAST_COMMAND_JMP = 0;

auto text_construct(Text *text_struct, FILE *user_code) -> void
{

	long file_length = size_of_file(user_code);

	text_struct->data = (char *)calloc(file_length, sizeof(char));
	assert(text_struct->data);

	int file_lines = 0;
	file_length = 0;

	useful_sizes(user_code, text_struct, &file_lines, &file_length);

	text_struct->lines = (Line *)calloc(file_lines + 1, sizeof(Line));
	assert(text_struct->lines);

	Line *lines = (Line *)calloc(file_lines + 1, sizeof(Line));
	assert(lines);

	long length = 0;
	long n_structs = 0;

	while (true)
	{
		int iter_length = 0;

		while (isspace(text_struct->data[length]))
			length++;

		if (length >= file_length)
			break;

		lines[n_structs].line = &(text_struct->data)[length];

		while ((text_struct->data[length] != '\n') && (length < file_length))
		{
			if ((text_struct->data[length] == ';'))
			{
				while (text_struct->data[length] != '\n')
					length++;
				break;
			}
			length++;
			iter_length++;
		}

		lines[n_structs].length = iter_length;
		while (isspace(lines[n_structs].line[lines[n_structs].length - 1]))
			(lines[n_structs].length)--;

		n_structs++;
	}

	for (int j = 0; j < n_structs; j++)
	{
		text_struct->lines[j].line = lines[j].line;
		text_struct->lines[j].length = lines[j].length;
	}

	text_struct->n_struct = n_structs;
	text_struct->length_file = file_length;

	free(lines);
	lines = nullptr;

	FILE *info = fopen("[!]info.txt", "w");
	assert(info);

	fprintf(info, "Number of lines      = %d\n", text_struct->n_struct);
	fprintf(info, "Length of the file   = %d\n", text_struct->length_file);

	for (int x = 0; x < text_struct->n_struct; x++)
	{
		fprintf(info, "lines[%d]. [", x + 1);

		for (int y = 0; y < text_struct->lines[x].length; y++)
			fprintf(info, "%c", text_struct->lines[x].line[y]);

		fprintf(info, "]\n");
		fprintf(info, "\tlength = %ld\n\n", text_struct->lines[x].length);
	}

	//print_text_struct(text_struct);        // NEED FOR DEBUG
	fclose(info);
}

auto text_destruct(Text *text_struct) -> void // FOR DEBUG
{
	free(text_struct->data);
	text_struct->data = nullptr;

	free(text_struct->lines);
	text_struct->lines = nullptr;
}

auto print_text_struct(Text *text_struct) -> void // FOR DEBUG
{
	assert(text_struct);

	FILE *res = fopen("[!]string_text.txt", "w");

	for (int i = 0; i < text_struct->n_struct; i++)
	{
		for (int k = 0; k < text_struct->lines[i].length; k++)
			fprintf(res, "%c", text_struct->lines[i].line[k]);
		fprintf(res, " ");
	}

	fclose(res);
}

auto code_construct(Text *text_struct, Code *code_struct) -> void
{
	assert(text_struct);
	assert(code_struct);

	code_struct->length = text_struct->length_file + 3; // may be without + 3

	code_struct->data = (char *)calloc(code_struct->length, sizeof(char));
	assert(code_struct->data);

	int cur_size = 0;

	for (int x = 0; x < text_struct->n_struct; x++)
	{
		for (int y = 0; y < text_struct->lines[x].length; y++)
		{
			code_struct->data[cur_size++] = text_struct->lines[x].line[y];
			if (isspace(text_struct->lines[x].line[y]))
				while (isspace(text_struct->lines[x].line[y + 1]))
					y++;
		}

		if ((cur_size < code_struct->length + 1))
			code_struct->data[cur_size++] = ' ';
	}

	code_struct->data[cur_size] = '\0';
	int terms = 0;
	code_struct->terms = terms;

	for (int i = 0; i < code_struct->length; i++)
		if (code_struct->data[i] == ' ')
		{
			code_struct->terms++;
			while (isspace(code_struct->data[i]))
				i++;
		}

	print_code_buffer(code_struct);
	return;
}

auto code_destruct(Code *code_struct) -> void
{
	assert(code_struct);

	free(code_struct->data);
	code_struct->data = nullptr;
}

auto print_code_buffer(Code *code_struct) -> void
{
	assert(code_struct);
	FILE *code_text = fopen("[!]code_struct.txt", "w");

	fwrite(code_struct->data, sizeof(char), strlen(code_struct->data), code_text);

	return;
}

auto get_bytecode(Code *code_struct, Bytecode *byte_struct) -> void
{
	assert(code_struct);
	assert(byte_struct);
	using namespace my_commands;

	Label *labels = (Label *)calloc(MAX_LABELS + 1, sizeof(Label));
	assert(labels);

	int amount_labels = get_labels(labels, code_struct);

	byte_struct->data = (double *)calloc(code_struct->terms, sizeof(double));
	assert(byte_struct->data);

	double *specifiers = (double *)calloc(code_struct->terms, sizeof(double));
	assert(specifiers);

	int flags_size = 0;
	code_struct->cur_size = 0;

	char *temp = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
	assert(temp);

	for (int i = 0; i < code_struct->terms; i++)
	{
		if (byte_struct->error_state)
		{
            printf("error state\n");
			break;
        }
		int j = 0;
		get_lexeme(&j, code_struct, temp);

		code_struct->cur_size += j + 1;

		if (!strcmp(temp, "push"))
		{
			byte_struct->data[i] = CMD_PUSH;
			IS_LAST_COMMAND_PUSH = 1;
		}
		else if (!strcmp(temp, "pop"))
		{
			byte_struct->data[i] = CMD_POP;
			//specifiers[flags_size] = S_NUMBER_SPEC;
		}

		cmd_check(add, CMD_ADD)
		cmd_check(mul, CMD_MUL)
		cmd_check(div, CMD_DIV)
		cmd_check(sub, CMD_SUB)
		cmd_check(sin, CMD_SIN)
		cmd_check(cos, CMD_COS)
		cmd_check(pow, CMD_POW)
		cmd_check(sqrt, CMD_SQRT)
		cmd_check(in, CMD_IN)
		cmd_check(out, CMD_OUT)
		cmd_check(del, CMD_DEL)
		cmd_check(abs, CMD_ABS)
		cmd_check(circ, CMD_CIRC)
		cmd_check(cat, CMD_CAT)
		cmd_check(KOPM, CMD_KOPM)
		cmd_check(ln, CMD_LN)
		cmd_check(mem, CMD_MEM)
		cmd_check(log10, CMD_LOG10)
		cmd_check(log2, CMD_LOG2)
		cmd_check(draw, CMD_DRAW)
		cmd_check(fill, CMD_FILL)
		cmd_check(ret, CMD_RET)
		cmd_check(hlt, CMD_HLT)

		rix_check(rax, CMD_RAX)
		rix_check(rbx, CMD_RBX)
		rix_check(rcx, CMD_RCX)
		rix_check(rdx, CMD_RDX)
		else if (IS_LAST_COMMAND_PUSH)
		{
			byte_struct->data[i] = (double)std::atof(temp);
			specifiers[flags_size++] = S_NUMBER_SPEC;
			IS_LAST_COMMAND_PUSH = false;
		}

		transition_check(je, CMD_JE)
		transition_check(jab, CMD_JAB)
		transition_check(jae, CMD_JAE)
		transition_check(jbe, CMD_JBE)
		transition_check(ja, CMD_JA)
		transition_check(jb, CMD_JB)
		transition_check(call, CMD_CALL)
		transition_check(jmp, CMD_JMP)

		bracket_check('[')
		bracket_check('(')

		else if (temp[strlen(temp) - 1] == ':')
			byte_struct->data[i] = CMD_LABEL;
		else if (IS_LAST_COMMAND_JMP)
		{
			temp++;

			bool FIND_LABEL = false;
			for (int index = 0; index < amount_labels; index++)
				if (!strcmp(temp, labels[index].name))
				{
					byte_struct->data[i] = labels[index].adress;
					FIND_LABEL = true;
					break;
				}
			if (!FIND_LABEL)
			{
				byte_struct->error_state += ERROR_FIND_LABEL;
				break;
			}
			temp--;
			IS_LAST_COMMAND_JMP = false;
		}
		else
		{
			byte_struct->error_state += error_process(i, temp);
			byte_struct->unknown_command = (char*) calloc(MAX_SIZE_COMMAND, sizeof(char));
			assert(byte_struct->unknown_command);

            strcpy(byte_struct->unknown_command, temp);
			free(temp);
			temp = nullptr;
			break;
		}

		byte_struct->bytecode_capacity++;
	}

	if (byte_struct->error_state)
	{
		free(specifiers);
        specifiers = nullptr;

		free(labels);
        labels = nullptr;

        free(temp);
        temp = nullptr;

		determine_status(byte_struct);
		return;
	}

	determine_status(byte_struct);

	FILE *assembler_txt = fopen("[!]assembler_code.txt", "w");
	assert(assembler_txt);
    //printf("here\n");
	print_assem_id(assembler_txt)

    flags_size = 0;
   // printf("capacity is %d\n", byte_struct->bytecode_capacity);
	for (int i = 0; i < byte_struct->bytecode_capacity; i++)
		if ((static_cast<int>(byte_struct->data[i]) == CMD_POP) || (static_cast<int>(byte_struct->data[i]) == CMD_PUSH))
		{
			fprintf(assembler_txt, "%lg ", byte_struct->data[i++] + specifiers[flags_size++]);
			fprintf(assembler_txt, "%lg ", byte_struct->data[i]);
		}
		else
			fprintf(assembler_txt, "%lg ", byte_struct->data[i]);

	free(specifiers);
    specifiers = nullptr;

	for (int y = 0; y < amount_labels; y++)
		if (labels[y].name)
			free(labels[y].name);

	free(labels);
	labels = nullptr;

	free(temp);
	temp = nullptr;

	fclose(assembler_txt);

	return;
}

auto bytecode_destruct(Bytecode *byte_struct) -> void
{
	assert(byte_struct);

	free(byte_struct->data);
	byte_struct->data = nullptr;

	free(byte_struct->unknown_command);
	byte_struct->unknown_command = nullptr;

	byte_struct->bytecode_capacity = 0;
}

auto useful_sizes(FILE *user_code, Text *text_struct, int *file_lines, long *file_length) -> void
{
	assert(user_code);
	assert(text_struct);
	assert(file_lines);
	assert(file_length);

	while ((text_struct->data[*file_length] = fgetc(user_code)) && (!feof(user_code)))
	{
		if (text_struct->data[*file_length] == '\n')
			(*file_lines)++;
		(*file_length)++;
	}
}

auto set_time(struct tm *time) -> char *
{
	char string_time[size_time] = {};
	char *tmp = nullptr;

	strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

	tmp = (char *)malloc(sizeof(string_time));
	strcpy(tmp, string_time);

	return tmp;
}

auto define_date(void) -> char*
{
	const time_t timer = time(nullptr);
	struct tm *local_time = localtime(&timer);

	return set_time(local_time);
}

auto size_of_file(FILE *user_code) -> long
{
	assert(user_code);

	fseek(user_code, 0, SEEK_END);
	long file_length = ftell(user_code);
	fseek(user_code, 0, SEEK_SET);

	file_length++;

	return file_length;
}

inline get_lexeme(int *j, Code *code_struct, char *temp)
{
	for ((*j) = 0; (*j) < MAX_SIZE_COMMAND; (*j)++)
	{
		if ((code_struct->data[*j + code_struct->cur_size] != ' '))
			temp[*j] = code_struct->data[*j + code_struct->cur_size];
		else
		{
			temp[*j] = '\0';
			break;
		}
	}
}

inline bracket_exe(char spec, char *temp, Bytecode *byte_struct, int *flags_size, double *specifiers, int i)
{
	using namespace my_commands;

	int SPEC_NUMBER = 0;
	int SPEC_REGIST = 0;

	byte_struct->error_state += define_specs(&SPEC_NUMBER, &SPEC_REGIST, spec);

	if (isdigit(*(temp + 1)))
	{
		byte_struct->data[i] = std::atoi(temp + 1);
		specifiers[(*flags_size)++] = SPEC_NUMBER;
		IS_LAST_COMMAND_PUSH = false;
	}
	else if (strlen(temp) == 5)
	{
		char new_temp[] = {temp[1], temp[2], temp[3]};
		if (!strncmp(new_temp, "rax", 3))
		{
			byte_struct->data[i] = CMD_RAX;
			specifiers[(*flags_size)++] = SPEC_REGIST;
			IS_LAST_COMMAND_PUSH = false;
		}
		else if (!strncmp(new_temp, "rbx", 3))
		{
			byte_struct->data[i] = CMD_RBX;
			specifiers[(*flags_size)++] = SPEC_REGIST;
			IS_LAST_COMMAND_PUSH = false;
		}
		else if (!strncmp(new_temp, "rcx", 3))
		{
			byte_struct->data[i] = CMD_RCX;
			specifiers[(*flags_size)++] = SPEC_REGIST;
			IS_LAST_COMMAND_PUSH = false;
		}
		else if (!strncmp(new_temp, "rdx", 3))
		{
			byte_struct->data[i] = CMD_RDX;
			specifiers[(*flags_size)++] = SPEC_REGIST;
			IS_LAST_COMMAND_PUSH = false;
		}
	}
	else
	{
		printf("Something went wrong in ()\n");
		byte_struct->error_state += ERROR_PROC_BRACKET;
	}
}

auto define_specs(int *SPEC_NUMBER, int *SPEC_REGIST, char spec) -> int
{
	using namespace my_commands;

	if (spec == '[')
	{
		*SPEC_NUMBER = OP_DOUBLE_NUMBER;
		*SPEC_REGIST = OP_DOUBLE_REGIST;
	}
	else if (spec == '(')
	{
		*SPEC_NUMBER = OP_CHAR_NUM;
		*SPEC_REGIST = OP_CHAR_REG;
	}
	else
	{
		printf("Something went wrong in define bracket\n");
		return ERROR_UNK_BRACKET;
	}
	return 0;
}

auto get_labels(Label *labels, Code *code_struct) -> number_of_labels
{
	for (int y = 0; y < MAX_LABELS; y++)
	{
		labels[y].name = (char *)calloc(MAX_LABEL_SIZE, sizeof(char));
		assert(labels[y].name);
	}

	char *temp = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
	assert(temp);

	int cur_labels = 0;
	code_struct->cur_size = 0;

	for (int i = 0; i < code_struct->terms; i++)
	{
		int iter = 0;
		get_lexeme(&iter, code_struct, temp);

		if (temp[strlen(temp) - 1] == ':')
		{
			strncpy(labels[cur_labels].name, temp, strlen(temp) - 1);

			labels[cur_labels].length = strlen(temp);

			labels[cur_labels++].adress = i;
		}

		code_struct->cur_size += iter + 1;
	}

	code_struct->cur_size = 0;
	free(temp);

	return cur_labels; //int amount_labels = cur_labels;
}

auto error_process(int i, char *temp) -> int
{
	using namespace my_commands;

	/*FILE* error = fopen("[!]ERRORS.txt", "w");
	assert(error);

    fprintf(error, "\n\tData of error : %s (dd/mm/yy)\n\n", define_date());
    fprintf(error, "Assembler doesn't know some command..\n");
    fprintf(error, "bytecode[%d] = [%s]\n", i, temp);
    fclose(error); */

	printf("Unknown command error\n");

	return ERROR_UNKNOWN_COM;
}

auto determine_status(Bytecode *byte_struct) -> void
{
	using namespace my_commands;

	FILE *error = fopen("[!]DUMP_ASSEM.txt", "w");
	assert(error);

	fprintf(error, "\tData of DUMP in Assembler : %s (dd/mm/yy)\n\n", define_date());

	if (!byte_struct->error_state)
		print_good(error);
	else
		for (int i = 0; i < NUMBER_OF_ERRORS; i++)
			if (get_byte(byte_struct->error_state, i + 1))
				print_error(error, i + 1, byte_struct);

	fclose(error);
	return;
}

auto print_good(FILE *error) -> void
{
	assert(error);

	fprintf(error, "Assem ended without any errors\n");

	return;
}

auto print_error(FILE *error, int error_bit, Bytecode *byte_struct) -> void
{
	using namespace my_commands;
	switch (error_bit)
	{

	case ID_UNKNOWN_COM:
		fprintf(error, "Assembler met unknown command #%s#\n", byte_struct->unknown_command);
		break;
	case ID_FIND_LABEL:
		fprintf(error, "Cann't find label\n");
		break;
	case ID_PROC_BRACKET:
		fprintf(error, "Error in processing of brackets\n");
		break;
	case ID_UNK_BRACKET:
		fprintf(error, "Unknown type of bracket\n");
		break;
	default:
		fprintf(error, "Error in ERRORS. IT means that program has unknown error\n");
		break;
	}
}

auto get_byte(int digit, int number_of_bit) -> bool
{
    return (bool((1 << (number_of_bit - 1))  &  digit));
}
