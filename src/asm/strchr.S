SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strchr
%endif
	GLOBAL _strchr

_strchr:
strchr:
  mov rdi, rax

loop:
  cmp qword [rax], 0
  je eol

  cmp [rax], rsi
  je strchr_end

  inc rax
  
  jmp loop

eol:
  xor rax, rax

strchr_end:
  ret                   ; Return control
