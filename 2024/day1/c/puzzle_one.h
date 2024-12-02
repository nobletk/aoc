#ifndef PUZZLE_ONE_H
#define PUZZLE_ONE_H

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define INITIAL_ARRAY_SZ 125

int add_val_to_arr(const char *line, const size_t list_size, int *left_arr,
                   int *right_arr);
int get_result(const char *filename);

#endif
