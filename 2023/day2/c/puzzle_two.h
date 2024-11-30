#ifndef PUZZLE_TWO_H
#define PUZZLE_TWO_H

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_color(const int *count, const char *color, const int *red,
                const int *green, const int *blue);
int parse_cube(const char *cube, const int *red, const int *green,
               const int *blue);
int get_game_id(const char *line, const int *red, const int *green,
                const int *blue);
int sum_of_ids(const int *red, const int *green, const int *blue,
               const char *filename);

#endif
