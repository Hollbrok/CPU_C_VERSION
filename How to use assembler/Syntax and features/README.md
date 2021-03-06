# Гайд по синтаксису и особенностям с примерами
Как и обещалось выше в данном разделе мы __продемострируем__ как что работает и покажем ассемблера.
## Список команд
Начнем с обязательных и самых распространенных команд, а закончим уже более серъезными.
Обязательные команда:
* __hlt__ -- обязательная команда, которая дает процессору сигнал остановить работу, то есть завершить выполнение программы. __ЗАМЕЧАНИЕ!__ Она имеет особое значение только для __CPU__, Ассемблеру же в свою очередь эта команда ничего не говорит. Если сейчас это непонятно, то не переживайте, станет понятно потом.
* ..
Распространенные команды:
* С аргументом (-ами)
    * __push__ _аргумент_ -- Заносит(пушит) в стек значение, соответствующее аргументу. В свою очередь аргументом может быть: __число__, __регистр__, либо значение из ОП. В случае числа все очевидно, берется число и заносится в стек, при использовании регистра берется фактически значение, которое лежит в регистре, и заносится в стек. С оперативной памятью чуть сложнее, потому что т.ж. существуют разные типы данных: byte, word, dword, поэтому к ее рассмотрению вернемся чуть позже
    * __pop__ _аргумент_ -- Команда работает по противоположной аналогии с __push__, она достает из стека. Различие в том, что мы не может написать __pop 5__, то есть достать из стека и положить в число, что логично.
* Без аргументов:
* __add__ -- Достает из верхушки стека два числа, складывает и пушит обратно в стек результат. 
* __sub__ -- Достает из верхушки стека два числа, отнимает от первого второе и пушит результат обратно в стек. Может возникнуть вопрос от какого именно числа отнимается другое, так вот в этом и во всех следующих случаях это работает по следующей логике: если вы хотите от x1 отнять x2, то есть получить результат выражения x1 - x2, то для этого следует написать следующий код:
```C++
; Хотим от 7 отнять 5
    push 7  ; Теперь на верхушке стека лежит 7
    push 5  ; Теперь на верхушке стека лежит 5, а ниже его 7
    sub     ; Достает из верхушки стека (число 5), результат кладет в x2, потом опять дос-                ; тает из верхушке стека (число 7), результат в x1. И кладет обратно в стек x1 - x2.
; результатом в стеке будет лежать число 7 - 5 = 2
```
    
* __mul__ -- Достает из верхушки стека два числа, умножает и пушит обратно в стек результат.
* __div__ -- Достает из верхушки стека два числа, делит и пушит обратно в стек результат. Надеюсь понятно по какой логике происходит деление.
* __sin__ -- Достает из верхушки стека число, берет синус от этого значения(в радианах) пушит обратно в стек результат. 
* __cos__ -- Достает из верхушки стека число, берет косинус от этого значения(в радианах) пушит обратно в стек результат. 
* __pow__ -- Достает из верхушки стека два числа, возводит одно в степень другого и пушит обратно в стек результат. Надеюсь понятно по какой логике происходит деление.
* __sqrt__ -- Достает из верхушки стека число, берет квадратный корень от этого значения и пушит обратно в стек результат. 

Если вы союираетесь писать только простейшие программы, для которых даже не нужны разветвления в зависимости от чего-либо, то на этом можно и остановиться, а для остальных покажем при помощи каких команд можно реализовать ветвление, функции и многое другое!!

Основные команды для ветвлений: 
* Безусловный переход:
    * __jmp__ :_имя_метки_ -- Ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
* Условные переходы:
    * __je__  :_имя_метки_ -- Достает из стека два числа, и , если они __равны__, то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    * __jb__  :_имя_метки_ -- Достает из стека два числа, и , если первое __меньше__ второго (по уже известному нам принципу), то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    * __ja__  :_имя_метки_ -- Достает из стека два числа, и , если первое __больше__ второго (по уже известному нам принципу), то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    * __jae__ :_имя_метки_ -- Достает из стека два числа, и , если первое __больше или равно__ второго (по уже известному нам принципу), то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    * __jbe__ :_имя_метки_  -- Достает из стека два числа, и , если первое __меньше или равно__ второго (по уже известному нам принципу), то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    * __jab__ :_имя_метки_ -- Достает из стека два числа, и , если они __не равны__, то ищет в коде следующий фрагмент _`имя_метки:`_ и при выполнении программы следующий код будет выполнен после как раз таки этого фрагмента.
    
Теперь рассмотрим функции:
* __call__ _имя_функции_: -- Работает аналогично команде __jmp__, различия только в том, что мы переходим не на метку, а на функцию. Сама же функция обязана заканчиваться командой `ret`, то есть по факту `ret` это еще одна команда
* __ret__ -- возвращается в то место, откуда была вызвана функция.

Осталось разобрать инструкции для работы с оперативкой и видеопамятью. А работать мы будем при помози все тех же команд __push__ и __pop__.

Как мы уже знаем у команды __push__ аргументами могут быть число или регистр. Для того, чтобы работать с оперативкой добавляются скобочки: __[]__ и __()__. Первые используются для работы с __dword__, то есть __double__. А вторые скобки для работы с __byte__, то есть __char__. Первые в основном используются для временного хранилища чисел, а второй уже как раз таки для видеопамять. Если вы по какому-либо закону заполните все память, отведенную для видеопамяти, а потом выполните команду `draw`, то в результате получите графическое изображение. Примером такого кода и соответствующей картинки:
- [X] [Код]<details><summary></summary>
    ```C++
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
    ```
- [X] [Результат]<details><summary></summary>
![Круг](https://github.com/Hollbrok/CPU_C_VERSION/blob/main/examples%20and%20results/Draw/CIRC.png)

С большим количеством примеров можете ознакомиться [здесь](https://github.com/Hollbrok/CPU_C_VERSION/blob/main/README.md).