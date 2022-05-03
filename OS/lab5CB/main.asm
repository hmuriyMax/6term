.386
.model flat, c
option casemap: none


.data
    cnt dw 0
    space db ' '

.code

;инициализирующая функция библиотеки
DllMain proc hInstDLL:DWORD, reason:DWORD, unused:DWORD
      mov eax, -1
      ret
DllMain endp

; экспортируемая функция0
cutter proc src:DWORD, dst:DWORD
    BSIZE equ 15
    push ebp
    push esp
    push esi
    push edi

    mov esi, [src]
    mov edi, [dst]

    mov ebx, 0
    mov ecx, 0

    mov ecx, esi
    mov byte ptr [edi], 0

CheckEnd:
    mov cnt, 0
    mov bl,[ecx]
    cmp bl, 0
    je Exit

SkipSpace:
    mov bl, [ecx]
    cmp bl, 20h
    jne _skipSpace
    mov space, bl

    inc ecx
    jmp SkipSpace

_skipSpace:

WordEnd:
    mov bl,[ecx]

    cmp bl, 0
    je CheckWord

    cmp bl, 20h
    je CheckWord

    inc ecx
	inc cnt
    jmp WordEnd

CheckWord:
    mov ax, cnt
    mov dx, 0
    mov bx, 2
	div bx
	cmp dx, 0
	je GoToStart

	inc ax

GoToStart:
    mov cnt, ax
    dec ecx
    mov bl, [ecx]
    cmp bl, 20h
    je _wordEnd_1

    cmp ecx, esi
    je _wordEnd_2

    jmp GoToStart

_wordEnd_1:
    inc ecx
_wordEnd_2:

CopyWord:
    mov bl, [ecx]
    mov byte ptr [edi], bl
    inc edi

    inc ecx
	dec cnt

    cmp cnt, 0
    je PasteSpace

    jmp CopyWord

PasteSpace:
    mov bl, ' '
    mov byte ptr [edi], bl
    inc edi
    dec ecx

SkipEnd:
	mov bl,[ecx]

    cmp bl, 0
    je CheckEnd

    cmp bl, 20h
    je CheckEnd

    inc ecx
	jmp SkipEnd

Exit:
    mov byte ptr [edi], 0
    pop edi
    pop esi
    pop esp
    pop ebp

    ret
cutter endp

End DllMain
