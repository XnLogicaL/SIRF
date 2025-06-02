section .data
section .text
global main
add:
  push rbp
  mov rbp, rsp
  mov eax, 0
  jmp .Ladd.epilogue
.Ladd.epilogue:
  pop rbp
  ret
main:
  push rbp
  mov rbp, rsp
  mov eax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  pop rbp
  ret
section .bss
