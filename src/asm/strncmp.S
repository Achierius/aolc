SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strncmp
%endif
	GLOBAL _strncmp

_strncmp:
strncmp:
	mov eax, 0
  ret                   ; Return control
