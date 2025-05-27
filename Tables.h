/* Header file for symbol tabe, literal table and pool table construction and updation */

#ifndef TABLES_H
#define TABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYM_SIZE 10
#define LIT_SIZE 10


void enterSymtab(char *, int);		// function to insert new symbol into Symbol table
int printSymtab();
void destroySymtab();
void enterLittab(char *);			// function to construct the Literal table
void printLittab();
void destroyLittab();
void createPool();					// function to create pools
void printPool();


// structure to define the Symbol table
typedef struct symtab
{
	/*
		INTRODUCTION:
		Defines the structure of Symbol table ie. stores the variable data in an assembly program, ex: A

		VARIABLES:
			- sym (char[]):	name of the symbol
			- addr (int):	address, stores the address of the symbol
			- uf (int): 	used flag, denotes if the symbol has been used in the program
			- df (int): 	defined flag, denotes if the symbol has been defined in the program
			- cnt (int): 	count, denotes how many times the symbol has been used in the program
			- next (ptr):	pointer to the next entry in symbol table
	*/

	char sym[SYM_SIZE];
	int addr, uf, df, cnt;
	struct symtab *next;
} SYMTAB;

SYMTAB *shead = NULL, *snew = NULL, *scurr = NULL;		// variables to handle the Symbol table linked-list

// structure to define the Literal table
typedef struct littab
{
	/*
		INTRODUCTION:
		Defines the structure of Literal table ie. stores the literal data in an assembly program, ex: 5

		VARIABLES:
			- lit (char[]):	value of the literal
			- cnt (int): 	count, denotes how many times the literal has been used in the program
			- addr (int):	address, stores the address of the literal
			- pool (int): 	denotes which pool the literal lies in
			- next (ptr):	pointer to the next entry in the Literal table

	*/

	char lit[LIT_SIZE];
	int cnt, addr, pool;
	struct littab *next;
} LITTAB;

LITTAB *lhead = NULL, *lnew = NULL, *lcurr = NULL;		// variables to handle the Literal table linked-list

int plc, lc = 0;		// location counter variables

void enterSymtab(char *a, int val)
{
	/*
		INTRODUCTION:
			adds a new symbol into the Symbol table, or updates an existing symbol based on its category as an operand or a symbol

		INPUT PARAMETERS:
			- a (char[]):	name of the symbol
			- val (int):	specifies if the symbol is an operand or a label

		OUTPUT PARAMETERS:
			- symtab*:	either a new node in the Symbol table linked-list or an updation to an existing node

		RETURN VALUES:
			- None

		APPROACH:
			1. A linear search through the Symbol table linked-list to check if the symbol with given name already exists
			2. If it does not, add the new symbol to the linked list
			3. If it does, update the required fields in the corresponding node
	*/
	SYMTAB *temp;
	int flag = 0;

	for(temp = shead; temp != NULL; temp = temp->next)
	{
		if (strcasecmp(temp->sym, a) == 0)
		{
			flag = 1;
			break;
		}
	}

	// new entry to Symbol table
	if(!flag) {
		snew = (SYMTAB *)malloc(sizeof(SYMTAB));
		strcpy(snew->sym, a);
		snew->addr = -1; snew->uf = -1; snew->df = -1; snew->cnt = 0; snew->next = NULL;		// Initialization

		switch (val)
		{
			case 0:			// operand
				snew->uf = 1;
				snew->cnt = 0;
				break;

			case 1:				// label
				snew->addr = plc;
				snew->df = 1;
				++snew->cnt;
				break;
		}
		snew->next = NULL;

		if (shead == NULL)
			shead = scurr = snew;
		else {
			scurr->next = snew;
			scurr = snew;
		}
	}

	else		// symbol already in Symbol table
	{
		switch (val)
		{
			case 0:			// operand
				temp->uf = 1;
				break;

			case 1:			// label
				if (temp->addr == -1)
					temp->addr = plc;
				++temp->cnt;
				temp->df = 1;
				break;
		}
	}
}

int printSymtab() {
	/*
		INTRODUCTION:
			prints all the entries in the Symbol table and also checks if there are any errors in the Symbol table entries

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- Displayed on the standard output

		RETURN VALUES:
			- int:	A flag variable returning 1 if errors present, else returns 0

		APPROACH:
			1. A linear traversal through the Symbol table linked-list
			2. Print all values of each node till the last node
			3. Check if any errors present in the Symbol table entries
	*/

	SYMTAB *temp = NULL;
	int n = 1, isError = 0;
	printf("\n\nSymbol Table");
	printf("\n\nS.No.\tName\tAddress\tUsed\tDefined\tCount\n------------------------------------------------\n");
	for(temp = shead; temp; temp = temp->next)
	{
		printf("%d\t%s\t%d\t%d\t%d\t%d\n", n, temp->sym, temp->addr, temp->uf, temp->df, temp->cnt);
		++n;
	}
	printf("\n");
	for(temp = shead; temp; temp = temp->next)
	{
		if(temp->df == -1 && temp->uf == 1) {
			printf("\nError: Symbol %s is used but not defined!\n", temp->sym);
			++isError;
		}
		if(temp->cnt > 1) {
			printf("Error: Redeclaration of symbol %s!\n", temp->sym);
			++isError;
		}
		if(temp->uf == -1 && temp->df == 1)
			printf("\nWarning: Symbol %s is defined but not used!\n", temp->sym);
	}
	return(isError > 0);
}

void destroySymtab() {
	/*
		INTRODUCTION:
			destroys all the entries of the symbol table

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- None

		APPROACH:
			1. A linear traversal through the Symbol table linked-list
			2. Storing current node to a temporary variable and moving head to the next node
			3. freeing current node
			4. Repeat till end of linked list
	*/

	SYMTAB *temp = shead, *curr = NULL;
	while(temp) {
		curr = temp;
		temp = temp->next;
		free(curr);
	}
}

void enterLittab(char *str)
{
	/*
		INTRODUCTION:
			adds a new literal into the Literal table by converting it from string to integer

		INPUT PARAMETERS:
			- str (char[]):	value of the literal

		OUTPUT PARAMETERS:
			- littab*:	a new node in the Literal table linked-list

		RETURN VALUES:
			- None

		APPROACH:
			1. A linear search through the Literal table linked-list to check if the literal with given value already exists
			2. If it does not, add the new literal to the linked list
			3. If it does, update the required fields in the corresponding node
	*/

	LITTAB *temp;
	int flag = 0;

	for (temp = lhead; temp != NULL; temp = temp->next)
	{
		if (strcasecmp(temp->lit, str) == 0)
		{
			flag = 1;
			break;
		}
	}

	// new entry to Literal table
	if(!flag) {
		lnew = (LITTAB *)malloc(sizeof(LITTAB));
		strcpy(lnew->lit, str);
		lnew->addr = -1;
		lnew->pool = -1;
		lnew->cnt = 1;
		lnew->next = NULL;

		if (lhead == NULL)
			lhead = lcurr = lnew;
		else {
			lcurr->next = lnew;
			lcurr = lnew;
		}
	}

	else		// literal already in Literal table
		++temp->cnt;
}

void printLittab() {
	/*
		INTRODUCTION:
			prints all the entries of the Literal table

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- Displayed on the standard output

		RETURN VALUES:
			- None

		APPROACH:
			1. A linear traversal through the Literal table linked-list
			2. Print all values of each node
			3. Repeat till end of linked list
	*/

	int n = 1;
	printf("\n\nLiteral Table");
	printf("\n\nL.No.\tLiteral\tAddress\tCount\n----------------------------------------\n");
	for(LITTAB *l = lhead; l; l = l->next)
	{
		printf("%d\t%s\t%d\t%d\n", n, l->lit, l->addr, l->cnt);
		++n;
	}
}

void destroyLittab() {
	/*
		INTRODUCTION:
			destroys the Literal table

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- None

		RETURN VALUES:
			- None

		APPROACH:
			1. A linear traversal through the Literal table linked-list
			2. Store current node in a temporary variable, and moving head to next node
			3. freeing current node
			4. Repeat till end of linked list
	*/

	LITTAB *temp = lhead, *curr = NULL;
	while(temp) {
		curr = temp;
		temp = temp->next;
		free(curr);
	}
}

void createPool()
{
	/*
		INTRODUCTION:
			creates pools according to the given program, creates one (1) pool by default

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- Updates `pool` variable in each node of Literal table, if they exist
			- The pools are made based on the input program, by default creates one (1) pool

		RETURN VALUES:
			- None

		APPROACH:
			finds the value of `pool` for the first literal node, then assigns `pool` value for subsequent nodes depending on the division provided in the program
	*/

	LITTAB *temp = lhead;
	if (temp == NULL)
		return;
	int n = 0;
	if (temp->pool == -1)
		n = 1;
	else
	{
		for (temp = lhead; temp != NULL; temp = temp->next)
		{
			if(n < temp->pool)
				n = temp->pool;
		}
		++n;
	}

	for (temp = lhead; temp != NULL; temp = temp->next)
	{
		if (temp->pool == -1)
		{
			temp->pool = n;
			temp->addr = plc;
			++plc;
			++lc;
		}
	}
	--lc;
}

void printPool() {
	/*
		INTRODUCTION:
			calculates and prints the pools in the assembly code.

		INPUT PARAMETERS:
			- None

		OUTPUT PARAMETERS:
			- Displayed on standard output

		RETURN VALUES:
			- None

		APPROACH:
			1. Assigns 1 pool by default
			2. Calculates next pools by identifying keywords in the code and accessing the `pool` variable in the LITTAB structure
			3. Prints these pools
	*/

	int n = 1;
	printf("\n\nPool Table");
	printf("\n\nLiteral No.\n------------\n");
	printf("1");
	for(LITTAB *l = lhead; l; l = l->next)
	{
		if(l->pool != n)
		{
			n = l->pool;
			printf("\n%d", n);
		}
	}
	printf("\n");
}

#endif 			// TABLES_H