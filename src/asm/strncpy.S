SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strncpy
%endif
	GLOBAL _strncpy

_strncpy:
strncpy:
	mov  rax, rdi
  mov  ecx, edx
  mov  rdx, rsi

  ; Register Contents:
  ;  - RAX:  Return value (address of string passed in)
  ;  - RDI:  Pointer to dest string
  ;  - RDX:  Pointer to src string
  ;  - ECX:  Number of bytes to set; this reg is used by LOOP
  ;  - R11b: Temporary storage, specifically the last byte -- BL
  cmp  ecx, 0           ; Pre-emptive check so we don't go through the first
  je   strncpy_end      ; iteration of the loop when passed 0 as a length.

strncpy_loop:
  mov  bl, [rdx]        ; Read byte from memory.
  mov  [rdi], bl        ; Write back to memory.
  cmp  bl, 0            ; Check if we've hit a null terminator -- if so,
  je   strncpy_zloop    ; write zeroes for the rest of the length.
  inc  rdx              ; Increment dest-string pointer.
  inc  rdi              ; Increment src-string pointer.
  loop strncpy_loop     ; Loop 'n' times, as long as we don't hit \0.
  jmp  strncpy_end      ; If we finish looping while still copying, no need to
                        ; copy additional zeroes -- go to end.
strncpy_zloop:
  mov  [rdi], byte 0    ; Write null terminators for the remaining length
  inc  rdi
  loop strncpy_zloop

strncpy_end:
  ret                   ; Return control
