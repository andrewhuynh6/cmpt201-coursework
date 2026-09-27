#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *buff = NULL;
  size_t size = 0;
  while (1) {
    printf("Enter programs to run.\n");
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char != -1) {
      buff[num_char - 1] = '\0';
    } else if (num_char == -1) {
      perror("No programs were given\n");
      exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid < 0) {
      perror("Fork Failure\n");
      continue;
    }
    if (pid == 0) {
      if (execlp(buff, buff, NULL) == -1) {
        perror("Exec failure");
        exit(EXIT_FAILURE);
      }
    } else {
      int wstatus = 0;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(wstatus)) {
        printf("Child done\n");
      }
    }
  }
  free(buff);
}
