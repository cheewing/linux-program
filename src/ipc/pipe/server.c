#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 1024

void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAXLINE+1];

    // read pathname from IPC channel
    if ((n = read(readfd, buff, MAXLINE)) == 0)
        perror("end-of-file while reading pathname");
    buff[n] = '\0';

    if ((fd = open(buff, O_RDONLY)) < 0)
    {
        snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
        n = strlen(buff);
        write(writefd, buff, n);
    }
    else
    {
        while ((n = read(fd, buff, MAXLINE)) > 0)
            write(writefd, buff, n);
        close(fd);
    }
}
