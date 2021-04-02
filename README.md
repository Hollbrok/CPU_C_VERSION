
# **C** version of CPU

Вы также можете прочитать этот README на [английском](https://github.com/Hollbrok/CPU_C_VERSION/blob/main/READMES/README.en.md).
_____________________

## Основная информация об проекте.
В данном проекте были реализованные следующие основные понятие: __ассемблирование команд__ в __машинный код__ , симуляция работы __центрального обрабатывающего устройства__ (__CPU__), а так же __дизассемблирование__ машинного кода обратно в ассемблерные команды.     
Для того, чтобы быть в курсе всего, что происходит настоятельно рекомендуем для начала ознакомится с [__этим проектом__](https://github.com/Hollbrok/STACK). Это необходимо так как процессор, реализованный в этом проекте, __стековый__, а значит __stack__ является его основной частью.
В этом проекте мы реализовали __симуляцию работы процессора__. То есть как __ассемблируются__ команды в  ассемблерный код и как он исполняется __процессором__. Под процессором мы понимаем нашу __симуляцию__ его работы. Для наглядность продемострируем краткий список реализованных вещей в этом проекте, а после него постараемся как можно понятнее объяснить что к чему.
 _________
- [X] [_Assembler_]<details><summary></summary>
    - [X] Обработка первоначального текста
        - [X] Обрабатывать разные случаи текста
            - [X] __Без__ комментариев
            - [X] __С__ комментариями
            - [X] С __пустыми строками__
            - [X] С __лишними__ разделительными знаками
        - [X] Обработка меток с самого начала
            - [X] Через массив из структур меток, то есть двухпроходного считывание
    - [X] Кодировка команд в __ассемблерный код__. 
        - [X] Сделать кодировку ***биективной***
    - [X] Обеспечить работу с __различными__ типами данных
        - [X] Через __классы__, то есть переписать проект на _C++_ 
        - [X] Реализовать работу с типом данных __double__  
    - [X] Обеспечить работу с регистрами
        - [X] ***Биективная*** кодировка
    - [X] Обработка меток
        - [X] ***Биективная*** кодировка меток 
    - [X] Условные переходы
        - [X] ***Биективное*** кодирование условных переходов
    - [X] __Функции__
    - [X] __Рекурсия__
    - [X] __Бинарное кодирование__
    - [X] __RAM__
    - [X] Видеопамять.  
- [X] [_CPU_]<details><summary></summary>
    - [X] Обеспечить считывание __ассемблерного кода__ из файла  
    - [X] Реализация работу _CPU_ через __Stack__ для большей эффективности и мобильности
        - [X] Использоть собственную библиотеку для работы со __Stack__'ом
    - [X] Сделать возможным работу с _основными командами_
        - [X] [`push 66, push rix`](#основы-нашего-ассемблерного-языка) 
        - [X] add, mul, sub, div
        - [X] sin, cos, ln, log2, log10, pow, sqrt
        - [X] in, out
        - [X] jmp, je, jae, jab, jbe, ja, jb
        - [X] hlt
        - [ ] mov, neq, in __rix__
        - [X] call function: 
    - [X] _Однозначность_ результата обработки при исходном коде.
    - [X] Поддержка __Рекурсии__.
    - [X] Распознавание __бинарного кодирования__.
    - [X] __RAM__.
    - [X] Видеопамять.  
- [X] [_Disassembler_]<details><summary></summary>
    - [X] Обработка __закодированных команд__.
    - [X] Дизассемблирование кодов команд в __коды__.
    - [X] __Биективный__ дизассемблинг.
        - [X] Получать из кодов меток названия самих меток (возможно немного преобразованных, но после обратного __ассемблинга__ ассемблерный код остается такой же). 
    - [X] Поддержка __разных__ типов данных.
- [X] [__Make_all__]

## Как использовать?
Если вы хотите написать программу на языке этого ассемблера, потом запусть ассемблинг вашего кода и, наконец, процессор, то для этого вам понадобится сделать следующие шаги:
* 1) Скачайте все содержимое [__папки__](https://github.com/Hollbrok/CPU_C_VERSION/tree/main/exe) с __exe__'шниками.
* 2) Теперь, чтобы написать код, вам необходимо ознакомится с синтаксисом и особенностями ассемблера. Это можно сделать [здесь](https://github.com/Hollbrok/CPU_C_VERSION/tree/main/How%20to%20use%20assembler/Syntax%20and%20features)
* 3) Чтобы сразу запустить и ассемблинг и __CPU__ есть два способа (через __консоль__ и через __make_all.exe__):
    - 3.1) При помощи __консоли__:
        - 3.1.1) Запустите консоль в скаченной папке. Это можно сделать следующим образом:
        ![how_to_open_cmd](https://github.com/Hollbrok/CPU_C_VERSION/blob/main/How%20to%20use%20assembler/pictures/%D0%BA%D0%B0%D0%BA_%D0%BE%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8C_cmd.png)
        - 3.1.2) Теперь в открывшейся консоли необходимо написать ``` make_all.exe имя_файла_с_кодом ```. __Имя_файла_с_кодом__ можно и не указывать, только тогда __assemlber__ будет ассемблировать файл с именем __user_code[for_user].asm__. Если же указать имя файла, то и ассемблировать он будет, соответственно, его.
    - 3.2) При помощи __make_all.exe__. Для этого просто запустите(откройте) его. Минус этого способа заключается в том, что вы не сможете аргументом указать какой файл ассемблировать. Если же такой необходимости нет, то это вполне юзабельный способ.
* 4) Если была получена какая-либо ошибка при ассемблировании или исполнении, то вы получите об этом сообщение, это ошибка будет отображена в DUMP_ASSEM или DUMP_CPU соответственно.

## Примеры моих ассемблерных работ

Прошу заметить, что примеры ассемблерных программ уже есть в этом репозитории, они находятся в [__этой папке__](https://github.com/Hollbrok/CPU_C_VERSION/tree/main/examples%20and%20results). Однако я считаю, что необходимо примести пару примеров особо интересных программ с пояснением как они работают.
- [X] [__Факториал__]<details><summary></summary>
    ```C++
        push 9          ; факториал какого числа будем считать
        pop  rax        ; заносим это число в регистр rax
        push 1          ; начальное значение факториала
        pop  rbx        ; регистр rbx будет хранить результат работа программы

        call :Factorial  ; вызываем функцию факториала, после ее завершения в rbx будет значение
                        ; факториала искомого числа (в данном примере это 9)

        push rbx        ; перед концом программы занесем значение результата в стек, чтобы можно 
                        ; было посмотреть его в CPU_DUMP
        hlt

    Factorial:		
        push 0          ; граничное значние rax (rax в этом случае, это счетчик)
        push rax        ; само значение в rax
        je :END		    ; проверка на граничное условие, чтобы перейти на метку END:
                        ; то есть если изначальное число (9) стало 0, то мы посчитали факториал.        
        
        push rax        ; Если rax != 0, то факториал не посчитать, и нужно считать дальше
        push rbx        ; теперь в стеке лежит rax и rab
        mul             ; умножаем их между собой
        pop  rbx        ; а результат обратно в счетчик факотриала
        
        push rax        ; теперь rax, который отвечает за счетчик факториала кладем в стек
        push 1          ; а именно пушим в стек 1
        sub             ; отнимаем
        pop  rax        ; результат обратно в счетчик (то есть мы уменьшили его на 1)
        
        call :Factorial ; уходим в рекурсию, то есть опять вызываем функцию Factorial
    END:                
        ret             ; Выходим из функции
    ```
- [X] Решение квадратного уравнения 
    

