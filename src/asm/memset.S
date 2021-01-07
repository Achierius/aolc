SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memset
%endif
	GLOBAL _memset

_memset:
memset:
	mov  rax, rdi
  mov  ecx, edx
  mov  rdx, rsi         ; idk if we can access SIL (the 1B register for RSI) but we definitely can access DL

  ; Register Contents:
  ;  - RAX: Return value (address of string passed in)
  ;  - RDI: Pointer to string addr
  ;  -  DL: Byte to write (we only want the first 8b of EDX => use DL)
  ;  - ECX: Number of bytes to set; this reg is used by LOOP
  cmp  ecx, 0
  je   memset_end
memset_loop:
  mov  [rdi], dl
  inc  rdi
  loop memset_loop
memset_end:
  ret                   ; Return control
