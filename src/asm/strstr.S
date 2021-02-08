SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strstr
%endif
	GLOBAL _strstr:

_strstr:
strstr:
	mov eax, 0
  ret                   ; Return control
