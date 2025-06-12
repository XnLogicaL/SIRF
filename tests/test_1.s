section .data
section .text
global main
extern printf
add:
  push rbp
  mov rbp, rsp
  mov rax, 34
  add rax, 35
  shr rax, 1
  jmp .Ladd.epilogue
.Ladd.epilogue:
  leave
  ret
main:
  push rbp
  mov rbp, rsp
  mov rax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  leave
  ret
section .bss
section .note.GNU-stack,"",@progbits
