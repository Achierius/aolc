SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strrchr
%endif
	GLOBAL _strrchr

_strrchr:
strrchr:
	mov eax, 0
  ret                   ; Return control
