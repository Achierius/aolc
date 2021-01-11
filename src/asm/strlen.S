%use smartalign

SECTION .data
  lomagic DB 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
  himagic DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80

ALIGN 16
SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strlen
%endiF
	GLOBAL _strlen

  ; General strategy:
  ;  1. Align to 8 and then 16-byte boundary with 8-byte (qword) str-read and null checks (label 'byte_scan')
  ; `2. If necessary, use a single SSE (16-byte vector op) to align to a 32-byte boundary (label 'sse_scan')
  ;  3. Loop over the remaining length of the string using AVX2 256b (32B) vector instructions (label 'avx2_scan')
  ; This is necessary because aligned vector reads from memory (e.g. vmovdqa) require their address be 16B (SSE) / 32B (AVX)
  ; aligned, and unaligned reads (e.g. vmovdqu) are notably slower.
_strlen:
strlen:
  xorps        xmm0, xmm0
  mov          rsi, rdi
  test         dil, 0x0F     ; Check for alignment on a 16-byte boundary
  jz           sse_scan      ;  skip alignment if so, go straight to vector-ops

  ; Uses standard bit hacks to read 8 bytes at a time
  ; (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)
  ; rcx will contain the ((v) - 0x01010101UL); rdx the ~(v) & (0x80808080UL).
byte_scan:
  movdqu       xmm2, [rdi]
  pcmpeqb      xmm2, xmm0
  ptest        xmm0, xmm2
  jc           sse_scan
.null_byte:
  pmovmskb     edx,  xmm2
  bsf          ecx,  edx
  add          rdi,  rcx
  jmp          done

; Uses SSE instructions to read 16 bytes at a time;
; we only do this at most once in order to align to a 32B boundary, after which point we fall through to AVX.
ALIGN 16
sse_scan:
  add          rdi,  15
  and          dil,  0xF0

  bt           rdi, 4       ; 'byte_scan' guarantees 16B alignment; this checks if it also gave us 32B alignment --
  jnc          avx2_scan    ;   if so, skip SSE entirely.
  xor          rcx,  rcx

  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - XMM0: 16-null-byte-mask (aka all zeroes)
  ;  - XMM1: 128-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
  movdqa       xmm1, [rdi]  ; Load 16 bytes of string into register
  pcmpeqb      xmm1, xmm0   ; Compare each byte against '\0'
  pmovmskb     edx,  xmm1   ; Pull the leading bit from each byte in xmm1 -- 1 indicates a '\0' match
  cmp          edx,  0      ; If the whole register is 0, then we ran into no null terminators --
  jnz          .null_byte   ;  if it's not, then goto handler
  add          rdi,  16     ; If so though, then just add 16 and move on to AVX
  jmp          avx2_scan

.null_byte:
  bsf          ecx,  edx     ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add them to our count,
  jmp          done         ;  and return.
  

; Was experiencing massive slowdowns without this present --
; has to do with L1i$ line size
ALIGN 64
avx2_scan:
  vxorps       ymm0, ymm0
;  mov          rax, 0xFF
;  jmp .loop_body            ; Small loop optimization (necessary to skip the ptr-advancement on the first loop)
  sub          rdi,  128
  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - YMM0: 32-null-byte-mask (aka all zeroes)
  ;  - YMM1: 256-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
ALIGN 16
.loop:
  add          rdi,  128         ; Advance string pointer -- skipped on first pass
.loop_body:
  vmovdqa      ymm3, [rdi +  0]  ; Load 32 bytes of string into register
  vpcmpeqb     ymm3, ymm0        ; Compare each byte against '\0'
  vmovdqa      ymm5, [rdi + 32]  ; Load 32 bytes of string into register
  vpcmpeqb     ymm5, ymm0        ; Compare each byte against '\0'

  vmovdqa      ymm4, [rdi + 64]  ; Load 32 bytes of string into register
  vpcmpeqb     ymm4, ymm0        ; Compare each byte against '\0'
  vmovdqa      ymm6, [rdi + 96]  ; Load 32 bytes of string into register
  vpcmpeqb     ymm6, ymm0        ; Compare each byte against '\0'

  vpmaxub      ymm1, ymm3, ymm5
  vpmaxub      ymm2, ymm4, ymm6

  vpmaxub      ymm2, ymm1
  vptest       ymm0, ymm2
  jc           .loop

avx2_nullcheck:
.bytes_0~32:
  vptest       ymm0, ymm3
  jc .bytes_32~64
  add          rdi,  0
  vpmovmskb    rdx,  ymm3
  jmp .extract_byte
.bytes_32~64:
  vptest       ymm0, ymm5
  jc .bytes_64~96
  add          rdi,  32
  vpmovmskb    rdx,  ymm5
  jmp .extract_byte
.bytes_64~96:
  vptest       ymm0, ymm4
  jc .bytes_96~128
  add          rdi,  64
  vpmovmskb    rdx,  ymm4
  jmp .extract_byte
.bytes_96~128:
 ;vptest       ymm0, ymm6
 ;jc .nowhere_lol
  add          rdi,  96
  vpmovmskb    rdx,  ymm6
 ;jmp .extract_byte
.extract_byte:
  bsf          rcx,  rdx    ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add then to our count,
 ;jmp          done         ;  and fall through to return.

done:
  mov          rax, rdi    ; The string length is equal to our current ptr
  sub          rax, rsi    ;   minus the stored value of the string base ptr
  vzeroall                 ; Architecture-required or we'll kill succeeding programs
  ret                      ; Return control
