%define SYS_EXIT 60
%define SYS_READ 0
%define SYS_WRITE 1

%define STDIN 0
%define STDOUT 1

%define SYM_NULL 0
 
; Принимает код возврата и завершает текущий процесс
exit: 
    mov rax, SYS_EXIT ; записываем SYS_EXIT в rax
    syscall           ; 'exit' syscall kill process

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax                         ; rax - результат, в начале нужно обнулить 
    .loop:                               ; проверка каждого символа на SYM_NULL (строка оканчивается на 0x00)
        cmp byte[rdi+rax], SYM_NULL     
        je .end                          ; выход из цикла, если нашлось
        inc rax                          ; если нет, инкрементируем rax
        jmp .loop                        ; в начало цикла
    .end:
        ret
    

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    call string_length
    mov rdx, rax 
    mov rax, SYS_WRITE
    mov rsi, rdi         ; передаем в строку то что хотим вывести
    mov rdi, STDOUT      ; (дескриптор)
    syscall
    ret

; Принимает код символа и выводит его в stdout
print_char:
    push rdi
    mov rdx, 1           ; нужно кол-во байтово
    mov rax, SYS_WRITE 
    mov rsi, rsp         ; записываем вершину стека в rsi
    mov rdi, STDOUT
    syscall
    pop rdi              ; достаем из стэка адрес символа
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, 0xA
    jmp print_char ; вызываем предыдущую функцию с нашим аргументом

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    mov 	rax, rdi
	mov 	r8, 10		    ; делитель (10)
	push	0				; null-terminate

	.loop:
		xor	rdx, rdx		
		div	r8			    ; делим rax на r8 (10)
		add	rdx, '0'		; получаем ASCII символ	добавив (num + 0x30)
		push	rdx			; символ -> вершину стэка
		cmp	rax, 0			; rax ? 0 (сравнение)
		jne	.loop

	.print_uint64:
		pop	rdi			    
		cmp	rdi, 0			; -> stdout, пока не 0
		je	.return
		call	print_char  ; отправляем символ в print_char
		jmp	.print_uint64

	.return:
		ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    test rdi, rdi
    jns print_uint
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    xor rcx, rcx
    
    .loop:
        
        mov r8b, [rdi + rcx]
	mov r9b, [rsi + rcx]
        cmp r8b, r9b
        jne .notequal
	cmp r8b, SYM_NULL ; проерка на ASCII null, если да - то останавливаемся.
        je .equal

        inc rcx
        jmp .loop

    .notequal:
    mov rax, 0
    ret

    .equal:
    mov rax, 1
    ret

; Иное короткое решение)

;string_equals2:
;    std
;    repnz cmpsb
;    mov al, byte [rdi]
;    test al, al
;    setz al
;    ret



; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    mov rax, SYS_READ
    mov rdi, STDIN
    push rax
    mov rdx, 1        ; 1 byte
    mov  rsi, rsp
    syscall
    test rax, rax
    jz .eof
    mov rax, [rsp]             ; по сути pop * работает как:
    .eof:                      ; mov *, [rsp]
    add rsp, 8                 ; add rsp, 8
    ret

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    .loop1:
        push rdi
        push rsi
        call read_char
        pop rsi
        pop rdi

        cmp al, 0
        je .fail
        call check_whitespace
        jz .loop1

        xor rcx, rcx
        test rsi, rsi
        jz .fail
    .loop2:
        cmp al, 0
        je .done
        call check_whitespace
        jz .done

        mov byte [rdi+rcx], al
        inc rcx
        cmp rcx, rsi
        jge .fail

        push rdi
        push rsi
        push rcx
        call read_char
        pop rcx
        pop rsi
        pop rdi
	jmp .loop2

    .fail:
        xor rax, rax
        xor rdx, rdx
        ret

    .done:
        mov byte [rdi+rcx], 0
        mov rax, rdi
        mov rdx, rcx
        ret


check_whitespace: ; sets ZF if char in AL is whitespace; affects no registers except FLAGS
    cmp al, 0x20
    je .whitespace
    cmp al, 0x9
    je .whitespace
    cmp al, 0xA
    je .whitespace
    .whitespace:
    ret

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    xor rcx, rcx
    xor rdx, rdx
.loop:
        mov cl, byte [rdi+rdx]       ; проверка 1го символа на число
        test cl, cl
        jz .end
        cmp cl, 0x30
	jl .end
        cmp cl, 0x39
	jg .end
        sub cl, 0x30

        imul rax, rax, 10
        add rax, rcx
        inc rdx
        jmp .loop
.fail:
    xor rdx, rdx
.end:
    ret






; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    push rbx
    xor rax, rax
    xor bl, bl
    xor rdx, rdx
.loop:
        xor rcx, rcx
        mov cl, byte [rdi+rdx]       ; проверка 1го символа на число
        cmp cl, '-'
        jne .normal
        mov bl, 1
        jmp .next
        .normal:
        test cl, cl
        jz .end
        cmp cl, 0x30
	jl .end
        cmp cl, 0x39
        jg .end
        sub cl, 0x30
        test bl, bl
        jz .positive
        neg rcx
.positive:
        imul rax, rax, 10
        add rax, rcx
.next:
        inc rdx
        jmp .loop
.fail:
    xor rdx, rdx
.end:
    pop rbx
    ret

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
xor rcx, rcx
.loop:
    cmp rcx, rdx
    jge .fail
    mov al, byte [rdi+rcx]
    mov byte [rsi+rcx], al
    inc rcx
    test al, al
    jnz .loop
    dec rcx
    jmp .end
.fail:
    mov byte [rsi], 0
    xor rcx, rcx
.end:
    mov rax, rcx
    ret


