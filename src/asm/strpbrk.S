SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strpbrk
%endif
	GLOBAL _strpbrk

_strpbrk:
strpbrk:
	mov eax, 0
  ret                   ; Return control
