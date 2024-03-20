#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <ctype.h>
#include <unordered_set>
#include <string>

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
    char buffer1[5000];
    char buffer2[5000];
    char write[] = "write.fifo";
    char read1[] = "read1.fifo";
    char read2[] = "read2.fifo";

    mknod(write, S_IFIFO | 0666, 0);
    mknod(read1, S_IFIFO | 0666, 0);
    mknod(read2, S_IFIFO | 0666, 0);

    reader(read1, read2, buffer1, buffer2, argv[1], argv[2]);
    writing(write, buffer1, argv[3]);

}