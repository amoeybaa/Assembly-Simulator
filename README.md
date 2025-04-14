# Assembly Language Compiler & Simulator

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

## Requirements
* C compiler (e.g. GCC)
* Unix/Windows with terminal support

## Assembly Language Syntax



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

* Run the Simulator as a usual C program.
