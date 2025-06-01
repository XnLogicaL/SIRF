section .data
section .text
global main
main:
  push rbp
  mov rbp, rsp
  mov eax, 0
  jmp .Lmain.epilogue
.Lmain.epilogue:
  pop rbp
  ret
section .bss
