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
  cmp          rdi,  0     ; Check for null pointer
  jz           done
  mov          rsi, rdi
  mov           cl, dil
  and           cl, 0x0F
  jz           sse_scan
  cmp           cl, 0x08
  jge           byte_scan_upper

  ;cmp          ecx, 1000B
  ;jge          byte_scan_upper

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
  cmp          dil, 0
  jz           sse_scan
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
  mov          cl, dil
  and          cl, 0x0F
  jz           sse_scan

sse_scan:
  xor          r10,  r10
  vmovd        xmm0, r10d  ; Create 128-byte 0 register for null-byte checking later on
  xor          rcx,  rcx
  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - XMM0: 16-null-byte-mask (aka all zeroes)
  ;  - XMM1: 128-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
.loop:
  vmovdqa      xmm1, [rdi] ; Load 16 bytes of string into register
  pcmpeqb      xmm1, xmm0  ; Compare each byte against '\0'
  pmovmskb     edx,  xmm1  ; Pull the leading bit from each byte and put it in %dx (yes, %dx);
  tzcnt        cx,   dx    ;  then count the number of trailing zeros. If any are 1, we hit a \n.
  add          rdi,  rcx   ; Add the number of trailing non-null bytes to our retval accumulator
  cmp          cx,   16    ; If all 16 bits are 0, we haven't hit one yet --
  je           .loop       ;  continue looping.

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
