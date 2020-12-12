SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strlen
%endif
	GLOBAL _strlen

_strlen:
strlen:
  mov  rax, 0
; Naive implementation -- TODO improve this along the lines of glibc.
; Strictly, I believe that only 2-byte alignment is necessary (bc SSE?),
; but why not go the extra step ngl.
strlen_loop:
  cmp  byte [rdi + 0], 0
  jz   strlen_end
  inc  rax
  cmp  byte [rdi + 1], 0
  jz   strlen_end
  inc  rax
  cmp  byte [rdi + 2], 0
  jz   strlen_end
  inc  rax
  cmp  byte [rdi + 3], 0
  jz   strlen_end
  inc  rax
  add  rdi, 4
  jmp  strlen_loop
strlen_end:
  ret                   ; Return control
