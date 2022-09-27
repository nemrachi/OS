#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char recv_buff;
  int p_pipe[2];
  int ch_pipe[2];
  pipe(p_pipe);
  pipe(ch_pipe);

  if (fork() == 0) {
    read(ch_pipe[0], &recv_buff, 1);
    printf("%d: received ping\n", getpid());
    write(p_pipe[1], "a", 1);
  } else {
    write(ch_pipe[1], "a", 1);
    read(p_pipe[0], &recv_buff, 1);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}
