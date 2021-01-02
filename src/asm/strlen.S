SECTION .DATA
  lomagic DB 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
  himagic DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

ALIGN 16
SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strlen
%endif
	GLOBAL _strlen

_strlen:
strlen:
  cmp          rdi,  0
  jz           done
  mov          rsi, rdi
  mov           cl, dil
  and           cl, 0x0F
  jz           sse_scan
  cmp           cl, 0x08
  jge           byte_scan_upper

byte_scan_lower:
  mov          rdx, [rdi]
.byte0:
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte1:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte2:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte3:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte4:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte5:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte6:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
.byte7:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1

byte_scan_upper:
  mov          rdx, [rdi]
.byte0:
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte1:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte2:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte3:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte4:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte5:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte6:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan
.byte7:
  shr          rdx, 8
  cmp          dl,  0
  jz           done
  add          rdi, 1
  ;mov          cl, dil
  ;and          cl, 0x0F
  ;jz           sse_scan

sse_scan:
  xor          rcx,  rcx
  xorps        xmm0, xmm0
  jmp .loop_body
  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - XMM0: 16-null-byte-mask (aka all zeroes)
  ;  - XMM1: 128-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
ALIGN 16
.loop:
  add          rdi,  16     ; Advance string pointer -- skipped on first pass
.loop_body:
  movdqa       xmm1, [rdi]  ; Load 16 bytes of string into register
  pcmpeqb      xmm1, xmm0   ; Compare each byte against '\0'
  pmovmskb     edx,  xmm1   ; Pull the leading bit from each byte in xmm1 -- 1 indicates a '\0' match
  cmp          edx,  0      ; If the whole register is 0, then we ran into no null terminators --
  jz           .loop;  continue looping.
  bsf          cx,   dx     ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add then to our count,
 ;jmp          .done        ;  and fall through to return.

done:
  mov          rax, rdi
  sub          rax, rsi
  ret                      ; Return control

DEBUG:
  ;mov rax, rdx
 ; mov rax, rsi
  mov rax, rdi
  sub rax, rsi

  ;and rax, 0x0F
  ret

DIE:
  mov rax, [0]
