SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memcmp
%endif
	GLOBAL _memcmp

_memcmp:
memcmp:
	mov eax, 0
  ret                   ; Return control
