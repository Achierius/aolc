SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strerror
%endif
	GLOBAL _strerror

_strerror:
strerror:
	mov eax, 0
  ret                   ; Return control
