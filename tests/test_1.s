section .data
section .text
global main
extern printf
add:
  push rbx
  push rbp
  mov rbp, rsp
  mov rax, 34
  add rax, 35
  sub rsp, 8     ; alloca %2
  mov qword [rbp-8], rax
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
  call add
  mov rax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  pop rbx
  pop rbp
  ret
section .bss
section .note.GNU-stack,"",@progbits
