SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
  GLOBAL memmove:
%endif
	GLOBAL _memmove

_memmove:
memmove:
	mov eax, 5
  ret                   ; Return control
