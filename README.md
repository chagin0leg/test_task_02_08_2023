# starline_test_task

Test task for the Starline job vacancy "Embedded developer"

## Nt[ybxtcrjt задание:

Реализовать программный модуль управления кольцевым буфером на 100 байт для STM32F412:
- извлечение байта
- добавление байта (при переполнении новый байт перетирает старый по кругу)
- если буфер непустой, его байты отправляются по UART

Модуль должен содержать:
- функцию настройки периферии UART (в параметрах: скорость, разрядность, четность, кол-во стоп битов)
- обработчик прерывания по передаче байта

К модулю нужно составить достаточную документацию, по которой другой программист сможет встроить его в свой код.

Результат прислать в виде:
- исходные коды модуля + документация
- проекта Keil, демонстрирующий работу модуля на микроконтроллере STM32, который компилируется и может быть прошит в микроконтроллер для проверки. Демо-проект должен генерировать случайные массивы байт через случайные промежутки времени и добавлять их в очередь на отправку в UART.