/* header file for simulator functions */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INS_SIZE 80				// Each line of instuction in object file
#define INS_CODE_SIZE 10		// Size of each individual instruction code
#define MAX_MEM 1000			// Size of memory
#define REG_COUNT 4				// # of registers

void load_file(); 		// Load object file into linked list
void execute();	  		// execute the loaded .obj file
void trace();	  		// trace registers and memory through execution
void display();	  		// display file contents

FILE *fp = NULL;
int *memory, regs[REG_COUNT] = {0}, flag = 0;
int pc = 0;
static char *reg_name[] = {"AREG", "BREG", "CREG", "DREG"};

void load_file()
{
	/*
        INTRODUCTION:
            loads the object file into the simulated memory of size MAX_MEM

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - None

        RETURN VALUES:
            - None

        APPROACH:
			0. The file name has been taken in main function and it has been opened already in "w" mode
            1. Reads each instruction in the object file and tokenize it
			2. Store the instruction at the appropriate memory location
			3. Initialize the value of the program counter `pc`
	*/

	char t[2][INS_CODE_SIZE], *temp = NULL, buffer[INS_SIZE];
	memory = (int *)calloc(MAX_MEM, sizeof(int));
	int i, init_addr = -1;
	int lc, inst;

	rewind(fp);
	while (fgets(buffer, INS_SIZE, fp)) {
		i = 0;
		temp = strtok(buffer, " \t\n,");
		strcpy(t[0], "");
		strcpy(t[1], "");

		while (temp && i < 2)
		{
			strcpy(t[i], temp);
			temp = strtok(NULL, " \t\n,");
			++i;
		}
		lc = atoi(t[0]);
    	inst = atoi(t[1]);

		if (init_addr < 0 && lc >= 0)
			init_addr = lc;
		
		if (lc >= 0 && lc < MAX_MEM)
			memory[lc] = inst;
	}
	pc = init_addr;
}

void execute()
{
	/*
        INTRODUCTION:
            executes the object file using program counter and the instructions stored in the memory

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - None

        RETURN VALUES:
            - None

        APPROACH:
			0. The file has already been loaded onto the memory using `load_file()` function
            1. Goes to the memory location pointed by program counter and stars to execute the program
			2. Executes till it encounters END
	*/

	int temp_pc = pc, operand1, operand2, reg_code, inst, address, opcode;
	while(1)
	{
		inst = memory[temp_pc];
		address = inst % 1000;
		inst = inst / 1000;
		opcode = inst / 10;
		reg_code = inst % 10;
		switch (opcode)
		{
			case 0:
				if (address == 0 && opcode == 0 && reg_code == 0)
					return;
				else
					break;

			case 1:
				regs[reg_code - 1] += memory[address];
				break;

			case 2:
				regs[reg_code - 1] -= memory[address];
				break;

			case 3:
				regs[reg_code - 1] *= memory[address];
				break;

			case 8:
				if (memory[address] == 0)
				{
					printf("\nZero Division Error!\n");
					break;
				}
				regs[reg_code - 1] /= memory[address];
				break;

			case 4:
				regs[reg_code - 1] = memory[address];
				break;

			case 5:
				memory[address] = regs[reg_code - 1];
				break;

			case 6:
				operand1 = regs[reg_code - 1];
				operand2 = memory[address];
				break;

			case 7:
				switch (reg_code)
				{
					case 1:
						if (operand1 < operand2)
							temp_pc = address - 1;
						break;

					case 2:
						if (operand1 <= operand2)
							temp_pc = address - 1;
						break;

					case 3:
						if (operand1 == operand2)
							temp_pc = address - 1;
						break;

					case 4:
						if (operand1 > operand2)
							temp_pc = address - 1;
						break;

					case 5:
						if (operand1 >= operand2)
							temp_pc = address - 1;
						break;

					case 6:
						temp_pc = address - 1;
						break;
				}
				break;

			case 9:
				printf("\nEnter a number: ");
				scanf("%d", &memory[address]);
				break;

			case 10:
				printf("\n---> Output: %d\n", memory[address]);
				break;
		}
		++temp_pc;
	}
}

void trace()
{
	/*
        INTRODUCTION:
            executes the object file using program counter and the instructions stored in the memory
			also prints the values of current opcode, register code, address, value at the address and value at the register

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - Displays onto the standard display

        RETURN VALUES:
            - None

        APPROACH:
			0. The file has already been loaded onto the memory using `load_file()` function
            1. Goes to the memory location pointed by program counter and stars to execute the program
			2. Executes till it encounters END and prints at each instruction
	*/

	int temp_pc = pc, operand1, operand2, reg_code, inst, address, opcode;
	while(1)
	{
		inst = memory[temp_pc];
		address = inst % 1000;
		inst = inst / 1000;
		opcode = inst / 10;
		reg_code = inst % 10;

		printf("\nOpcode: %d\nRegister: %d\nAddress: %d\n", opcode, reg_code, address);

		switch (opcode)
		{
			case 0:
				if (address == 0 && opcode == 0 && reg_code == 0)
					return;
				else
					break;

			case 1:
				regs[reg_code - 1] += memory[address];
				break;

			case 2:
				regs[reg_code - 1] -= memory[address];
				break;

			case 3:
				regs[reg_code - 1] *= memory[address];
				break;

			case 8:
				if (memory[address] == 0)
				{
					printf("\nZero Division Error!\n");
					break;
				}
				regs[reg_code - 1] /= memory[address];
				break;

			case 4:
				regs[reg_code - 1] = memory[address];
				break;

			case 5:
				memory[address] = regs[reg_code - 1];
				break;

			case 6:
				operand1 = regs[reg_code - 1];
				operand2 = memory[address];
				break;

			case 7:
				switch (reg_code)
				{
					case 1:
						if (operand1 < operand2)
							temp_pc = address - 1;
						break;

					case 2:
						if (operand1 <= operand2)
							temp_pc = address - 1;
						break;

					case 3:
						if (operand1 == operand2)
							temp_pc = address - 1;
						break;

					case 4:
						if (operand1 > operand2)
							temp_pc = address - 1;
						break;

					case 5:
						if (operand1 >= operand2)
							temp_pc = address - 1;
						break;

					case 6:
						temp_pc = address - 1;
						break;
				}
				break;

			case 9:
				printf("\nEnter a number: ");
				scanf("%d", &memory[address]);
				break;

			case 10:
				printf("\n---> Output: %d\n", memory[address]);
				break;
		}
		if (address != 0 && opcode != 0 && reg_code != 0)
			printf("Value at address %d: %d\n", address, memory[address]);
		if (reg_code != 0)
			printf("Value at register %s: %d\n", reg_name[reg_code-1], regs[reg_code-1]);

		++temp_pc;
	}
}

void display()
{
	/*
        INTRODUCTION:
            prints the contents of the object file as it is

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - Displayed on the standard output

        RETURN VALUES:
            - None

        APPROACH:
			0. The file has already been loaded onto the memory using `load_file()` function
            1. Traverse the file linearly and print each line till end of file
	*/

	char t[2][INS_CODE_SIZE], *temp = NULL, buffer[INS_SIZE];
	int i;

	rewind(fp);
	while (fgets(buffer, INS_SIZE, fp))
	{
		i = 0;

		strcpy(t[0], "");
		strcpy(t[1], "");
		temp = strtok(buffer, " \t\n,");
		while(temp && i < 2)
		{
			strcpy(t[i], temp);
			temp = strtok(NULL, " \t\n,");
			++i;
		}
		printf("\n%s\t%s", t[0], t[1]);
	}
	printf("\n----------------------------------------------------------------\n");
}

#endif				// SIMULATOR_H