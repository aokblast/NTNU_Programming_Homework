INCLUDE Irvine32.inc

.data

AUGEND byte 255, 255, 0, 0
ADDEND byte 5, 5, 0, 0
SUM byte 5 dup (?)
.code

main PROC
xor eax, eax
clc
pushfd
mov ecx, 4
mov esi, offset AUGEND
mov edi, offset SUM
mov edx, offset ADDEND
L1:
	mov al, [esi]
	popfd
	adc al, [edx]
	pushfd
	mov [edi], al
	inc esi
	inc edi
	inc edx
	loop L1
	mov al, 0
	popfd
	adc al, 0
	mov [edi], al
	mov al, [SUM + 2]
	call WriteInt

ret
main ENDP
END main


