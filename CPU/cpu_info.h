#ifndef CPU_INFO_H_INCLUDED
#define CPU_INFO_H_INCLUDED

constexpr int START_SIZE    = 10;
constexpr int size_time     = 40;
constexpr double epsilon    = 1e-8;
constexpr double absEpsilon = 1e-12;

constexpr int SIZEX         = 300;
constexpr int SIZEY         = 300;
constexpr int OP_SIZE       = 3 * SIZEX * SIZEY;



struct Bytecode
{
    double* data                = nullptr;
    int     unknown_command     = -666;
    int     bytecode_capacity   = 0;
	int 	error_state 	    = 0;
};

struct Rix
{
    double rax = NAN;
    double rbx = NAN;
    double rcx = NAN;
    double rdx = NAN;
};

namespace memes_names
{
    enum Commands
    {
		DED_CODESTYLE 	 = 0,
		DED_ILAB_CLASSIC = 1,
		CAT              = 2,
		BOMONKA			 = 3,
		DED_HOHLOV		 = 4,
		CODING_BUGS		 = 5
	};
}

#endif // CPU_INFO_H_INCLUDED
