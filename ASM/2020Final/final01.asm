INCLUDE Irvine32.inc

.data

TIMESTR byte 20 dup(?)

.code

main PROC
	mov dx, 2AE7H
	mov esi, offset TIMESTR
	xor ebx, ebx
	xor eax, eax
	mov ax, dx
	shr ax, 11
	mov bl, 1
	mul bl
	aam
	or ax, 3030h
	mov [esi], ah
	inc esi
	mov [esi], al
	inc esi
	mov byte ptr [esi], ':'
	inc esi
	mov ax, dx
	shr ax, 5
	and ax, 63
	mov bl, 1
	mul bl
	aam
	or ax, 3030h
	mov [esi], ah
	inc esi
	mov [esi], al
	inc esi
	mov byte ptr [esi], ':'
	inc esi
	mov ax, dx
	and ax, 31
	mov bl, 2
	mul bl
	aam
	or ax, 3030h
	mov [esi], ah
	inc esi
	mov [esi], al
	inc esi
	mov byte ptr [esi], 0
	mov edx, offset TIMESTR
	call WriteString
	ret
main ENDP
END main


