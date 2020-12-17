SECTION .DATA
  lomagic DB 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
  himagic DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

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
  sub  r11, [lomagic]

  mov  r12, qword [rdi]
  not  r12
  and  r11, [himagic]

  and  r11, r12
  jz   .qword_loop
.dword_loop_prelude:
  jmp .dword_loop_body
.dword_loop:
  add  rax, 4
  add  rdi, 4
.dword_loop_body:
  mov  r11d, dword [rdi]
  sub  r11d, [lomagic]

  mov  r12d, dword [rdi]
  not  r12d
  and  r11d, [himagic]

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
.end:
  ret                   ; Return control
