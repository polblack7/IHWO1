#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <ctype.h>
#include <unordered_set>
#include <string>

#define BUFFER_SIZE 5000

ssize_t twoInRow(char *str1, char *str2, char *buffer, size_t size)
{

    char result[BUFFER_SIZE];


    int char_set1[256] = {0};

    // Заполнение набора символами из первой строки
    for (int i = 0; str1[i] != '\0'; i++) {
        char_set1[(int)str1[i]] = 1;
    }

    // Создание строки для хранения пересечения символов
    char intersection[5000];
    int intersection_index = 0;

    // Проверка каждого символа второй строки
    // Если символ уже присутствует в наборе символов первой строки и отсутствует в строке пересечения,
    // то добавляем его в строку пересечения
    for (int i = 0; str2[i] != '\0'; i++) {
        if (char_set1[(int)str2[i]] && strchr(intersection, str2[i]) == NULL) {
            intersection[intersection_index++] = str2[i];
        }
    }
    intersection[intersection_index] = '\0';





    int id = 0;
    while (intersection[id] != '\0')
    {
        buffer[id] = intersection[id];
        id++;
    }

    buffer[id] = '\0';
    return id;

}

void resultGetting(char fdwrite[], char fdread1[], char fdread2[], char *buffer1)
{
    int fdreader1;
    if((fdreader1 = open(fdread1, O_RDONLY)) < 0){
        printf("Can'\t open fifo");
        exit(-1);
    }
    int fdreader2;
    if((fdreader2 = open(fdread2, O_RDONLY)) < 0){
        printf("Can'\t open fifo");
        exit(-1);
    }

    char buffer2[5000];
    ssize_t bytes1 = read(fdreader1, buffer1, 5000);
    ssize_t bytes2 = read(fdreader2, buffer2, 5000);



    if (bytes1 == -1 || bytes2 == -1)
    {
        printf("Error reading pipe\n");
        exit(-1);
    }

    if (close(fdreader1) < 0 || close(fdreader2) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }


    ssize_t result = twoInRow(buffer1, buffer2, buffer1, bytes1);
    int fdwriter;
    if((fdwriter = open(fdwrite, O_WRONLY)) < 0){
        printf("Can'\t open fifo");
        exit(-1);
    }

    ssize_t writer = write(fdwriter, buffer1, result);

    if (writer != result)
    {
        printf("Error writing pipe\n");
        exit(-1);
    }

    if (close(fdwriter) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    char buffer[BUFSIZ];
    char write[] = "write.fifo";
    char read1[] = "read1.fifo";
    char read2[] = "read2.fifo";

    resultGetting(write, read1, read2, buffer);
}