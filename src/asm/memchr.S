SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memchr
%endif
	GLOBAL _memchr

_memchr:
memchr:
	mov eax, 0
  ret                   ; Return control
