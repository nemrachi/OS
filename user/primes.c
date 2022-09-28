#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int getOnlyPrimes(int p);
void forkPrimes(int pPipe);
int isPrime(int n);

// !!
// pipe[0] for read
// pipe[1] for write
// !!

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(int));    
    }

    close(p[1]);
    forkPrimes(getOnlyPrimes(p[0]));
    close(p[0]);

    exit(0);
}

int getOnlyPrimes(int p) {
    int num;
    int pp[2];
    pipe(pp);

    while (1) {
        if (read(p, &num, sizeof(int)) <= 0) {
            break;
        }
        if (isPrime(num)) {
            write(pp[1], &num, sizeof(int));
        }
    }

    close(p);
    close(pp[1]);
    return pp[0];
}

void forkPrimes(int pPipe) {
    int num;
    read(pPipe, &num, sizeof(int));
    printf("prime %d\n", num);
    
    int chPipe[2];
    pipe(chPipe);

    while (1) {
        if (read(pPipe, &num, sizeof(int)) <= 0) {
            break;
        }
        write(chPipe[1], &num, sizeof(int));
    }

    if (fork() == 0) {
        close(pPipe);
        close(chPipe[1]);
        forkPrimes(chPipe[0]);
        close(chPipe[0]);
    } else {
        close(pPipe);
        close(chPipe[1]);
        close(chPipe[0]);
        wait(0);
    }

    return;
}

int isPrime(int n) {
    if (n <= 1) {
        return 0; // false
    }
    for (int i = 2; i <= n/2; i++) {
        if (n % i == 0) {
            return 0; // false
        }
    }
    return 1; // true
}