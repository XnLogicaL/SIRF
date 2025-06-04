section .data
section .text
global main
add:
  push rbp
  mov rbp, rsp
  mov rax, 10
  sub rsp, 8                    ; r11 spill
  mov qword [rbp-8], 69
  sub rsp, 8                    ; r29 spill
  mov qword [rbp-16], 62823
  mov qword [rbp-8], 35
  add qword [rbp-16], qword [rbp-8]
  sub qword [rbp-8], 591
  shr qword [rbp-8], 4
  mov rax, 0
  jmp .Ladd.epilogue
.Ladd.epilogue:
  pop rbp
  ret
main:
  push rbp
  mov rbp, rsp
  mov rax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  pop rbp
  ret
section .bss
section .note.GNU-stack,"",@progbits
