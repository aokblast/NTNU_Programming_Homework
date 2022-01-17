INCLUDE Irvine32.inc

.data

ARY2D sdword 10, 2, -50, -100, 45, 100, 60, -15, 70
NOROW byte 3
NOCOL byte 3
MAXVAL sdword ?
ROWNO byte ?
COLNO byte ?

.code

main PROC
	xor ecx, ecx
	xor eax, eax
	mov ebx, -2147483647
	mov edx, offset ARY2D
	mov cl, NOROW
	L1:
		push ecx
		mov cl, NOCOL
	L2:
		mov eax, [edx]
		cmp eax, ebx
		jle L3
		mov ebx, eax
		mov al, NOCOL
		sub eax, ecx
		mov COLNO, al
		mov al, NOROW
		pop esi
		sub eax, esi
		push esi
		mov ROWNO, al
	L3:
		add edx, 4
		loop L2
		pop ecx
		loop L1
	mov MAXVAL, ebx
	mov eax, MAXVAL
	call WriteInt
	xor eax, eax
	mov al, ROWNO
	call WriteInt
	mov al, COLNO
	call WriteInt
		
	ret
main ENDP
END main


