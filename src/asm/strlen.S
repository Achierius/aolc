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

_strlen:
strlen:
  mov          rsi, rdi
  test         dil, 0x0F
  jz           sse_scan

byte_scan:
  cmp           cl, 0x08
  jge           .upper
.lower:
  mov          rdx, [rdi]
  mov          rcx,  rdx
  
  ; eqn:
  ; (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)
  ; rcx will contain the ((v) - 0x01010101UL); rdx the ~(v) & (0x80808080UL).
  not          rdx
  and          rdx, [himagic]
  sub          rcx, [lomagic]
  and          rdx, rcx
  jnz          .null_byte
  add          rdi, 8
 ;jmp          .upper        ; Fall through

.upper:
  mov          rdx, [rdi]
  mov          rcx,  rdx
  
  ; eqn:
  ; (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)
  ; rcx will contain the ((v) - 0x01010101UL); rdx the ~(v) & (0x80808080UL).
  not          rdx
  and          rdx, [himagic]
  sub          rcx, [lomagic]
  and          rdx, rcx
  jnz          .null_byte
  add          rdi, 8
  and          dil, 0xF0
  jmp sse_scan

ALIGN 16
.null_byte:
  bsf          rcx,  rdx     ; Otherwise, count how many leading non-null bytes we had,
  shr          rcx,  3
  add          rdi,  rcx     ;  add then to our count,
  jmp          done          ;  and fall through to return.

; Uses SSE instructions to read 16 bytes at a time
ALIGN 16
sse_scan:
  bt           rdi, 4
  jnc          avx2_scan
  xor          rcx,  rcx
  xorps        xmm0, xmm0
 ;jmp .loop_body

  ; Register Uses:
  ;  - RDI:  (Pre-) Pointer to string to search in
  ;  - XMM0: 16-null-byte-mask (aka all zeroes)
  ;  - XMM1: 128-byte chunk of string, results of pcmpeqb
  ;  - RDX:  Generated comparison-to-null-byte bitmask
  ;  - RCX:  Intermediate values
.loop_body:
  movdqa       xmm1, [rdi]  ; Load 16 bytes of string into register
  pcmpeqb      xmm1, xmm0   ; Compare each byte against '\0'
  pmovmskb     edx,  xmm1   ; Pull the leading bit from each byte in xmm1 -- 1 indicates a '\0' match
  cmp          edx,  0      ; If the whole register is 0, then we ran into no null terminators --
  jnz          .null_byte   ;  if it's not, then goto handler
  add          rdi,  16     ; If so though, then just add 16 and move on to AVX
  jmp          avx2_scan

.null_byte:
  bsf          cx,   dx     ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add then to our count,
  jmp          done         ;  and return.
  

ALIGN 64
avx2_scan:
  vxorps       ymm0, ymm0
  jmp .loop_body
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
  vmovdqa      ymm1, [rdi]  ; Load 16 bytes of string into register
  vpcmpeqb     ymm1, ymm0   ; Compare each byte against '\0'
  vpmovmskb    rdx,  ymm1   ; Pull the leading bit from each byte in xmm1 -- 1 indicates a '\0' match
  cmp          rdx,  0      ; If the whole register is 0, then we ran into no null terminators --
  jz           .loop;  continue looping.
  bsf          rcx,  rdx     ; Otherwise, count how many leading non-null bytes we had,
  add          rdi,  rcx    ;  add then to our count,
  jmp          done        ;  and fall through to return.


done:
  vzeroall                 ;
  mov          rax, rdi    ; The string length is equal to our current ptr
  sub          rax, rsi    ;   minus the stored value of the string base ptr
  ret                      ; Return control




PRINTDIE0:
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
  mov r12w, 8
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

  mov eax, 1
  int 0x80


DIE:
  mov rax, [0]
