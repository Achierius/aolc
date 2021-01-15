SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strchr
%endif
	GLOBAL _strchr

_strchr:
strchr:

  mov rax, rdi
  mov rdx, rsi
  xor rcx, rcx

loop:
  cmp [rax], rcx
  je eol

  cmp [rax], dl
  je strchr_end

  inc rax
  
  jmp loop

eol:
  xor rax, rax

strchr_end:
  ret                   ; Return control
