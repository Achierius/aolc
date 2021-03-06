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
  add edi, -1      ; We subtract  
  add esi, -1
  mov eax, edi
  mov edi, esi
  ; TODO -- for long strings it's probably worth doing some fancy
  ; leetcode work -- tbd if that's actually possible though

  ; Register Contents:
  ;  - EDI: Stored ptr. to (1 before) token string
  ;  - ESI: Ptr. to somewhere inside the token string
  ;  - EAX: Ptr. to somewhere within the target string
  ;  - CL:  Container for target-string byte currently under consideration
  ;  - DL:  Container for current token

.loop_str1:
  add eax, 1       ; advance target string ptr
  mov cl, [eax]    ; store character we want to check against tokens
  cmp cl, 0
  jz  .return_fail ; end of target string => return NULL
  mov esi, edi     ; re-load pre-addr of token-string

.loop_str2:        ; Loop over each token and see if it matches curr char 
  add esi, 1       ; advance token to search for
  mov dl, [esi]    ; load token into register for speed of access
  cmp dl, 0
  jz  .loop_str1   ; end of token str => move on to next outer loop
  cmp dl, cl       ; compare character in string to current token
  jne .loop_str2   ; continue looping tokens

.return_succ:
  ret

.return_fail:      ; Jumping directly to this returns \0
  xor eax, eax     ; zero out return register
  ret
