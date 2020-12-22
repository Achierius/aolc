SECTION .DATA

SECTION .TEXT
%ifdef OVERRIDE_LIBC_NAMES 
  GLOBAL memmove:
%endif
	GLOBAL _memmove

_memmove:
memmove:
    ; Function arguments:
    ; - RDI:    Dest pointer
    ; - RSI:    Src pointer
    ; - EDX:    Number of bytes to copy

    ; Register layout
    ; - rdi:    Destination address
    ; - rsi:    Source address
    ; - rax:    Scratch
    ; - bh:     Whether we are reversing
    ; - rcx:    Counter
    ; - edx:    Raw n. bytes to copy

    push rdi            ; Save the dest addr
    push rbx            ; We need another register

    mov rcx, rax      ; rcx := rdx, ie. number of bytes

    cmp rdi, rsi        ; Check if rdi > rsi, if so, we decrement addrs
    jna not_above

    std                 ; Set direction flag
    mov bh, 1           ; Set local direction flag
    add rdi, rcx
    sub rdi, 1          ; rdi += rcx - 1
    add rsi, rcx
    sub rsi, 1          ; rsi += rcx - 1

not_above:
    cld                 ; Unset direction flag

qwords_check:
    shr rcx, 3          ; rcx = rcx >> 3
    cmp rcx, 0          
    jne bytes           ; If <64B to copy, skip qw section

qwords:
    cmp bh, 1           ; First, determine if we are descending or ascending.
    jne .fwds     ; Skip padding and unpadding process?

.in_rev:
    sub rdi, 7          ; rdi -= 7
    sub rsi, 7          ; rsi -= 7
    rep movsq           ; Move qwords
    add rdi, 7          ; Prepare for bytes
    add rsi, 7          
    jmp .epilogue

.fwds:
    rep movsq

.epilogue:
    mov rcx, rax        ; Reset counter to default value
    and rcx, 7          ; rcx &= 0b0...0111 = # bytes - # qw * 64
    
bytes:
    ; Check if we have bytes to copy
    cmp rcx, 0
    jne epilogue        ; If not, go straight to epilogue
    rep movsb           ; Move bytes

epilogue:
    pop rbx
    pop rax        ; Return the dest addr
  ret                   ; Return control
