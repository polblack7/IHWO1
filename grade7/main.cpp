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

void writing(char fd[], char *buffer, char *output)
{
    int fdwrite;
    if((fdwrite = open(fd, O_RDONLY)) < 0){
        printf("Can'\t open fifo");
        exit(-1);
    }


    ssize_t bytes = read(fdwrite, buffer, 5000);

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

    if(close(fdwrite) < 0){
        printf("Can\'t close pipe\n");
        exit(-1);
    }


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

void reader( char fd1[], char fd2[],  char *buffer1, char *buffer2,  char *file1, char *file2)
{
    ssize_t bytes1 = readfiles(file1, buffer1, 5000);
    ssize_t bytes2 = readfiles(file2, buffer2, 5000);
    int fdread1;
    if((fdread1 = open(fd1, O_WRONLY)) < 0){
        printf("Can'\t writing fifo");
        exit(-1);
    }
    int fdread2;
    if((fdread2 = open(fd2, O_WRONLY)) < 0){
        printf("Can'\t writing fifo");
        exit(-1);
    }



    if (bytes1 == -1)
    {
        printf("Error reading file\n");
        exit(-1);
    }

    ssize_t writer1 = write(fdread1, buffer1, bytes1);
    ssize_t writer2 = write(fdread2, buffer2, bytes2);

    if (writer1 != bytes1)
    {
        printf("Error writing pipe\n");
        exit(-1);
    }
    if (writer2 != bytes2)
    {
        printf("Error writing pipe\n");
        exit(-1);
    }

    if (close(fdread1) < 0 || close(fdread2) < 0)
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



    char write[] = "first.fifo";
    char read1[] = "second.fifo";
    char read2[] = "third.fifo";

    mknod(write, S_IFIFO | 0666, 0);
    mknod(read1, S_IFIFO | 0666, 0);
    mknod(read2, S_IFIFO | 0666, 0);

    char buffer1[5000];
    char buffer2[5000];


    pid_t pid = fork();

    if (pid == -1) {
        printf("Incorrect fork");
        exit(-1);
    } else if (pid == 0) {
        
        resultGetting(write, read1, read2, buffer1);

    } else {
        
        reader(read1, read2, buffer1, buffer2, argv[1], argv[2]);
        writing(write, buffer1, argv[3]);

    }
    
    return 0;
}
