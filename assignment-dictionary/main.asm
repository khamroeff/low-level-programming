global _start
extern find_word
extern string_length
extern read_word
extern print_string
extern print_newline
extern err
extern exit

section .data
msg_not_found: db "Word not found", 10, 0
msg_too_long: db "The string is too long", 10, 0

%include "colon.inc"

section .text
_start:
  sub rsp, 256             
                           
  mov rsi, 255                                    
  mov rdi, rsp
  
  call read_word           
  test rax, rax            
  je main_input_too_long  
  mov rdi, rsp
  mov rsi, colon_head      
  call find_word
  test rax, rax            
  je main_word_not_found  
  lea rdi, [rax + 8]       
  call string_length
  lea rdi, [rdi + rax + 1] 
  call print_string
  call print_newline
  call exit
main_input_too_long:
  mov rdi, msg_input_too_long
  call err
main_word_not_found:
  mov rdi, msg_word_not_found
  call err