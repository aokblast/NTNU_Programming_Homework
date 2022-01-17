INCLUDE Irvine32.inc

.data
TDATA byte "This is a book.", 0
SCHAR byte 's'

.code

main PROC

mov edx, offset TDATA
mov bl, SCHAR
or bl, 32

L1:
	mov al, [edx]
	cmp al, 0
	je LRET
	or al, 32
	cmp al, bl
	jne LLOOP
	xor al, 32
	mov [edx], al

LLOOP:
	inc edx
	jmp L1
LRET:
	mov edx, offset TDATA
	call WriteString
	ret


ret
main ENDP
END main


