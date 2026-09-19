#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  while (1) {
    char *buff = NULL;
    size_t size = 0;
    printf("Please enter some text (type 'exit' to stop): ");
    ssize_t num_char = getline(&buff, &size, stdin);
    // break if user did not enter anything
    if (num_char == -1) {
      perror("getline failed");
      exit(EXIT_FAILURE);
    }

    // break if user enter 'exit'
    if (strcmp(buff, "exit\n") == 0) {
      break;
    }

    // print out the first token
    char *saveptr;
    char *ret = strtok_r(buff, " ", &saveptr);
    printf("Tokens: \n");
    printf("\t %s\n", ret);

    // print the rest
    char *ret2 = strtok_r(NULL, " ", &saveptr);
    while (ret2 != NULL) {
      printf("\t %s\n", ret2);
      ret2 = strtok_r(NULL, " ", &saveptr);
    }
    free(buff);
  }
}
