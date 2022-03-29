# Brainfuck Compiler and Interpreter

This package provides a compiler and interpreter for running programs in the esoteric programming language Brainfuck.

The compiler parses and converts the Brainfuck code into an intermediate assembly reducing the amount of computation required. The goal of this package is to develop a smart and fast compiler by applying various optimizations.

## Implemented Optimizations

Since Brainfuck syntax is very limited, the compiled code is translated to a mil-level assembly (see compiler.cc for assembly instructions).

### Compress Instructions

The code is compressed so that `+++` becomes `ADD 3` and `<<<` becomes `MOVE -3`.

### Optimize Simple Loops

We find L1 loops where the start and end position is on the same memory element with the element's value decremented in every run. These loops are replaced with simple multiply + add using registers.

### Usage

Compile sources (from `cpp_source` directory):

```
$ g++ *.cc */*.cc
```

Run compiled code:

```
$ ./a.out run ../test.bf
```

Print compiled assemply:

```
$ ./a.out asm ../test.bf
```

Print instructions to run after various optimizations:

```
$ ./a.out opt ../test.bf
```

Sample output:

```
Total raw instructions: 1799
After simple compilation: 1222
After loop optimization: 765
```