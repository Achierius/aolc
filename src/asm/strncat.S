SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strncat
%endif
	GLOBAL _strncat

extern _strlen

_strncat:
strncat:
  push rdi  ; Destination
  push rsi  ; Source
  push rdx  ; n

  call _strlen
  mov r9, rax

  pop r8   ; n_counter
  pop rcx  ; Working source
  pop rdx  ; Working destination

  mov rax, rdx  ; Address of destination (return value)
  add rdx, r9   ; Null terminator of destination

.loop:
  cmp r8b, 0  ; Check n_counter > 0 to keep going
  je .n_0

  mov rsi, [rcx]  ; Move source char to destination
  mov [rdx], rsi

  cmp sil, 0  ; Check if end of source
  je .end

  dec r8
  inc rcx
  inc rdx

  jmp .loop

.n_0:  ; Handle adding null terminator when n_counter = 0
 xor r10, r10
 cmp [rdx], r10b  ; Check if null terminator already exists here,
 je .end       ; avoiding possible overflow if n = 0

 inc rdx
 mov [rdx], r10b  

.end:
  ret                   ; Return control
