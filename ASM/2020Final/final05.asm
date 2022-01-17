INCLUDE Irvine32.inc

.data

QUOT word 0
REMA word 0

.code

main PROC
	mov eax, 5
	xor ebx, ebx
	mov bx, 2

	shl ebx, 16
	cmp eax, ebx
	ja LOVF
	mov ecx, 16
	shr ebx, 1
L4:
	
	cmp eax, ebx
	jb L6
	sub eax, ebx
	shl QUOT, 1
	or QUOT, 1
	jmp L7
L6:
	shl QUOT, 1
L7:
	shl eax, 1
	loop L4
	shr eax, 16
	mov REMA, ax
	mov ax, QUOT
	call writeInt
	mov ax, REMA
	call writeInt
	ret
LOVF:
	stc
	ret
main ENDP
END main


