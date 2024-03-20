# ИДЗ по ОС №1

### Исполнитель: Анненков Никита Игоревич, БПИ226.

### Вариант 35.
Задание: Разработать программу, которая на основе анализа двух ASCII–
строк формирует на выходе строку, содержащую символы, присутствующие в обеих строках (пересечение символов). Каждый
символ в соответствующей выходной строке должен встречаться
только один раз. Входными и выходными параметрами являются
имена трех файлов, задающих входные и выходную строки.

### Работа по требованиям выполнена на оценку 8.

### Небольшая инструкция по запуску:
Для запуска grade4 - grade7: ./a.out filename1.txt filename2.txt resultfile.txt

Для запуска grade8: ./solver & ./readNwrite filename1.txt filename2.txt resultfile.txt

(Все исполняемые файлы уже собраны, но если есть желание можно их собрать заново, я использовал gcc)

#
### Теперь к отчету по каждой разбалловке:

## 4 балла:
Реализовано 3 процесса: чтения, обработки и записи. Чтение и обработка связаны с помощью неименованных каналов fdread1 и fdread2(по одному на файл), процесс обработки и записи связаны с помощью неименнованного канала fdwrite.

Разработано консольное приложение, использующее неименованные каналы, обеспечивающее взаимодействие по следующей схеме:

• первый процесс читает текстовые данные из заданного файла и
через неименованный канал передает их второму процессу;

• второй процесс осуществляет обработку данных в соответствии
с вариантом задания и передает результат обработки через
неименованный канал третьему процессу;

• третий процесс осуществляет вывод данных в заданный файл.

Для задания имен входного и выходного файлов используются аргументы командной строки.

Ввод и вывод данных при работе с файлами осуществляется через системные вызовы операционной системы read и write.

Размеры буферов для хранения вводимых данных и результатов обработки равны 5000 байт.

Так же предоставлен набор из 5 тестовых данных((2 файла для чтения и один файл результата) х 5).

Файлы открываются для чтения и записи в readfiles и writefiles соответсвенно.

## 5 баллов:

Реализовано 3 процесса: чтения, обработки и записи. Чтение и обработка связаны с помощью именованных каналов first.fifo и second.fifo(по одному на файл), процесс обработки и записи связаны с помощью именованного канала third.fifo. Они создаются с помощью mknod.

Разработано консольное приложение, использующее именованные каналы, обеспечивающее взаимодействие по следующей схеме:

• первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;

• второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал третьему процессу;

• третий процесс осуществляет вывод данных в заданный файл.

Для задания имен входного и выходного файлов используются аргументы командной строки.

Ввод и вывод данных при работе с файлами осуществляется через системные вызовы операционной системы read и write.

Размеры буферов для хранения вводимых данных и результатов обработки равны 5000 байт.

Так же предоставлен набор из 5 тестовых данных((2 файла для чтения и один файл результата) х 5).

Файлы открываются для чтения и записи в readfiles и writefiles соответсвенно.

## 6 баллов:

Реализовано 2 процесса: чтение с записью и обработка строк. Чтение и обработка строки связаны с помощью неименованных каналов fdread1 и fdread2(по одному на файл) далее обработанная строка передается с помощью неименованного канала fdwrite на запись.

Разработано консольное приложение, использующее неименованные каналы, обеспечивающее взаимодействие по следующей схеме:
• первый процесс читает текстовые данные из заданного файла и через неименованный канал передает их второму процессу;

• второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через неименованный канал обратно первому процессу;

• первый процесс осуществляет вывод данных в заданный файл.

Для задания имен входного и выходного файлов используются аргументы командной строки.

Ввод и вывод данных при работе с файлами осуществляется через системные вызовы операционной системы read и write.

Размеры буферов для хранения вводимых данных и результатов обработки равны 5000 байт.

Так же предоставлен набор из 5 тестовых данных((2 файла для чтения и один файл результата) х 5).

Файлы открываются для чтения и записи в readfiles и writefiles соответсвенно.

## 7 баллов

Реализовано 2 процесса: чтение с записью и обработка строк. Чтение и обработка строки связаны с помощью именнованных каналов first.fifo и second.fifo(по одному на файл) далее обработанная строка передается с помощью именованного канала third.fifo на запись. Они создаются с помощью mknod.

Разработано консольное приложение, использующее именованныеканалы, обеспечивающее взаимодействие по следующей схеме:

• первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;

• второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал обратно первому процессу;

• первый процесс осуществляет вывод данных в заданный файл.

Для задания имен входного и выходного файлов используются аргументы командной строки.

Ввод и вывод данных при работе с файлами осуществляется через системные вызовы операционной системы read и write.

Размеры буферов для хранения вводимых данных и результатов обработки равны 5000 байт.

Так же предоставлен набор из 5 тестовых данных((2 файла для чтения и один файл результата) х 5).

Файлы открываются для чтения и записи в readfiles и writefiles соответсвенно.

## 8 баллов

Реализовано независимых 2 процесса: чтение с записью и обработка строк. Чтение и обработка строки связаны с помощью именнованных каналов read1.fifo и read2.fifo(по одному на файл) далее обработанная строка передается с помощью именованного канала write.fifo на запись. Они создаются с помощью mknod.

Файл readNwrite - чтение и запись.

Файл solver - обработка строки.

Разработано консольное приложение, использующее именованные
каналы, обеспечивающее взаимодействие по следующей схеме:

• первый процесс читает текстовые данные из заданного файла и через именованный канал передает их второму процессу;

• второй процесс осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал обратно первому процессу;

• первый процесс осуществляет вывод данных в заданный файл.

Для задания имен входного и выходного файлов используются аргументы командной строки.

Ввод и вывод данных при работе с файлами осуществляется через системные вызовы операционной системы read и write.

Размеры буферов для хранения вводимых данных и результатов обработки равны 5000 байт.

Так же предоставлен набор из 5 тестовых данных((2 файла для чтения и один файл результата) х 5).

Файлы открываются для чтения и записи в readfiles и writefiles соответсвенно.






