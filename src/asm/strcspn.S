SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strcspn
%endif
	GLOBAL _strcspn

_strcspn:
strcspn:
	mov eax, 0
  ret                   ; Return control
