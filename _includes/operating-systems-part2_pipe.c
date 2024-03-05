#include <unistd.h>             // fork, pipe, read, write, close
#include <sys/wait.h>           // waitpid
#include <stdio.h>              // printf

const char MSG[] = "Hello World! :)";

int main(int, char**) {
  int fds[2];                   // reading, writing
  pipe(fds);
  const pid_t pid = fork();
  if (pid == 0) {               // child
    close(fds[1]);              // not writing
    char buffer[sizeof(MSG)];
    read(fds[0], buffer, sizeof(buffer));
    printf("Read from pipe bridging processes: %s.\n", buffer);
    close(fds[0]);
    return 0;
  }
  close(fds[0]);                // not reading
  write(fds[1], (void*)MSG, sizeof(MSG));
  waitpid(pid, NULL, 0);
  close(fds[1]);
  return 0;
}
