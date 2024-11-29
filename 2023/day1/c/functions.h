#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int find_first_digit(const char *line, size_t *start_pos); 
int find_last_digit(const char *line, size_t *start_pos, size_t *end_pos); 
int sum_first_and_last_digits(const char* line);
int read_file_and_sum_digits(const char* filename);

#endif
