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

.loop:
  cmp [rax], dl  ; Look for character before null byte
  je strchr_end

  cmp [rax], cl
  je eol

  inc rax
  
  jmp .loop

eol:
  xor rax, rax

strchr_end:
  ret                   ; Return control
