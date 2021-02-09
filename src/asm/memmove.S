SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
  GLOBAL memmove:
%endif
	GLOBAL _memmove

_memmove:
memmove:
    ; Function arguments:
    ; - RDI:    Dest pointer
    ; - RSI:    Src pointer
    ; - RDX:    Number of bytes to copy
    ; Register layout
    ; - RAX:    Original dest addr
    ; - RCX:    Count variable

  .prologue:
    mov rcx, rdx        ; Count variable
    mov rax, rdi        ; Save rdi
    cmp rdi, rsi

    ; If rdi > rsi, we need to determine whether there
    ; is overlap. Otherwise, we can use fast forwards
    ; byte-by-byte moves.
    je .epilogue
    ja .try_rev

  .fwds
    cld
    rep movsb           ; rdi < rsi
    jmp .epilogue

  .try_rev:
    add rsi, rcx        ; rsi += rcx
    cmp rdi, rsi
    sub rsi, rcx        ; Reset rsi

    ; If rdi > last byte to copy, forwards copy works.
    ja .fwds

  .rev:
    std
    mov rcx, rdx        ; Fix rcx
    sub rdx, 1          ; rdx -= 1 to omit an instruction
    add rsi, rcx
    add rdi, rcx

    rep movsb
    cld

  .epilogue:
    ret
