SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strtok
%endif
	GLOBAL _strtok

_strtok:
strtok:
	mov eax, 0
  ret                   ; Return control
