# SIRF
Simple Intermediate Representation Format

> [!WARNING]
> This project is currently in the planning stage.

SIRF is an intermediate representation format that aims to be optimzable, portable and minimal.

# Features

## IR Abstractions

SIRF defines abstractions on the following points as:
- Core semantics (eg. arithmetic): **LOW** abstractions
- Data types: **MEDIUM** abstractions
- Variables & stack: **MEDIUM** abstractions
- Functions: **HIGH** abstractions

# Examples

Consider the following C code:
```c
#include <stdio.h> // For printf

static int add(int a, int b) {
  return a + b;
}

int main(int argc, char** argv) {
  int a = 10, b = 25;
  char* name = argv[0];
  int result = add(a, b);

  printf("%s: %d\n", name, result);
  return 0;
}
```

Given a fully implemented frontend, the C code above should emit this SIRF code:
```asm
extern @printf
global @main

data .fmt: "%s: %d\n\0"

fun i32 @add(a i32, b i32) !static {
  mov r0, %a
  mov r1, %b
  add r0, r1
  ret r0
}

fun i32 @main(argc i32, argv i8^^) {
  %1 = 10 i32
  %2 = 25 i32
  %3 = load %argv
  %4 = call @add(%1, %2)
  call @printf(.fmt, %3, %4)
  ret  0
}
```
