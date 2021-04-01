in		; a
in		; b
in		; c

jmp :DISCK

GOOD:

push rbx
push -1
mul			; -b

push rdx
sqr			; корень из discr

add			; -b + корень из discr

push 2
push rax
mul			; 2a

div			; x1

push rbx
push -1
mul			; -b

push rdx
sqr 		; корень из discr

sub			; -b - корень из discr

push 2
push rax
mul			; 2a

div			; x2

out
out

EXIT:
hlt

DISCK:
pop rcx
pop rbx
pop rax

push rbx
push 2
pow			; b^2

push rax
push rcx
mul			; ac
push 4
mul

sub			; b^2 - 4ac

pop rdx

push rdx
push 0

jae :GOOD

jmp :EXIT