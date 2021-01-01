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
  mov rax, 0
  vmovd xmm8, eax
.load_tokens:
  mov rax, [rsi] ; Load first few tokens into register for quick access
  cmp  al, 0
  jz  .return_fail
  vmovd xmm9, eax
  vpbroadcastb xmm0, xmm9
  jmp .eval_1token
  ;movzx edx,  al
  ;movd xmm1, edx

  ;cmp  al, 0
  ;add ecx, 1
  ;jz .eval_Ctokens
  ;movzx edx,  al
  ;movd xmm2, edx

.eval_1token:
  vmovdqa      xmm15, [rdi] ; 

  movdqa       xmm14, xmm15
  pcmpeqb      xmm14, xmm0 ; check for token
  pmovmskb     edx, xmm14
  tzcnt        cx, dx
  cmp cx, 16
  jne .success_null_check
  add rdi, 128
  ;jmp .normal_null_check

.normal_null_check:
  movdqa       xmm14, xmm15
  pcmpeqb      xmm14, xmm8
  pmovmskb     eax, xmm14
  tzcnt        dx, ax
  cmp dx, 16
  jne .return_fail
  jmp .eval_1token

.success_null_check:
  movdqa       xmm14, xmm15
  pcmpeqb      xmm14, xmm8
  pmovmskb     eax, xmm14
  tzcnt        dx, ax
  cmp cx, dx
  jge .return_fail
  jmp .return_succ
  
.eval_Ctokens:
  jmp .return_fail


;  and edx, 0x00FFFFFF ; Clip off the last byte, since we can't easily access it
;  add esi,  2      ; Number of permanently-stored token values
;
;  add rdi, -1      ; We subtract  
;  add esi, -1
;  mov r11, rdi     ; Move string ptr to storage location
;  mov edi, esi
;
;
;.normal_loop:
;  ; Register Contents:
;  ;  - EDI: Stored ptr. to (1 before) 3rd token in token string
;  ;  - ESI: Ptr. to interior of token string
;  ;  - R11: Ptr. to interior of target string
;  ;  - DL:  Container for 1st token
;  ;  - DH:  Container for 2nd token
;  ;  - CH:  Container for nth, current, token (n > 2)
;  ;  - CL:  Container for target-string byte currently under consideration
;.loop_str1:        ; Outer loop: 
;  add r11, 1       ; advance target string ptr
;  mov ecx, [r11]   ; store character we want to check against tokens
;  cmp  cl, 0
;  jz  .return_fail ; end of target string => return NULL
;  mov esi, edi     ; re-load pre-addr of token-string
;
;  cmp  dl,  cl
;  je  .return_succ
;  cmp  dh,  cl
;  je  .return_succ
;
;.loop_str2:        ; Inner loop: loop over each token and see if it matches curr char 
;  add esi, 1
;  mov  ch, [esi]   ; Load next token into scratch register
;  cmp  ch, 0       ; Check for null terminator;
;  jz  .loop_str1   ;  if so, => end of token str; goto next outer loop
;  cmp  ch,  cl     ; Check if current char matches
;  jne .loop_str2   ;  if !match, goto next inner loop (continue looping tokens)
;                   ;  if match, fall down to .return_succ


.return_succ:      ; If success, return current position of string ptr
  mov          rax, rdi
  add           al,  cl
  ret

.return_fail:      ; If fail, return NULL
  xor rax, rax     ; zero out return register
  ret
