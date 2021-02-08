SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL memchr
%endif
	GLOBAL _memchr

_memchr:
memchr:
    ; Function arguments:
    ; - RDI: Pointer to string
    ; - SIL: Character (u8) we are searching for
    ; - RDX: Number of bytes in RDI to search
    ; Register layout:
    ; - RCX: Counter variable
    ; - RAX: Return address
    ; - R8:  Scratch register

  .prologue:
    xor rcx, rcx        ; rcx := 0
    xor rax, rax        ; rax := 0

  .do:
    mov r8,  [rdi + rcx]
    inc rcx
    and r8,  255        ; Mask only LSB
    cmp sil, r8b
    jne .while          ; If sil != r8, continue
    lea rax, [rdi + rcx]
    jmp .return

  .while:
    cmp rcx, rdx
    jb  .do
    mov rax, 0          ; rax := 0

  .return:
    ret                 ; Return control
