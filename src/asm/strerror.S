SECTION .DATA

SECTION .TEXT
	GLOBAL strerror

strerror:
	mov eax, 0
  ret                   ; Return control
