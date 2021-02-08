SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
  GLOBAL strxfrm
%endif
  GLOBAL _strxfrm

_strxfrm:
strxfrm:
	mov eax, 0
  ret                   ; Return control
