SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strchr
%endif
	GLOBAL _strchr

_strchr:
strchr:
	mov eax, 0
  ret                   ; Return control
