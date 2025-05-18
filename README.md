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

```
// printf definition here...
data .fmt {
  str: "%d\0"
}

fun @add(a i32, b i32) i32 {
  mov r0, %a;
  mov r1, %b;
  add r0, r1;
  ret r0;
}

fun @main(argc i32, argv ^^i8) {
  %1 = 10;
  %2 = 25;
  %3 = call @add(%1, %2);
  call @printf(.fmt, %3);
  ret 0;
}
```
