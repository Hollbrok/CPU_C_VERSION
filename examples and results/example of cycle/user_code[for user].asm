push 0
pop rbx
L3:
push 1
push rbx
add
pop rbx
push rbx
push 12
jb :L3
push rbx
out
hlt