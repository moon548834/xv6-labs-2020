#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[]) {
    int status;
    char buf[64];
    int buf_index = 0;
    int argv_index = 0;
    int i = 0;
    while (read(0, buf + buf_index, 1) != 0) {
        if (buf[buf_index] == '\n') {
            buf[buf_index] = '\0';
            if (argv_index >= 32) {
                printf("too many args");
                exit(-1);
            }
            char *newargv[MAXARG];
            i = 0;
            for (; i + 1 < argc; i++) {
                newargv[i] = argv[i + 1];
            }
            newargv[i] = buf;
            int pid = fork();
            if (pid == 0) {
                exec(argv[1], newargv);
            }
            else {
                wait(&status);
            }
            buf_index = 0;
        } else {
            buf_index++;
        }
    }
    wait(&status);
    exit(0);
    return 0;
}
