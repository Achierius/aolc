SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strpbrk
%endif
	GLOBAL _strpbrk


  ; Register Contents:
  ;  - RDI:  Pointer to string to search in
  ;  - RSI:  Pointer to string of characters to search for

_strpbrk:
strpbrk:
  mov rcx, [rsi]      ; Load first few tokens into register for quick access
  cmp  cl, 0
  jz  .return_fail
  ;cmp  ch, 0          ; If only 1 token, bool logic not worth it;
  ;jmp .monotoken_loop ;  => goto special optimization loop


.makemagic:           ; make magic numbers which allow fast token checking
  ;xor rcx, rcx        ; cl = 'u'; will hold (!tok1 | !tok2 | ... | !tokN)
  mov rcx, 0x0FF         ; dl = 'v'; will hold (!tok1 & !tok2 & ... & !tokN)
  mov rdx, 0x0FF         ; dl = 'v'; will hold (!tok1 & !tok2 & ... & !tokN)
  add rsi, -1
.loop_makemagic:
  add rsi, 1          ; Advance token pointer
  mov  al, [rsi]      ; Load next token into scratch register
  cmp  al, 0          ; Check for null terminator;
  jz  .normal_loop    ;  if present, => end of token str; goto main body of code
  and  cl, al         ; u = u && token
  not  al
  and  dl, al         ; v = v && ~token
  jmp .loop_makemagic


.normal_loop:
  ; Register Contents:
  ;  - RDI: Ptr. to target string
  ;  - AL:  Scratch storage for target-string bytes
  ;  - R8B: Intermediate computation values
  ;  - CL:  'u' magic number
  ;  - DL:  'd' magic number
  add rdi, -1
.loop_str1:
  add rdi, 1       ; advance target string ptr
  mov  al, [rdi]   ; store character we want to check against tokens
  cmp  al, 0
  jz  .return_fail ; end of target string => return NULL
  mov r8b,  al
  and r8b,  dl
  not  al
  and  al,  cl
  or   al, r8b
  jnz .loop_str1


.return_succ:      ; If success, return current position of string ptr
  mov rax, rdi
  ret

.return_fail:      ; If fail, return NULL
  xor rax, rax     ; zero out return register
  ret
