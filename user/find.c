#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *file);
char *fmtname(char *path);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "cmd usage: find path filename");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}

void find(char *path, char *file) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if (strcmp(fmtname(path), file)) {
		fprintf(1, "%s/%s\n", path, file);
	}

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type != T_DIR) {
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2,"find: path too long\n");
        close(fd);
        exit(1);
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
            
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (fstat(fd, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", path);
            continue;
        }
        
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..")){
            continue;
        }

        find(buf, file);
    }

    close(fd);
}

char* fmtname(char *path) { // from ls.c
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}