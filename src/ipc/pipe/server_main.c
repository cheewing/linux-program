#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void server(int, int);

int main(int argc, char **argv)
{
    int readfd, writefd;

    if (mkfifo(FIFO1, FILE_MODE) < 0 && (errno != EEXIST))
        perror("can't create FIFO1");
    if (mkfifo(FIFO2, FILE_MODE) < 0 && (errno != EEXIST))
    {
        unlink(FIFO1);
        perror("can't create FIFO1");
    }

    readfd = open(FIFO1, O_RDONLY, 0);
    writefd = open(FIFO2, O_WRONLY, 0);

    server(readfd, writefd);
   
    exit(0);
}
