push 9
pop  rax
push 1
pop  rbx

call :Function

push rbx
out
hlt

Function:		; factorial 
push 0
push rax
je :FUNC1		; проверка на граничное условие выхода из рекурсии
push rax
push rbx
mul
pop  rbx
push rax
push 1
sub
pop  rax
call :Function 	; уходим в рекурсию
FUNC1:
ret