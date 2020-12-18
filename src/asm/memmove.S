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
    ; - RDX:    Number of bytes to copy

    ; We can use qword-string instructions to shorten the
    ; total number of instructions. 
    mov rcx, rdx
    shr rcx, 3          ; RCX: Number of QWORDS
    cmp rcx, 0
    ; Lines 21-23 may not be necessary.
    je  bytes_loop      ; If there are no QWORDS, go byte-by-byte.
qwords_loop:
    rep movsq           ; Move RCX quadwords from [RSI] to [RDI]
bytes_loop:
    mov rcx, rdx
    and rcx, 7          ; RCX: Number of bytes modulo 8
                        ; (ie. remainder after qwords)
    rep movsb           ; Move RCX bytes from [RSI] to [RDI]
  ret                   ; Return control
