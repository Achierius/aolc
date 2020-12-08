SECTION .DATA

SECTION .TEXT
	GLOBAL memcpy

memcpy:
	mov  rax, rdi
  mov  ecx, edx
  mov  rdx, rsi

  ; Register Contents:
  ;  - RAX:  Return value (address of string passed in)
  ;  - RDI:  Pointer to dest addr
  ;  - RDX:  Pointer to src addr
  ;  - ECX:  Number of bytes to set; this reg is used by LOOP
  ;  - R11b: Temporary storage
  cmp  ecx, 0
  je   memcpy_end
memcpy_loop:
  mov  bl, [rdx]
  mov  [rdi], bl
  inc  rdx
  inc  rdi
  loop memcpy_loop
memcpy_end:
  ret                   ; Return control
