#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

// assign the address of the input string to the array of string pointers
void storeInput(char *input, int counter, char **input_array) { input_array[counter % 5] = input; }

// if the counter is smaller than or = 5 start from 0, else start from the oldest index
// For example if counter = 6, with F B C D E being the input strings
//                                     5 1 2 3 4, we have to start from 1, so start = 6 - min(6,5)=1
void print_history(char **input_array, int counter) {
  int count = MIN(counter, 5);
  int start = counter - count;
  for (int i = start; i < counter; i++) {
    printf("%s\n", input_array[i % 5]);
  }
}

int main() {
  // while loop to get user input
  char *input_history[5] = {};
  size_t counter = 0;

  while (1) {
    // get input with buffer and getline
    size_t size = 0;
    char *buffer = NULL;
    printf("Enter input: ");
    size_t num_char = getline(&buffer, &size, stdin);

    if (num_char == -1) {
      perror("getline failed");
      exit(EXIT_FAILURE);
    }

    buffer[num_char - 1] = '\0';

    // store input
    storeInput(buffer, counter, input_history);
    counter++;

    if (strcmp(buffer, "print") == 0) {
      print_history(input_history, counter);
    }
  }
}
