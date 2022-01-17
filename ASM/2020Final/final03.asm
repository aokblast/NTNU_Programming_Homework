INCLUDE Irvine32.inc

.data

X dword 123456h
STR1 byte 11 dup(?)
MA byte "0123456789ABCDEF"
.code

main PROC
	xor ebx, ebx
	xor eax ,eax
	xor edx, edx
	mov ecx, 4
	mov edi, offset STR1
	mov esi, offset X + 3
L1:
	mov al, [esi]
	mov dl, al
	shr dl, 4
	
	mov bl, [MA + edx]
	mov [edi], bl
	inc edi
	mov dl, al
	and dl, 15
	mov bl, [MA + edx]
	mov [edi], bl
	mov [edi], bl
	inc edi
	dec esi
	loop L1

	mov byte ptr [edi], 'H'
	mov byte ptr [edi + 1], 0
	mov edx, offset STR1
	call WriteString
	ret
main ENDP
END main


