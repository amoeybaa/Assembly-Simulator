# Assembly Language Compiler & Simulator in C

This project implements a two-pass assembler and a simulator for a simple assembly language. It includes:
* A compiler that checks syntax, builds symbol and literal tables, generates intermediate code (Variant I), and creates an object code file.
* A simulator that executes the generated object code and simulates memory/registers.


## Compiler (`compiler_program.c`)

Parses user-defined assembly codeand detects syntactic and semantic errors.

* Builds:
   * Symbol Table
   * Literal Table
   * Pool Table

* Generates:
   * Variant-I Intermediate Code
   * Object Code File (.obj)
 
## Simulator (`simulator_program.c`)

Loads and simulates the generated .obj file

Executes operations with simulated memory of 1000 locations and 4 registers (AREG, BREG, CREG, DREG).

* Provides:
   * Full program execution
   * Step-by-step trace mode
   * Raw object code view

## Technical Requirements
* C compiler (e.g. GCC)
* Unix/Windows with terminal support

## Assembly Language Syntax
* Instruction Set: `STOP`, `ADD`, `SUB`, `MULT`, `MOVER`, `MOVEM`, `COMP`, `BC`, `DIV`, `READ`, `PRINT`
* Assembly Directives: `START`, `END`, `EQU`, `ORIGIN`, `LTORG`
* Registers: `AREG`, `BREG`, `CREG`, `DREG`
* Conditional Codes: `LT`, `LE`, `EQ`, `GT`, `GE`, `ANY`
* Storage Instructions: `DC`, `DS`

## Compilation
```
gcc compiler_program.c
gcc simulator_program.c
```

## Execution
* Compile the Assembly Code  
  ```
  ./a input.asm
  ```
  Generates: input.obj — the object code file.

  Example assembly programs have been provided.

* Run the Simulator as a usual C program.

## Author & Acknowledgements
This project was part of an educational assignment on building a basic compiler and simulator for Assembly Language from scratch in C.

Made during my 5th Semester at Fergusson College pursuing B.Sc. (Computer Science) following the provided lab manual.

