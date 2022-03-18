global find_word
extern string_equals

section .text

find_word:
    .loop:
        push rsi
        add rsi, 16
        call string_equals 
        pop rsi

        test rax, 1
        jne .success

        mov rsi, [rsi]

        test rsi, rsi
        je .not_found

        jmp .loop
    .success:
        add rsi, 16
        mov rax, [rsi - 8]
        ret
    .not_found:
        xor rax, rax
        ret