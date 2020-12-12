SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strchr
%endif
	GLOBAL _strchr

_strchr:
strchr:
	mov eax, 0
  ret                   ; Return control
