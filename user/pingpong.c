#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char recv_buff;
    int pPipe[2];
    int chPipe[2];
    pipe(pPipe);
    pipe(chPipe);

    if (fork() == 0) {
        read(chPipe[0], &recv_buff, 1);
        printf("%d: received ping\n", getpid());
        write(pPipe[1], "a", 1);
    } else {
        write(chPipe[1], "a", 1);
        read(pPipe[0], &recv_buff, 1);
        printf("%d: received pong\n", getpid());
    }

    exit(0);
}
