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

ssize_t readfiles(char *file1, char *buffer, size_t size)
{
    int fd = open(file1, O_RDONLY);

    if (fd < 0)
    {
        printf("Error opening file\n");
        return -1;
    }
    ssize_t bytes = read(fd, buffer, size);


    if (bytes == -1)
    {
        printf("Error reading file\n");
        return -1;
    }

    buffer[bytes] = '\0';

    if (close(fd) < 0)
    {
        printf("Error closing file\n");
        return -1;
    }
    return bytes;
}

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

ssize_t writefiles(char *file, char *buffer, size_t size)
{
    int fd = open(file, O_WRONLY | O_CREAT, 0666);
    if (fd < 0)
    {
        printf("Error opening file\n");
        return -1;
    }
    ssize_t bytes = write(fd, buffer, size);
    if (bytes == -1)
    {
        printf("Error writing file\n");
        return -1;
    }
    if (close(fd) < 0)
    {
        printf("Error closing file\n");
        return -1;
    }
    return bytes;
}

void writing(int *fd, char *buffer, char *output)
{
    if (close(fd[1]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }


    ssize_t bytes = read(fd[0], buffer, 5000);

    if (bytes == -1)
    {
        printf("Error reading pipe\n");
        exit(-1);
    }


    ssize_t write = writefiles(output, buffer, bytes);

    if (write != bytes)
    {
        printf("Error writing file\n");
        exit(-1);
    }

    if (close(fd[0]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }
}

void resultGetting(int *fdwrite, int *fdread1, int *fdread2, char *buffer1)
{
    if (close(fdread1[1]) < 0 || close(fdread2[1]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }
    char buffer2[5000];
    ssize_t bytes1 = read(fdread1[0], buffer1, 5000);
    ssize_t bytes2 = read(fdread2[0], buffer2, 5000);


    if (bytes1 == -1 || bytes2 == -1)
    {
        printf("Error reading pipe\n");
        exit(-1);
    }

    if (close(fdread1[0]) < 0 || close(fdread2[0]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }


    ssize_t result = twoInRow(buffer1, buffer2, buffer1, bytes1);

    if (close(fdwrite[0]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }

    ssize_t writer = write(fdwrite[1], buffer1, result);

    if (writer != result)
    {
        printf("Error writing pipe\n");
        exit(-1);
    }

    if (close(fdwrite[1]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }
}

void reader( int *fd, char *buffer, char *file)
{
    ssize_t bytes = readfiles(file, buffer, 5000);
    if (close(fd[0]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }



    if (bytes == -1)
    {
        printf("Error reading file\n");
        exit(-1);
    }

    ssize_t writer = write(fd[1], buffer, bytes);

    if (writer != bytes)
    {
        printf("Error writing pipe\n");
        exit(-1);
    }

    if (close(fd[1]) < 0)
    {
        printf("Error closing pipe\n");
        exit(-1);
    }

}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s file1 file2 output\n", argv[0]);
        return -1;
    }

    int fdwrite[2];
    int fdread1[2];
    int fdread2[2];

    if (pipe(fdwrite) < 0 || pipe(fdread1) < 0 || pipe(fdread2) < 0)
    {
        printf("Error creating pipe\n");
        return -1;
    }




    char buffer1[5000];
    char buffer2[5000];


    int pid = fork();

    if (pid == -1) {
        printf("Incorrect fork");
        exit(-1);
    } else if (pid == 0) {
        
        resultGetting(fdwrite, fdread1, fdread2, buffer1);

    } else {
        
        reader(fdread1, buffer1, argv[1]);
        reader(fdread2, buffer2, argv[2]);
        writing(fdwrite, buffer1, argv[3]);

    }
    return 0;
}
