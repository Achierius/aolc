SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strcat
%endif
	GLOBAL _strcat

extern _strlen

_strcat:
strcat:
  push rdi  ; Save arguments before calling _strlen
  push rsi

  call _strlen
  mov rdx, rax  ;  Length of destination

  pop rcx  ; Source string
  pop rax  ; Destination string
  add rdx, rax  ; null terminator of destination

  xor rdi, rdi

.loop:
  mov r8, [rcx]
  mov [rdx], r8

  cmp [rcx], dil
  je .eol

  jmp .loop

.eol:


  ret                   ; Return control
