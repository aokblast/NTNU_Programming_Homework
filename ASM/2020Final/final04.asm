INCLUDE Irvine32.inc

.data

STUDENTS byte 100 dup(10 dup (?), 10 dup (?), ?)
STNAME byte 10 dup(?)
STNO byte 10
.code

main PROC
	xor ecx, ecx
	mov cl, STNO
	mov edx, offset STUDENTS
L1:
	mov esi, edx
	mov edi, offset STNAME
	push ecx
	mov ecx, 10
	repe cmpsb
	jne LFAILED
	pop ecx
	mov al, byte ptr [edx + 20]
	ret
LFAILED:
	pop ecx
	add edx, 21
	loop L1
	mov al, 0
	ret
main ENDP
END main


