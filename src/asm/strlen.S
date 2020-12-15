SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strlen
%endif
	GLOBAL _strlen

; TODO try out SSE instructions on this sometime
_strlen:
strlen:
  mov  rax, 0
.qword_loop_prelude:
  jmp  .qword_loop_body
.qword_loop:
  add  rax, 8
  add  rdi, 8
.qword_loop_body:
  mov  r11, qword [rdi]
  mov  r13, 0x0101010101010101
  sub  r11, r13

  mov  r12, qword [rdi]
  not  r12
  mov  r13, 0x8080808080808080
  and  r11, r13

  and  r11, r12
  jz   .qword_loop
.dword_loop_prelude:
  jmp .dword_loop_body
.dword_loop:
  add rax, 4
  add rdi, 4
.dword_loop_body:
  mov  r11d, dword [rdi]
  mov  r13d, 0x01010101
  sub  r11d, r13d

  mov  r12d, dword [rdi]
  not  r12d
  mov  r13d, 0x80808080
  and  r11d, r13d

  and  r11d, r12d
  jz   .dword_loop
.byte_scan:
  cmp  byte [rdi + 0], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 1], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 2], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 3], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 4], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 5], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 6], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 7], 0
  jz   .end
  inc rax
  cmp  byte [rdi + 8], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 9], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 10], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 11], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 12], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 13], 0
  jz   .end
  inc  rax
  cmp  byte [rdi + 14], 0
  jz   .end
  inc  rax
  ; Unnecessary since we know the \0 is within this dword
  ;cmp  byte [rdi + 15], 0
  ;jz   .end
  ;inc  rax
.end:
  ret                   ; Return control
