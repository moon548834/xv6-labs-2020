#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int i = 2;
    int fd[2];
    int buffer[36];
    int buffer_index = 0;
    int status;
    pipe(fd);
    int pid = fork();
    if (pid == 0) {
start:
        int val;
        int first = 0;
        buffer_index = 0;
        close(fd[1]);
        while(read(fd[0], &val, sizeof(val)) != 0) {
           if ((val % i) != 0 || val == i) {
               if (first == 0) {
                   printf("prime %d\n", val);
                   first = 1;
               } else {
                   buffer[buffer_index++] = val;
               }
           }
        }
        i++;
        if (buffer_index == 0) exit(0);
        close(fd[0]);
        pipe(fd);
        int newpid = fork();
        if (newpid == 0) {
            goto start;
        } else {
            close(fd[0]);
            write(fd[1], buffer, buffer_index * sizeof(int));
            write(fd[1], 0, 0);
        }
        close(fd[1]);
        wait(&status);
        exit(0);
    } else {
        close(fd[0]);
        i = 2;
        while (i <= 35) {
            write(fd[1], &i, sizeof(i));
            i++;
        }
        write(fd[1], 0, 0);
        close(fd[1]);
        wait(&status);
    }
    exit(0);
    return 0;
}
