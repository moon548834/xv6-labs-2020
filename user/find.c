#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}
void find(char *path, char *target) {
    char buf[128], *p;
    //char buf2[512];
    int fd;
    struct stat st;
    struct dirent de;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "can't go to %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "cant' stat %s\n", path);
        close(fd);
        return;
    }
    char *name = fmtname(path);
    switch(st.type) {
    case T_FILE:
        if (strcmp(name, target) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
          if(de.inum == 0)
            continue;
          memmove(p, de.name, DIRSIZ);
          p[DIRSIZ] = 0;
          if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
          }
          if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
            //printf("%s\n", buf);
            find(buf, target);
          }
        }
        break;
    }
    close(fd);
}

int main (int argc, char* argv[]) {
    if (argc <= 2) {
        printf("\n");
    } else {
        find(argv[1], argv[2]);
    }  
    exit(0);
}
