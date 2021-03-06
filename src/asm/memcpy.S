SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memcpy
%endif
	GLOBAL _memcpy

_memcpy:
memcpy:
	mov    rax, rdi               ; Set return value of function
  cmp    rdx, 0                 ; Check if len == 0
  jz     done                   ;  if so, return; otherwise, continue
.lencheck:
  cmp    rdx, 8                 ; If <8 bytes to write,
  jb     write_bytes            ;  don't bother with writing qwords, just write each byte directly

write_qwords:
  xor    ecx, ecx               ; This next chunk takes the first byte of %rsi and fills all 8 bytes of %rsi with it

  test   rdx, 0111B             ; If already aligned, skip down to the qword loop
  jz     .loop_prelude

  ; Register Contents:
  ;  - RAX: Return value (address of string passed in)
  ;  - RDI: Pointer to dest string addr
  ;  - RSI: 8 copies of the byte we want to write:
  ;    SIL: 1 copy
  ;     SI: 2 copies
  ;    ESI: 4 copies
  ;  - RDX: Number of bytes to write
.align_1:                       ; Here we aren't aligning the dest ptr, but the count -- want to write _8_B at a time!
  bt     rdx, 0                 ; i.e. xxx1B -> xxx0B
  jnc    .align_2
  mov    cl, [rsi]
  mov    [rdi], cl
  add    rdi, 1
  add    rsi, 1
  sub    rdx, 1
.align_2:
  bt     rdx, 1                 ; i.e. xx10B -> xx00B
  jnc    .align_4
  mov    cx, [rsi]
  mov    [rdi], cx
  add    rdi, 2
  add    rsi, 2
  sub    rdx, 2
.align_4:
  bt     rdx, 2                 ; i.e. x100B -> x000B -- 8 byte aligned!!
  jnc    .loop_prelude
  mov    ecx, [rsi]
  mov    [rdi], ecx
  add    rdi, 4
  add    rsi, 4
  sub    rdx, 4

.loop_prelude:
  add    rdx, rdi
  ; Register Contents:
  ;  - RAX: Return value (address of string passed in)
  ;  - RDI: Pointer to dest string addr
  ;  - RSI: 8 bytes of the byte we want to write
  ;  - RDX: One-past-the-end pointer, bounds our loop
ALIGN 16                        ; Write qwords until we're done; the alignment above ensures that we have a multiple
.loop:                          ; of 8 bytes to write, so once (%rdi > %rdx) we're done!
  mov    rcx, [rsi]             ; Write qword (8 bytes) to memory
  mov    [rdi], rcx             ; Write qword (8 bytes) to memory
  add    rdi, 8                 ; Advance destination memory pointer
  add    rsi, 8                 ; Advance destination memory pointer
  cmp    rdi, rdx               ; Check loop bounds
  jb     .loop                  ; Continue looping if within bounds
.end:                           ; Otherwise fall through to end
  ret                           ;  and return control

write_bytes:
.loop:
  mov    cl, [rsi]             ; Write bytes one at a time
  mov    [rdi], cl             ; Write bytes one at a time
  add    rdi, 1                 ; Advance destination memory pointer
  add    rsi, 1                 ; Advance destination memory pointer
  sub    rdx, 1                 ; Decrement # of bytes left to write
  jnz    .loop                  ; Keep looping until no more bytes left to write
.end:                           ; Once done, fall through to end
done:
  ret                           ;  and return control
