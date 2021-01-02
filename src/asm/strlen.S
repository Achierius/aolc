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
  mov          ecx, edi
  and          ecx, 1111B
  mov          eax, ecx
  jz           sse_scan
  cmp          ecx, 1000B
  jge          byte_scan_upper


byte_scan_lower:
  mov          rdx, [rdi]
.loop:
  cmp          dl,  0      ; Check 0th byte for null
  jz           .end
  shr          rdx, 8
  add           cl, 0001B
  cmp           cl, 1000B
  jne          .loop
.done:
  add          rdi, rcx
  sub          rdi, rax
  xor          rcx, rcx
  jmp          byte_scan_upper
.end:
  sub          ecx, eax
  mov          eax, ecx
  ret


byte_scan_upper:
  mov          rdx, [rdi]
.loop:
  cmp          dl,  0
  jz           .end
  shr          rdx, 8
  add           cl, 0001B
  jnc          .loop
.done:
  ;sub          ecx, eax
  ;mov          eax, ecx
  ;add          rdi, rax
  ;sub          rdi, 8
  add          eax, 8
  sub          eax, ecx
  add          rdi, 8
  sub          rdi, rcx
  ;mov          rax, rdi
  ;ret
  jmp          sse_scan
.end:
  cmp          ecx, 16
  je           .done
  sub          ecx, eax
  mov          eax, ecx
  ret


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
  add          rdi,  16    ; Advance RDI pointer
  pcmpeqb      xmm1, xmm0  ; Compare each byte against '\0'
  pmovmskb     edx,  xmm1  ; Pull the leading bit from each byte and put it in %dx (yes, %dx);
  tzcnt        cx,   dx    ;  then count the number of trailing zeros. If any are 1, we hit a \n.
  add          eax,  ecx   ; Add the number of trailing non-null bytes to our retval accumulator
  cmp          cx,   16    ; If all 16 bits are 0, we haven't hit one yet --
  je           .loop   ;  continue looping.

done:
  ret                      ; Return control
