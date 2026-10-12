#define _GNU_SOURCE
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 1024
// header struct
struct header {
  uint64_t size;
  struct header *next;
};

void *increase_heap_size(intptr_t size) {
  void *new_memory = sbrk(size);
  if (new_memory == (void *)-1) {
    perror("Failed to Increase Heap Size");
    exit(EXIT_FAILURE);
  }
  return new_memory;
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(
      buf, BUF_SIZE, format,
      (uint64_t)(data_size == sizeof(uint64_t) ? *(uint64_t *)data : (uint64_t) * (void **)data));
  if (len < 0) {
    perror("sprintf failure");
    exit(EXIT_FAILURE);
  }

  write(STDOUT_FILENO, buf, len);
}

int main() {
  const intptr_t EXTRA_SIZE = 256;
  // increase the heap size
  void *allocated_memory = increase_heap_size(EXTRA_SIZE);
  // initialize the header
  struct header *first_block = (struct header *)allocated_memory;
  first_block->size = 128;
  first_block->next = NULL;

  print_out("%p\n", &first_block, sizeof(&first_block));

  struct header *second_block = (struct header *)(char *)allocated_memory + 128;
  second_block->size = 128;
  second_block->next = first_block;

  print_out("%p\n", &second_block, sizeof(&second_block));

  // print header information
  printf("first block's size: %" PRIu64 "next : %p \n", first_block->size, first_block->next);
  printf("second block's size: %" PRIu64 "next : %p \n", second_block->size, second_block->next);

  // initialize the data of the first block to 0's and 1's
  memset((char *)first_block + sizeof(struct header), 0, first_block->size);
  memset((char *)second_block + sizeof(struct header), 1, second_block->size);

  char *datas_1 = (char *)(first_block + 1);
  char *datas_2 = (char *)(second_block + 1);
  // print out each byte for first and second block
  for (uint64_t i = 0; i < first_block->size; i++) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, "%u\n", datas_1[i]);
    write(STDOUT_FILENO, buf, len);
  }

  for (uint64_t i = 0; i < second_block->size; i++) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, "%u\n", datas_2[i]);
    write(STDOUT_FILENO, buf, len);
  }
}
