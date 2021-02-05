SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strrchr
%endif
	GLOBAL _strrchr

extern _strlen

_strrchr:
strrchr:
  
  push rdi
  push rsi

  call _strlen

  mov rdx, rax

  pop rcx
  pop rax

  mov rsi, rax  ; Start of string
  add rax, rdx

loop:
 cmp [rax], cl 
 je strrchr_end

 cmp rax, rsi
 je eol

 dec rax

 jmp loop
  
eol:
  xor rax, rax


strrchr_end:
  ret                   ; Return control
