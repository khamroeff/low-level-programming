section .data

align 16
coef_0: dd 0.131, 0.168, 0.189, 0.131

align 16
coef_1: dd 0.543, 0.686, 0.769, 0.543

align 16
coef_2: dd  0.272, 0.349, 0.393, 0.272

align 16
coef_3: dd 255.0, 255.0, 255.0, 255.0

global sepia_asm

section .text

%macro set_coef 1

  shufps xmm3, xmm3, %1
  shufps xmm4, xmm4, %1
  shufps xmm5, xmm5, %1
  
%endmacro

%macro read_xmm 3
  pxor %2, %2 

  pinsrb %2, byte[%1 + 0], 0 
  pinsrb %2, byte[%1 + 0], 4 
  pinsrb %2, byte[%1 + 0], 8 
  pinsrb %2, byte[%1 + 3], 12

  shufps %2, %2, %3
  cvtdq2ps %2, %2
%endmacro

%macro iteration 3

  set_coef %1
  read_xmm rdi, xmm0, %2 
  lea rdi, [rdi + 1]
  read_xmm rdi, xmm1, %2  
  lea rdi, [rdi + 1]
  read_xmm rdi, xmm2, %2
  lea rdi, [rdi - 2]
  mov r8, %3

%endmacro

; rdi - old_pixels
; rsi - new_pixels
; rdx - count of iterations
sepia_asm:

  mov r8, 1
  xor rcx, rcx

.loop:
  ;init coefficients
  movdqa xmm3, [coef_0]
  movdqa xmm4, [coef_1]
  movdqa xmm5, [coef_2]
  movdqa xmm6, [coef_3]

  ;reading data and transformations
  cmp r8, 1
  je .first_iter
  cmp r8, 2
  je .second_iter
  cmp r8, 3
  je .third_iter
.first_iter:
  iteration 0xE7, 0xE8, 2
  jmp .continue
.second_iter:
  iteration 0x79, 0xF8, 3
  jmp .continue
.third_iter:
  iteration 0x9E, 0xFC, 1
  lea rdi, [rdi + 3]

.continue:
  ;calculating new values of pixels components
  mulps xmm3, xmm0
  mulps xmm4, xmm1
  mulps xmm5, xmm2

  addps xmm3, xmm4
  addps xmm5, xmm3

  pminsd xmm5, xmm6
  cvtps2dq xmm5, xmm5
  
  ;store
  pextrb byte[rsi + 0], xmm5, 0 
  pextrb byte[rsi + 1], xmm5, 4
  pextrb byte[rsi + 2], xmm5, 8
  pextrb byte[rsi + 3], xmm5, 12

  lea rsi, [rsi + 4]
  lea rdi, [rdi + 3]
  add rcx, 4

  cmp rdx, rcx 
  ja .loop
  ret
