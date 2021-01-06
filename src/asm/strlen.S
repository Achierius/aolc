%use smartalign

SECTION .DATA
  lomagic DB 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
  himagic DB 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
  leadstr DB 'val: '
  leadlen EQU $-leadstr
  endlstr DB 0xa
  endllen EQU $-endlstr
  n0str   DB '0'
  n0len   EQU $-n0str
  n1str   DB '1'
  n1len   EQU $-n1str

ALIGN 16
SECTION .TEXT
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
  mov          rsi, rdi
  test         dil, 0x0F     ; Check for alignment on a 16-byte boundary
  jz           sse_scan      ;  skip alignment if so, go straight to vector-ops

  ; Uses standard bit hacks to read 8 bytes at a time
  ; (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)
  ; rcx will contain the ((v) - 0x01010101UL); rdx the ~(v) & (0x80808080UL).
byte_scan:
  test         dil, 0x07     ; Check for 8-byte alignment
  jz           .upper
.lower:                      ; Align onto an 8-byte boundary
  mov          rdx, [rdi]    ; Loads the target string
  mov          rcx,  rdx
  ; Begin bit hack math
  not          rdx
  and          rdx, [himagic]
  sub          rcx, [lomagic]; The result will have a 1 in the MSB of each byte that was '\0' in the original string
  and          rdx, rcx      ; 
  ; End bit hack math        ;
  jnz          .null_byte    ; Goto null byte handler if no such 1s are present (qword is all 0);
                             ; Otherwise, note that we still read a full 8 bytes, even if we were <8 from the 16B boundary --
  add          rdi, 8        ;  as such, after we (here) advance the source-string pointer by 8,
  and          dil, 11111000B;  discard any bytes beyond the boundary; yes is a waste, but a small one 
 ;jmp          .upper        ;  and fall through to the upper-qword handler.


.upper:                      ; Align onto a 16-byte boundary -- see above comments for details
  mov          rdx, [rdi]
  mov          rcx,  rdx
  ; Begin bit hack math
  not          rdx
  and          rdx, [himagic]
  sub          rcx, [lomagic]
  and          rdx, rcx
  ; End bit hack math
  jnz          .null_byte    ; Goto null byte handler if no such 1s are present (qword is all 0);
  add          rdi, 8        ;  otherwise advance pointer by 8,
  jmp sse_scan               ;  and proceed to vector operations.

ALIGN 16
.null_byte:
  bsf          rcx,  rdx     ; Otherwise, count how many leading non-null bits we had,
  shr          rcx,  3       ;  convert from bits to bytes (e.g., 0x00008000 corresponds to _1_ byte before '\0')
  add          rdi,  rcx     ;  add them to our count,
  jmp          done          ;  and fall through to return.


; Uses SSE instructions to read 16 bytes at a time;
; we only do this at most once in order to align to a 32B boundary, after which point we fall through to AVX.
ALIGN 16
sse_scan:
  bt           rdi, 4       ; 'byte_scan' guarantees 16B alignment; this checks if it also gave us 32B alignment --
  jnc          avx2_scan    ;   if so, skip SSE entirely.
  xor          rcx,  rcx
  xorps        xmm0, xmm0

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
  jmp .loop_body            ; Small loop optimization (necessary to skip the ptr-advancement on the first loop)
  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - YMM0: 32-null-byte-mask (aka all zeroes)
  ;  - YMM1: 256-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
ALIGN 16
.loop:
  add          rdi,  32     ; Advance string pointer -- skipped on first pass
.loop_body:
  vmovdqa      ymm1, [rdi]  ; Load 32 bytes of string into register
  vpcmpeqb     ymm1, ymm0   ; Compare each byte against '\0'
  vpmovmskb    rdx,  ymm1   ; Pull the leading bit from each byte in xmm1 -- 1 indicates a '\0' match
  cmp          rdx,  0      ; If the whole register is 0, then we ran into no null terminators --
  jz           .loop        ;  continue looping.
  bsf          rcx,  rdx    ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add then to our count,
  jmp          done         ;  and fall through to return.


done:
  mov          rax, rdi    ; The string length is equal to our current ptr
  sub          rax, rsi    ;   minus the stored value of the string base ptr
  vzeroall                 ; Architecture-required or we'll kill succeeding programs
  ret                      ; Return control



; DEBUG CODE FOLLOWS

PRINTDIE0:
  mov edx, 1
  mov ecx, n0str
  mov ebx, 1
  mov eax, 4
  int 0x80
  mov edx, endllen ;message length
  mov ecx, endlstr ;message to write
  mov ebx,1          ;file descriptor (stdout)
  mov eax,4          ;system call number (sys_write)
  int 0x80           ;call kernel
  jmp PRINTDIE
PRINTDIE1:
  mov edx, 1
  mov ecx, n1str
  mov ebx, 1
  mov eax, 4
  int 0x80
  mov edx, endllen ;message length
  mov ecx, endlstr ;message to write
  mov ebx,1          ;file descriptor (stdout)
  mov eax,4          ;system call number (sys_write)
  int 0x80           ;call kernel
  jmp PRINTDIE
PRINTDIE:
  mov r11, rdi
.print_header:
  mov edx, leadlen ;message length
  mov ecx, leadstr   ;message to write
  mov ebx,1          ;file descriptor (stdout)
  mov eax,4          ;system call number (sys_write)
  int 0x80           ;call kernel
.print_dil:
  xor r12, r12
  mov r12w, 16
.loop:
  cmp r12w, 0
  jz  .done
  sub r12w, 1
  bt  r11w, r12w
  jc .print_1
.print_0:
  mov edx, 1
  mov ecx, n0str
  mov ebx, 1
  mov eax, 4
  int 0x80
  jmp .loop
.print_1:
  mov edx, 1
  mov ecx, n1str
  mov ebx, 1
  mov eax, 4
  int 0x80
  jmp .loop
.done:
  mov edx, endllen ;message length
  mov ecx, endlstr   ;message to write
  mov ebx,1          ;file descriptor (stdout)
  mov eax,4          ;system call number (sys_write)
  int 0x80           ;call kernel

  mov rax, rdi
  sub rax, rsi
  vzeroall
  ret
  mov eax, 1
  int 0x80


DIE:
  mov rax, [0]
