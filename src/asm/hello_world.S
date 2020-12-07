SECTION .DATA
	hello_world:     db 'Hello world!',10
	hello_len:       equ $-hello_world

SECTION .TEXT
	GLOBAL asm_hello_world

asm_hello_world:
	mov eax, 4            ; syscall # for write()
	mov ebx, 1            ; fd for STDOUT
	mov edx, hello_len
	mov ecx, hello_world
	int 80h               ; Interrupt
  ret                   ; Return control
