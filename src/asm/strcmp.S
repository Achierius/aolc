SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strcmp
%endif
	GLOBAL _strcmp

_strcmp:
strcmp:
	mov eax, 0
  ret                   ; Return control
