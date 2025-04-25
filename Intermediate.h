// header file for intermediate code (variant I) generation from assembly language program

#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OP_SIZE 10      // Size of operand and opcode word

typedef struct intcode  // structure to define the intermediate code node
{
    /*
        INTRODUCTION:
        Defines the structure of intermediate code translated from the assembly code provided by the user

        VARIABLES:
            - regcode (int):    register code to select one of four (4) available registers
            - addr (int):       address, denotes the address of the stored intermediate code
            - opcode (char[]):  operation code, specifies which operation is to be performed
            - operand (char[]): denotes the operand to be used, can be a symbol or a literal
            - next (ptr):         pointer to the next node of the instruction code linked-list
    */

    int regcode, addr;
    char opcode[OP_SIZE], operand[OP_SIZE];
    struct intcode *next;
} INTCODE;

// variables to handle the instruction code linked-list
INTCODE *ihead = NULL, *icurr = NULL, *inew = NULL;

void searchIs(char *);      // search Instruction Set opcodes in `*optab`
void searchAd(char *);      // search Assembly Directives opcodes in `*dirtab`
void searchReg(char *);     // search opcodes of registers in `*regtab`
void searchCc(char *);      // search opcodes of conditional comparators in `*cc`
void searchSym(char *);     // search symbol in Symbol table
void searchLit(char *);     // search literal in Literal table
void printIntcode();        // print the inermediate code stored as linked list
void destroyIntcode();      // destroy the INTCODE linked list storing all the intermediate codes

void searchIs(char *str)
{
    /*
        INTRODUCTION:
            searches the opcode of corresponding operator in instruction set table from "Mneumonic.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the operator

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding index number (opcode) of the operator is fetched from `*optab` in the "Mneumonics.h" header file
    */

    int i;
    for (i = 0; i < optab_size; i++)
    {
        if (strcasecmp(str, optab[i]) == 0)
            break;
    }
    sprintf(inew->opcode, "<IS, %d>", i);
}

void searchAd(char *str)
{
    /*
        INTRODUCTION:
            searches the opcode of corresponding operator in assembly directives table from "Mneumonic.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the operator

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding index number (opcode) of the operator is fetched from `*dirtab` in the "Mneumonics.h" header file
    */

    int i;
    for (i = 0; i < dirtab_size; ++i)
    {
        if (strcasecmp(str, dirtab[i]) == 0)
            break;
    }
    sprintf(inew->opcode, "<AD, %d>", i + 1);
}

void searchReg(char *str)
{
    /*
        INTRODUCTION:
            searches the opcode of corresponding register in register table from "Mneumonic.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the register

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding index number (opcode) of the register is fetched from `*regtab` in the "Mneumonics.h" header file
    */

    int i;
    for (i = 0; i < regtab_size; ++i)
    {
        if (strcasecmp(str, regtab[i]) == 0)
            break;
    }
    inew->regcode = i + 1;
}

void searchCc(char *str)
{
    /*
        INTRODUCTION:
            searches the opcode of corresponding operator in conditional comparator table from "Mneumonic.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the operator

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding index number (opcode) of the operator is fetched from `*cc` in the "Mneumonics.h" header file
    */

    int i;
    for (i = 0; i < cc_size; ++i)
    {
        if (strcasecmp(str, cc[i]) == 0)
            break;
    }
    inew->regcode = i + 1;
}

void searchSym(char *str)
{
    /*
        INTRODUCTION:
            searches the symbol number of corresponding symbol in the Symbol table from "Tables.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the symbol

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding symbol number of the symbol is fetched from Symbol table in the "Tables.h" header file
    */

    int i = 1;
    for (SYMTAB *stemp = shead; stemp; stemp = stemp->next)
    {
        if (strcasecmp(stemp->sym, str) == 0)
        {
            sprintf(inew->operand, "<S, %d>", i);
            break;
        }
        ++i;
    }
}

void searchLit(char *str)
{
    /*
        INTRODUCTION:
            searches the literal number of corresponding literal in the Literal table from "Tables.h" header file

        INPUT PARAMETERS:
            - str (char[]): name of the literal

        OUTPUT PARAMETERS:
            - prints the intermediate code onto the standard output

        RETURN VALUES:
            - None

        APPROACH:
            validation has already been performed in the first pass
            the corresponding literal number of the literal is fetched from Literal table in the "Tables.h" header file
    */

    int i = 1;
    for (LITTAB *ltemp = lhead; ltemp; ltemp = ltemp->next)
    {
        if (strcasecmp(ltemp->lit, str) == 0)
        {
            sprintf(inew->operand, "<L, %d>", i);
            break;
        }
        ++i;
    }
}

void printIntcode() {
    /*
        INTRODUCTION:
            prints the intermediate code from the INTCODE structure linked list

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - Displayed on the standard output

        RETURN VALUES:
            - None

        APPROACH:
            1. Linearly traverse the INTCODE linked list
            2. print all values of each node till end of list
    */

    printf("\nThe variant-I intermediate code is:\n\n");
    for(INTCODE *temp = ihead; temp; temp = temp->next)
    {
        printf("%d\t%s ", temp->addr, temp->opcode);
        if(temp->regcode != 0)
            printf("<%d> ",temp->regcode);
            printf("%s\n", temp->operand);
    }
}

void destroyIntcode() {
    /*
        INTRODUCTION:
            destroys the intermediate code linked list

        INPUT PARAMETERS:
            - None

        OUTPUT PARAMETERS:
            - None

        RETURN VALUES:
            - None

        APPROACH:
            1. Linearly traverse the INTCODE linked list
            2. Store current node in a temporary variable and move the head to the next node
            3. free the current node
            4. Repeat till end of linked list
    */

    INTCODE *temp = ihead, *curr = NULL;
    while(temp) {
        curr = temp;
        temp = temp->next;
        free(curr);
    }
}

#endif          // INTERMEDIATE_H