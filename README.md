# SIRF
Simple Intermediate Representation Format

> [!WARNING]
> This project is in the early implementation stage.

# Features

## IR Abstractions

SIRF defines abstractions on the following points as:
- Core semantics (eg. arithmetic): **LOW** abstractions
- Data types: **MEDIUM** abstractions
- Variables & stack: **MEDIUM** abstractions
- Functions: **HIGH** abstractions

# Examples

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
SIRF IR equivalent:
```asm
extern @printf
global @main

data .LC0: "%s: %d\n\0"

fun i32 @add(%a i32, %b i32) !static {
  mov r0, %a
  add r0, %b
  ret r0
}

fun i32 @main(%argc i32, %argv i8^^) {
  %1 = 10 i32
  %2 = 25 i32
  %3 = i32
  %4 = i32
  load %3, ptr %argv
  call @add, %4, %1, %2
  call @printf, _, ptr .LC0, %3, %4
  ret  0 i32
}
```

---

```c
#include <stdio.h>

int sum_to_n(int n) {
  int acc = 0;
  while (n > 0) {
    acc += n;
    n -= 1;
  }
  return acc;
}

int main(int, char**) {
  int total = sum_to_n(5);
  printf("Sum: %d\n", total);
  return 0;
}
```
SIRF IR equivalent:
```asm
extern @printf
global @main

data .LC0: "Sum: %d\n\0"

fun i32 sum_to_n(%n i32) !static {
  %0 = 0 i32
  jmp .L1
.L0:
  ssp
  add %0, %n
  sub %n, 1 i32
  rsp
.L1:
  %1 = BYTE
  gt  %1, %n, 0 i32
  jnz %1, .L0
  ret %0
}

fun i32 main(i32, i8^^) {
  %0 = i32
  call @sum_to_n, %0, 5 i32
  call @printf, _, ptr .LC0, %0
  ret  0 i32
}
```

---

```c
#include <stdio.h>

int abs_diff(int a, int b) {
  if (a > b)
    return a - b;
  else
    return b - a;
}

int main() {
  int result = abs_diff(42, 17);
  printf("Absolute Difference: %d\n", result);
  return 0;
}
```
SIRF IR equivalent:
```asm
extern @printf
global @main

data .LC0: "Absolute Difference: %d\n\0"

fun i32 @abs_diff(%a i32, %b i32) !static {
  %0 = BYTE
  gt  %0, %a, %b
  jnz %0, .L1
.L0:
  %1 = i32
  sub %1, %b, %a
  ret %1
.L1:
  %2 = i32
  sub %2, %a, %b
  ret %2
}

fun i32 @main(i32, i8^^) {
  %0 = i32
  call @abs_diff, %0, 42 i32, 17 i32
  call @printf, _, ptr .LC0, %0
  ret 0 i32
}
```
