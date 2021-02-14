
# CPU_C_VERSION
__C__ version of __CPU__ with registers, labels, jump ,conditional jumps, functions.
_____________________
## Основная информация об проекте.
В данном проекте были реализованные следующие основные понятие: __ассемблирование команд__ в __машинный код__ [:one::one::one::zero::zero::one::zero::zero:], симуляция работы __центрального обрабатывающего устройства__ (__CPU__), а      __дизассемблирование__ машинного кода обратно в ассемблерные команды.     
Для того, чтобы быть в курсе всего, что происходит настоятельно рекомендуем для начала прочитать [__README__](https://github.com/Hollbrok/STACK). Это необходимо так как процессор, реализованный в этом проекте, __стековый__, а значит __stack__ является его основной частью.
В этом проекте мы реализовали __симуляцию работы процессора__. То есть как __ассемблируются__ команды в  ассемблерный код и как он исполняется __процессором__. Под процессором мы понимаем нашу __симуляцию__ его работы. Для наглядность продемострируем краткий список реализованных вещей в этом проекте, а после него постараемся как можно понятнее объяснить что к чему.
 _________
- [ ] _Assembler_
    - [X] Обработка первоначального текста.
        - [ ] Создать 2 режима обработки
            - [ ] С комментариями.
            - [X] Без комментариев.
            - [X] С пустыми строками
    - [X] Кодировка команд в __ассемблерный код__.  
        - [X] Сделать кодировку ***биективной***.
    - [X] Обеспечить работу с __различными__ типами данных.
        - [ ] Через __классы__, то есть переписать проект на _C++_. 
        - [X] Через __полиморфизм__ стека.
            - [X] Реализовать работу с типом данных __double__.  
    - [X] Обеспечить работу с регистрами.
        - [X] ***Биективная*** кодировка.
    - [X] Обработка меток.
        - [X] ***Биективная*** кодировка меток.  
    - [X] Условные переходы
        - [X] ***Биективное*** кодирование условных переходов.  
    - [X] __Функции__
    - [X] __Рекурсия__.
    - [ ] __Бинарное кодирование__.
    - [ ] Оперативку (__RAM__).
    - [ ] Работу с оперативкой.  
- [X] _Disassembler_
    - [X] Обработка __закодированных команд__.
    - [X] Дизассемблирование кодов команд в __коды__.
    - [X] __Биективный__ дизассемблинг.
        - [X] Получать из кодов меток названия самих меток (возможно немного преобразованных, но после обратного __ассемблинга__ ассемблерный код остается такой же). 
    - [X] Поддержка __разных__ типов данных.
- [ ] _CPU_
    - [X] Обеспечить считывание __ассемблерного кода__ из файла.  
    - [X] Реализация работу _CPU_ через __Stack__ для большей эффективности и мобильности.
        - [X] Использоть собственную библиотеку для работы со __Stack__'ом.
    - [X] Сделать возможным работу с _основными командами_.
        - [X] push __number__, push __rix__.  
        - [X] add, mul, sub, div.
        - [X] sin, cos, ln, log2, log10, pow, sqrt.
        - [X] in, out.
        - [X] jmp, je, jae, jab, jbe, ja, jb.
        - [X] hlt.
        - [ ] mov, neq, in __rix__.
        - [X] call, function: .
    - [X] _Однозначность_ результата обработки при исходном коде.
    - [X] Поддержка __Рекурсии__.
    - [ ] Распознавание __бинарного кодирования__.
    - [ ] Поддержка работы с оперативкой (__RAM__).
## Подробные сведения
Как и обещалось выше в данном разделе мы __продемострируем__ как что работает и покажем особенности этого замечательного проекта 😄.
На мой взгляд стоит начать с __ассемблерных команд__, ведь чтобы посмотреть как все работает необходимо написать какой-нибудь код, а для этого нужно знать __синтаксис__.
### Основы нашего __ассемблерного языка__
Начнем с самых простых команд: __push__ _number_, __pop__, __add__, __mul__, __div__, __sub__. Если вы ознакомились с моим проектом [Stack github](https://github.com/Hollbrok/STACK) или хотя бы прочитали [__README'__](https://github.com/Hollbrok/STACK) шку, то должны как минимум понимать что делают команды __push__ и __pop__, но если вы этого не сделал, то в кратце расскажу принцип их действия. Команда __push__ сама по себе ничего не делает, у нее должен быть __аргумент__, этим аргументом может быть как __число__, так и __регистр__, но об последнем позже. Для того, чтобы внести число в __stack__ для его хранения в нем необходимо написать следующее (символ ";" не является признаком конца __инструкции__, в языке __assembler__ это обозначение начала __строчного комментария__):
```C++
push 2     ; вносит число 2 в stack
push -25,3 ; вносит число -25,3 в stack 
hlt        ; завершение программы.
```
По аналогии чтобы достать число из __stack'__ а нужно применить команду __pop__. В отличии от предыдущей команды у нее как может быть, так и не может быть аргумента. Если команда используется без аргумента, то мы просто достаем и "уничтожаем" число из __stack'__ а, другими словами мы просто его никуда не запоминаем. Единственными аргумента у команды __pop__ могут быть регистры -- это означает что мы достаем число из __stack'__ а и заносим его в __регистр__. Для неопытных читателей будем считать, что __регистр__ это просто переменная (хотя по большей части это так и есть). Приведем пример использования:
```C++
pop     ; просто достает число из stack и никуда не записывает
pop rbx ; достает число и заносит его в регистр rbx stack
hlt     ; завершение программы.
```
Всего у данного процессора 4 регистра: __rax__, __rbx__, __rcx__, __rdx__. Разберем другие команды: __add__ -- достает из __stack'__ а два числа, складывает их, а результат заносит обратно в __stack__ ; __mul__ -- достает из __stack'__ а два числа, умножает их, а результат заносит обратно в __stack__ ; __sub__ -- достает из __stack'__ а два числа, отнимает от _нижнего_ числа _верхнее_, а результат заносит обратно в __stack__ ; __div__ -- достает из __stack'__ а два числа, делит нижнее_ число на _верхнее_, а результат заносит обратно в __stack__. Скорее всего с последними двумя командами не совсем очевидно из какого числа они __отнимают__ другое / __делят__ на другое. Для этого приведем наглядный пример и в добавок я попытаюсь объяснить почему я выбрал имено такой способ,а не другой. Рассмотрим случай, когда пользователь хочут поделить число __25__ на __5__. Для того, чтобы он не запутался реализован такой подход:
```C++
push 25 ; заносим в stack число 25
push 5  ; заносим в stack число 5
div     ; достаем верхнее число из stack (5), затем снова достаем верхнее число из stack (25) и делит нижнее число на верхнее, а так как 25 лежало в stack ниже, чем 5, то соответсвенно получаем 25 / 5.
hlt     ; завершение программы.
```
Аналогичная причина работы и принцип действия у команды __sub__:
```C++
push 10 ; заносим в stack число 10
push 2  ; заносим в stack число 2
sub     ; достаем верхнее число из stack (2), затем снова достаем верхнее число из stack (10) и отнимаем от нижнего числа верхнее, а так как 10 лежало в stack ниже, чем 2, то соответсвенно получаем 10 - 2.
hlt     ; завершение программы.
```
Команды __sin__ , __cos__ достают верхнее число из __stack'__ а, получают соответственное значение тригонометрической функции от этого числа, и заносят результат обратно в __stack__.
Команда __pow__ работает по той же логике, что и команды __sub__ и __div__. То есть по принципу сначала __нижнее__, а потом верхнее.
```C++
push 5 ; заносим в stack число 5
push 2 ; заносим в stack число 2
pow    ; достаем верхнее число из stack (2), затем снова достаем верхнее число из stack (5) и возводим нижнее число в степень верхнего, а так как 5 лежало в stack ниже, чем 2, то соотоетсвенно получаем 5^2.
hlt    ; завершение программы.
```
Команда __sqrt__ достает число из верхушки __stack'__ а и возвращает обратно в __stack__ квадратный корень от этого числа. Очень важная функция __in__ , с ее помощью мы можем считывать число с клавиатуры и заносить его в __stack__. Как всегда приведем пример ее использования.
```C++
in     ; уже при выполнении процессором ассемблерного кода он потребует ввести с клавиатуры число, пусть для конкретики это число (5). Заносим его в stack 
push 2 ; заносим в stack число (2)
add    ; складывает два верхних числа в стеке (принцип работы описан выше). В stack остается только одно число (7).
hlt    ; завершение программы.
```
Мы уже неоднократно использовали команду __hlt__ -- она является признаком окончания программы. Еще одна полезная команда __del__, она просто _выкидывает_ число из верхушки __stack'__ a. Думаю, что принцип ее работы объяснять не надо. Так же существуют такие функции как __ln__, __log10__, __log2__. Их принцип действия достаточно тривиален. 
Переходим к самой прекрасной число -- это __метки__ и __условные переходы__. Так же в папке [examples_results](https://github.com/Hollbrok/CPU_C_VERSION/tree/main/examples%20and%20results) вы можете посмотреть реализацию основных примеров программ: квадраты целых чисел от 1 до какого-то определенного значения через циклы и условные переходы, факториал чисел через рекурсию функций, так же самые тривиальные программы сложения чисел и т.п..Принцип работы остальных команд, примеры их испольхования, работа симулятора __CPU__ и __disassembler__ __coming soon..__
#TODO
* make_all.
* поддержка указания файлов через аргументы командной строки
* бинарное кодирование
* оперативка и видеопамять
______________
Now you can see some ~~ded's weakness~~ cats 😺😻😺. 
______________    
⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄.  
⠄⠄⠄⠄⠄⠄⢰⣷⡄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄  
⠄⠄⠄⠄⣠⣾⣿⣿⣷⣦⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄  
⠄⠄⠄⣠⣿⣿⣿⣿⣿⣿⣇⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄  
⠄⠄⠄⠄⠛⠿⣿⣿⣿⣿⣿⣆⠄⠄⠄⠄⠄⣴⣿⣿⣆⠄⠄⠄  
⠄⠄⠄⠄⠄⣰⣿⣿⣿⣿⣿⣿⣷⣄⠄⠄⠄⣿⣿⠛⠉⠄⠄⠄   
⠄⠄⠄⠄⠄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠄⠘⣿⡆⠄⠄⠄⠄   
⠄⠄⠄⠄⠄⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠸⣿⡀⠄⠄⠄  
⠄⠄⠄⠄⠄⠄⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠄⣿⡇⠄⠄⠄  
⠄⠄⠄⠄⠄⠄⢸⣿⡟⣿⣿⣿⣿⣿⣿⣿⣿⡇⢀⣿⠇⠄⠄⠄  
⠄⠄⠄⠄⠄⢀⣸⡿⢁⣘⣿⣿⣿⣿⣿⣿⣿⣇⣼⠋⠄⠄⠄⠄  
⠄⠄⠄⠄⠄⠻⠿⠓⠿⠿⠿⠿⠿⠿⠿⠿⠿⠛⠁⠄⠄⠄⠄⠄  
⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄  
______________________  
.......................／＞　 フ.....................  
　　　　　| 　_　 _ |  
　 　　　／`ミ _x 彡  
　　 　 /　　　 　 |  
　　　 /　 ヽ　　 ﾉ  
　／￣|　　 |　|　|  
　| (￣ヽ____ヽ___)___)  
　＼二つ  

