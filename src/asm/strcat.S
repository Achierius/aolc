SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strcat
%endif
	GLOBAL _strcat

_strcat:
strcat:
	mov eax, 0
  ret                   ; Return control
