INCLUDE Irvine32.inc

.data

NROW byte 3
NCOL byte 3
ARDAT dword 1, 2, 3, 4, 5, 6, 7, 8, 9
ROWNO byte 1
COLNO byte 1

.code

main PROC
	xor eax, eax
	mov al, NCOL
	mov bl, ROWNO
	mul bl
	add al, COLNO
	mov bl, 4
	mul bl
	xor ecx, ecx
	mov cx, ax
	add ecx, offset ARDAT
	mov eax, [ecx]
	call WriteInt

ret
main ENDP
END main


