SECTION .data

SECTION .text
%ifdef OVERRIDE_LIBC_NAMES 
	GLOBAL strrchr
%endif
	GLOBAL _strrchr

extern _strlen

_strrchr:
strrchr:
  
  push rdi
  push rsi

  call _strlen

  mov rdx, rax  ; Length of string in rdx

  pop rcx       ; Search char in rcx
  pop rax       ; Start of string

  mov rsi, rax  ; Store start of string
  add rax, rdx  ; Work from end of string

loop:
 cmp [rax], cl 
 je strrchr_end

 cmp rax, rsi
 je eol

 dec rax

 jmp loop
  
eol:
  xor rax, rax


strrchr_end:
  ret                   ; Return control
