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
		hlt