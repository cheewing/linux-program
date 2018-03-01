#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 1024

int main()
{
    int fd[2];
    pid_t childpid;
    const char *msg = "hello\n";
    char buf[256];

    pipe(fd);

    if ((childpid = fork()) < 0) 
    {
        perror("fork");
    }
    else if (childpid == 0)
    {
        // child
        // close read pipe
        close(fd[0]);

        // write hello into write pipe
        write(fd[1], msg, strlen(msg));
        close(fd[1]);
        exit(0);
    }

    // parent
    close(fd[1]);
    int len = read(fd[0], buf, sizeof(buf));
    write(STDOUT_FILENO, buf, len);

    waitpid(childpid, NULL, 0);  // wait for child to terminate
    close(fd[0]);
    return 0;
}
