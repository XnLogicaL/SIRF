section .data
.LC0 db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 10, 0
section .text
global main
add:
  push rbp
  mov rbp, rsp
  mov rax, 34
  add rax, 35
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
