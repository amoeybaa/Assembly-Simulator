/* Header file for all mneumonic search operations */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assembler data structures which store the opcodes for all the operands and registers
static char *optab[] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT"};
static char *regtab[] = {"AREG", "BREG", "CREG", "DREG"};
static char *dirtab[] = {"START", "END", "EQU", "ORIGIN", "LTORG"};
static char *storage[] = {"DC", "DS"};
static char *cc[] = {"LT", "LE", "EQ", "GT", "GE", "ANY"};

// Global variables to store how many words each array stores
static int optab_size = sizeof(optab) / sizeof(char *);
static int regtab_size = sizeof(regtab) / sizeof(char *);
static int dirtab_size = sizeof(dirtab) / sizeof(char *);
static int storage_size = sizeof(storage) / sizeof(char *);
static int cc_size = sizeof(cc) / sizeof(char *);

int isRegtab(char *);  // check if the given register name is a valid register
int isOptab(char *);   // check if given operator is a valid instriction set operator
int isDirtab(char *);  // check if given operator is a valid assembly directive operator
int isStorage(char *); // check if given operator is a valid storage operator
int isCc(char *);	   // check if given operator is a valid conditional comparison operator
void remCom(char *);   // function to remove any commas (,) given in the assembly code by the user

int isRegtab(char *str)
{
	/*
		INTRODUCTION:
			checks if the given register is a valid register mentioned in `*regtab`

		INPUT PARAMETERS:
			- str (char[]):	name of the given register

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- int:	returns 1 if given register is a valid register, otherwise returns 0

		APPROACH:
			iterate through the `*regtab` table comparing each valid register name
			if theres a match, return true (1), else return false (0)
	*/

	for (int i = 0; i < regtab_size; ++i)
	{
		if(strcasecmp(regtab[i], str) == 0)
			return 1;
	}
	return 0;
}

int isOptab(char *str)
{
	/*
		INTRODUCTION:
			checks if the given operator is a valid instruction operator mentioned in `*optab`

		INPUT PARAMETERS:
			- str (char[]):	name of the given operator

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- int:	returns 1 if given operator is a valid instruction set operator, otherwise returns 0

		APPROACH:
			iterate through the `*optab` table comparing each valid operator name
			if theres a match, return true (1), else return false (0)
	*/

	for (int i = 0; i < optab_size; ++i)
	{
		if(strcasecmp(optab[i], str) == 0)
			return 1;
	}
	return 0;
}

int isDirtab(char *str)
{
	/*
		INTRODUCTION:
			checks if the given operator is a valid assembly directive operator mentioned in `*dirtab`

		INPUT PARAMETERS:
			- str (char[]):	name of the given operator

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- int:	returns 1 if given operator is a valid assembly directive operator, otherwise returns 0

		APPROACH:
			iterate through the `*dirtab` table comparing each valid operator name
			if theres a match, return true (1), else return false (0)
	*/

	for(int i = 0; i < dirtab_size; ++i)
	{
		if(strcasecmp(dirtab[i], str) == 0)
			return 1;
	}
	return 0;
}

int isStorage(char *str)
{
	/*
		INTRODUCTION:
			checks if the given operator is a valid storage operator mentioned in `*storage`

		INPUT PARAMETERS:
			- str (char[]):	name of the given operator

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- int:	returns 1 if given operator is a valid storage operator, otherwise returns 0

		APPROACH:
			iterate through the `*storage` table comparing each valid operator name
			if theres a match, return true (1), else return false (0)
	*/

	for(int i = 0; i < storage_size; ++i)
	{
		if(strcasecmp(storage[i], str) == 0)
			return 1;
	}
	return 0;
}

int isCc(char *str)
{
	/*
		INTRODUCTION:
			checks if the given operator is a valid conditional comparison operator mentioned in `*cc`

		INPUT PARAMETERS:
			- str (char[]):	name of the given operator

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- int:	returns 1 if given operator is a valid conditional comparison operator, otherwise returns 0

		APPROACH:
			iterate through the `*cc` table comparing each valid operator name
			if theres a match, return true (1), else return false (0)
	*/

	for(int i = 0; i < cc_size; ++i)
	{
		if(strcasecmp(cc[i], str) == 0)
			return 1;
	}
	return 0;
}

void remCom(char *str)
{
	/*
		INTRODUCTION:
			removes any commas (,) present in the assembly language program provided to the simulator

		INPUT PARAMETERS:
			- str (char[]):	a complete line of instruction of the type <operator> <operand1>, <operand2>

		OUTPUT PARAMETERS:
			- deletes any commas (,) present in the provided line of instruction

		RETURN VALUES:
			- None

		APPROACH:
			iterate through each character and replace any commas (,) with blank spaces (' ')
	*/

	char c;
	for(int i = 0; i < strlen(str); ++i)
	{
		c = str[i];
		if(c == ',')
			str[i] = ' ';
	}
}
