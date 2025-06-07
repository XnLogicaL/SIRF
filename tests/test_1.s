section .data
section .text
global main
add:
  push rbx
  push rbp
  mov rbp, rsp
  mov rax, 10
  sub rsp, 8                    ; r11 spill
  mov qword [rbp-8], 69
  sub rsp, 8                    ; r29 spill
  mov qword [rbp-16], 100
  mov qword [rbp-8], 35
  mov r11, [rbp-8]
  add qword [rbp-16], r11
  sub qword [rbp-8], 591
  shr qword [rbp-8], 4
  mov rax, [rbp-8]
  jmp .Ladd.epilogue
.Ladd.epilogue:
  pop rbx
  pop rbp
  ret
main:
  push rbx
  push rbp
  mov rbp, rsp
  mov rax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  pop rbx
  pop rbp
  ret
section .bss
section .note.GNU-stack,"",@progbits
