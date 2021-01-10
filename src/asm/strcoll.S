SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strcoll
%endif
	GLOBAL _strcoll

_strcoll:
strcoll:
	mov eax, 0
  ret                   ; Return control
