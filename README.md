# Brainfuck Compiler and Interpreter

This package provides a compiler and interpreter for running programs in the esoteric programming language Brainfuck.

The compiler parses and converts the Brainfuck code into an intermediate assembly reducing the amount of computation required. The goal of this package is to develop a smart and fast compiler by applying various optimizations. At the moment, the compiler is still pretty naive (only real optimization is grouping stackable commands so `++++` compiles into `ADD 4` instead of running 4 add 1 operations).

The main goal of this project is for me to think and come up with fun optimizations to shrink the code size and running time. If it's useful to you, fell free to use as you please or send patches.

### Usage

Compile sources and run:

```
$ g++ *.hpp *.cpp
$ ./a.out run ../test.bf
```

Print compiled assemply:

```
$ g++ *.hpp *.cpp
$ ./a.out asm ../test.bf
```