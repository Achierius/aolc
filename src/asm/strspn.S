SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strspn
%endif
	GLOBAL _strspn

_strspn:
strspn:
	mov eax, 0
  ret                   ; Return control
