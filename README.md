# CPU_C_VERSION
C version of CPU with registers, labels, jumps and conditional jumps.
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
 _____________________
 
 ```  
    Вещи, которые будут или уже   
    реализованны в данном проекте.  
``` 
- [ ] _Assembler_
    - [X] Обработка первоначального текста.
        - [ ] Создать 2 режима обработки
            - [ ] С комментариями.
            - [X] Без комментариев.
    - [X] Кодировка команд в __ассемблерный код__.  
        - [X] Сделать кодировку ***биективной***.
    - [X] Обеспечить работу с различными типами данных.
        - [ ] Через __классы__, то есть переписать проект на _C++_. 
        - [X] Через __полиморфизм__ стека.
            - [X] Реализовать работу с типом данных __double__.  
    - [X] Обеспечить работу с регистрами.
        - [X] ***Биективная*** кодировка.
    - [X] Обработка меток.
        - [X] ***Биективная*** кодировка меток.  
    - [X] Условные переходы
        - [X] ***Биективное*** кодирование условных переходов.  
    - [ ] __Рекурсия__.
    - [ ] __Бинарное кодирование__.
    - [ ] Оперативку (__RAM__).
    - [ ] Работу с оперативкой.  
    
        
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
        - [ ] mov, neq, in +__rix__, call.
    - [X] _Однозначность_ результата обработки при исходном коде.
    - [ ] Поддержка __Рекурсии__.
    - [ ] Распознавание __инарного кодирования__.
    - [ ] Поддержка работы с оперативкой (__RAM__).
