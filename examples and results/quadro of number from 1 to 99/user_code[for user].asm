push 1
pop rbx
L2:
push rbx
push 2
pow
out
push rbx
push 1
add
pop rbx
push rbx
push 100
jb :L2
hlt