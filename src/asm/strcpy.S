SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
GLOBAL strcpy
%endif
GLOBAL _strcpy


_strcpy:
strcpy:
	mov  rax, rdi
  mov  ecx, edx
  mov  rdx, rsi

  ; Register Contents:
  ;  - RAX:  Return value (address of string passed in)
  ;  - RDI:  Pointer to dest string
  ;  - RDX:  Pointer to src string
  ;  - ECX:  Number of bytes to set; this reg is used by LOOP
  ;  - : Temporary storage, specifically the last byte -- BL

strcpy_loop:
  mov  bl, [rdx]        ; Read byte from memory.
  mov  [rdi], bl        ; Write back to memory.
  cmp  bl, 0            ; Check if we've hit a null terminator -- if so, end.
  je   strcpy_end      
  inc  rdx              ; Increment dest-string pointer.
  inc  rdi              ; Increment src-string pointer.
  jmp  strcpy_loop     ; Loop until we hit \0 (or segfault)

strcpy_end:
  ret                   ; Return control
