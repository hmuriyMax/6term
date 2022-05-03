.386 ; Директива .686 указывает компилятору ассемблера, что необходимо использовать набор операций процессора определённого поколения.
.model flat, c ; model - используемая модель памяти и соглашение о вызовах. Передача параметров и возврат из процедуры, будет задействован способ передачи параметров, принятый в языке C
.stack 4096 ; размер стека
option casemap: none ; option casemap: none заставляет компилятор языка ассемблера различать большие и маленькие буквы в метках и именах процедур.
ExitProcess PROTO, dwExitCode: DWORD; ExitProcess -  завершение программы


.data
    cnt dw 0
    space db ' '

.code ; сегмент кода

;<имя процедуры> PROC
; <тело процедуры>
;<имя процедуры> ENDP

DllMain proc hInstDLL:DWORD, reason:DWORD, unused:DWORD ;инициализирующая функция библиотеки
      mov eax, -1
      ret
DllMain endp

; экспортируемая функция0
cutter proc src:DWORD, dst:DWORD
    BSIZE equ 15   ; так задается константа BSIZE

    push ebp ; сохраняем используемые регистры
    push esp
    push esi
    push edi
; для обращения к памяти по адресу, хранящемуся в регистре, в команде указывается имя регистра в квадратных скобках
    mov esi, [src] ;переносим в регистры указатели на входные строки
    mov edi, [dst]

;Операция XOR имеет интересную особенность – если значения операндов совпадают, то результатом будет значение 0.
;Поэтому операцию XOR используют для обнуления регистров – она выполняется быстрее, чем запись нуля с помощью команды MOV.
    xor ebx, ebx ;
    xor ecx, ecx

    mov ecx, esi
    mov byte ptr [edi], 0 ; ptr - указывает размер пересылаемых данных, чтобы не было ошибки

CheckEnd:
    mov cnt, 0
    mov bl,[ecx]
    cmp bl, 0
    je Exit ; флаг если равно ZF = 1

SkipSpace:
    mov bl, [ecx]
    cmp bl, 20h ; h - шестнадцатеричная с.с. (в ASCII пробел в 16сс = 20)
    jne _skipSpace; не равно, флаг ZF = 0
    mov space, bl

    inc ecx
    jmp SkipSpace

_skipSpace:

WordEnd:
    mov bl,[ecx]

    cmp bl, 0 ; конец строки
    je CheckWord

    cmp bl, 20h ; пробел
    je CheckWord

    inc ecx ; дальше
	inc cnt ; увеличиваем счетчик букв в слове
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

    cmp bl, 0 ; конец строки
    je CheckEnd

    cmp bl, 20h ; пробел
    je CheckEnd

    inc ecx ; дальше
	jmp SkipEnd

Exit:
    mov byte ptr [edi], 0
    pop edi ;возврат значений
    pop esi
    pop esp
    pop ebp

    ret ; выход из процедуры
cutter endp

End DllMain
