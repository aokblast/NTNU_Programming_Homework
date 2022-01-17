INCLUDE Irvine32.inc

.data

MAP byte "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC"
MONTHSTR byte 4 dup(?)
.code

main PROC
mov dx, 0F0H
shr dx, 5
and dx, 15
dec dx
xor eax, eax
mov ax, dx
mov bl, 3
mul bl
mov esi, eax
add esi, offset MAP
mov edi, offset MONTHSTR
mov ecx, 3
rep movsb
mov byte ptr [edi], 0

mov edx, offset MONTHSTR
call WriteString

ret
main ENDP
END main


