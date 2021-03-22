		push 269999		; кол-во пикселей	
		pop rdx			; счетчик цикла

		push 0			; нач. координата x
		pop rax			; x
		push 0			; нач. координата y
		pop rbx			; y

FOR:
		push rax
		push 150
		sub
		push 2
		pow

		push rbx
		push 150
		sub
		push 2
		pow

		add


		push 50
		push 2
		pow

		jb :DRAW2
		jmp :DRAW1

DRAW1: 
		call :DRAW_1			; пиксель фона
		jmp :ENDDR
DRAW2:	
		call :DRAW_2			; пиксель круга
		jmp :ENDDR

ENDDR:
		push rdx
		push 1
		jbe :END		; условие входа из цикла
		jmp :FOR

END:
		draw
		hlt
		
rdxM:
		push rdx		; декремент
		push 1			; счетчика
		sub				; на 1
		pop rdx			; каждый раз
		ret

SET_PIX:
		push 25			; rax
		push 100		; rbx
		push 125		; rcx

		ret

XY:
		push rax
		push 299
		jb :DEF1
		jmp :DEF2
ENDXY:	
		ret

DEF1:	
		push rax
		push 1
		add
		pop rax
		jmp :ENDXY

DEF2:
		push 0
		pop rax

		push rbx
		push 1
		add
		pop rbx
		jmp :ENDXY

DRAW_1:
		call :XY
		push 125
		pop (rdx)		; кладем пиксель из стека в оперативку 2
		call :rdxM		; 2 - 1 = 1

		push 100
		pop (rdx)		; кладем пиксель из стека в оперативку 1
		call :rdxM		; 1 - 1 = 0

		push 25
		pop (rdx)		; кладем пиксель из стека в оперативку 0
		call :rdxM		; 0 - 1 = -1

		ret

DRAW_2: 
		call :XY
		push 155
		pop (rdx)		; кладем пиксель из стека в оперативку 2
		call :rdxM		; 2 - 1 = 1

		push 155
		pop (rdx)		; кладем пиксель из стека в оперативку 1
		call :rdxM		; 1 - 1 = 0

		push 155
		pop (rdx)		; кладем пиксель из стека в оперативку 0
		call :rdxM		; 0 - 1 = -1

		ret