#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    int i;
    char buff[512], ch;
    char *xArgs[MAXARG], *p = buff;
    int whiteSpace = 0, off = 0;

    if (argc-1 >= MAXARG) {
        fprintf(2, "xargs: maximum amount of args exceeded (max: %d, actual amount: %d)", MAXARG, argc-1);
        exit(1);
    }
    if (argc <= 1) {
        fprintf(2, "xargs: minimum amount of args required (min: 1)");
    }

    for (i = 1; i < argc; i++) {
        xArgs[i-1] = argv[i]; // copy
    }
    i--;

    while (read(0, &ch, sizeof(char))) {
        if (ch == ' ' || ch == '\t') {
            whiteSpace++;
            continue;
        }
        
        if (whiteSpace) {
            buff[off] = 0;
            xArgs[i++] = p;
            p = buff + off;
            whiteSpace = 0;
        }

        if (ch != '\n') {
            buff[off++] = ch;
        } else {
            xArgs[i++] = p;
            p = buff + off;
            if (fork() == 0) {
                exit(exec(xArgs[0], xArgs));
            }
            wait(0);
            i = argc - 1;
        }
    }

    exit(0);

    // char buf[2048], ch;
	// char *p = buf;
	// char *v[MAXARG];
	// int c;
	// int blanks = 0;
	// int offset = 0;

	// if(argc <= 1){
	// 	fprintf(2, "usage: xargs <command> [argv...]\n");
	// 	exit(1);
	// }

	// for (c = 1; c < argc; c++) {
	// 	v[c-1] = argv[c];
	// }
	// --c;

	// while (read(0, &ch, 1) > 0) {
	// 	if ((ch == ' ' || ch == '\t')) {
	// 		blanks++;
	// 		continue;
	// 	}

	// 	if (blanks) {  // 之前有过空格
	// 		buf[offset++] = 0;

	// 		v[c++] = p;
	// 		p = buf + offset;

	// 		blanks = 0;
	// 	}

	// 	if (ch != '\n') {
	// 		buf[offset++] = ch;
	// 	} else {
	// 		v[c++] = p;
	// 		p = buf + offset;

	// 		if (!fork()) {
	// 			exit(exec(v[0], v));
	// 		}
	// 		wait(0);
			
	// 		c = argc - 1;
	// 	}
	// }

	// exit(0);
}