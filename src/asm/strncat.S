SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strncat
%endif
	GLOBAL _strncat

_strncat:
strncat:
	mov eax, 0
  ret                   ; Return control
