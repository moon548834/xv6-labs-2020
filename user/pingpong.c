#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int fd[2];
    char buf[64];
    pipe(fd);
    int pid = fork();
    if (pid == 0) {
        read(fd[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        write(fd[1], "h", 1);
        exit(0);
    } else {
        int status;
        write(fd[1], "h", 1);
        wait(&status);
        read(fd[0], buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
    }
    exit(0);
    return 0;
}
