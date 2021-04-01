		push 25
		push 100
		push 125
		pop rcx
		pop rbx
		pop rax

		push rcx
		pop (2)		; кладем пиксель из стека в оперативку


		push rbx
		pop (1)		; кладем пиксель из стека в оперативку

		push rax
		pop (0)		; кладем пиксель из стека в оперативку

	
		fill
		
		push 255
		push 255
		push 0
		pop rcx
		pop rbx
		pop rax
		
		push rcx
		pop (5)		; кладем пиксель из стека в оперативку


		push rbx
		pop (4)		; кладем пиксель из стека в оперативку

		push rax
		pop (3)		; кладем пиксель из стека в оперативку
		
		push 300
		push 0
		push 100
		circ
		
		hlt