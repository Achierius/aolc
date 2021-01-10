SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memcpy
%endif
	GLOBAL _memcpy

_memcpy:
memcpy:
	mov  rax, rdi
  add  rdx, rdi

  ; Register Uses:
  ;  - RAX:  Return value (address of string passed in)
  ;  - RDI:  Pointer to dest memory region
  ;  - RSI:  Pointer to src memory region
  ;  - RDX:  Pointer to last address to copy + 1
  ;  - R11b: Temporary storage
  cmp  rdi, rdx
  je   .end
.loop:
  mov  r11b, [rsi]      ; Read byte from src memory
  mov  [rdi], r11b      ; Write byte to dest memory
  inc  rsi              ; Advance pointers
  inc  rdi
  cmp  rdi, rdx         ; Check loop bounds
  jb   .loop
.end:
  ret                   ; Return control
