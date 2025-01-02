#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SZ 1024
#define MAX_SZ 130

typedef struct {
    int row;
    int col;
} Pos;

int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int is_in_set(Pos *set, int size, Pos pos) {
    for (int i = 0; i < size; i++) {
        if (set[i].row == pos.row && set[i].col == pos.col) {
            return 1;
        }
    }
    return 0;
}

void insert(Pos **set, int *size, int *capacity, Pos pos) {
    if (!is_in_set(*set, *size, pos)) {
        if (*size >= *capacity) {
            *capacity *= 2;
            *set = realloc(*set, *capacity * sizeof(Pos));
        }
        (*set)[(*size)++] = pos;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return 1;
    }

    char line[BUFFER_SZ];
    char array[MAX_SZ][MAX_SZ];
    int row = 0;
    while (fgets(line, BUFFER_SZ, file) && row < MAX_SZ) {
        line[strcspn(line, "\n")] = '\0';
        for (int col = 0; col < MAX_SZ; ++col) {
            array[row][col] = line[col];
        }
        row++;
    }

    fclose(file);

    Pos guard_pos = {-1, -1};
    int dir = 0;
    for (int row = 0; row < MAX_SZ; row++) {
        for (int col = 0; col < MAX_SZ; col++) {
            if (array[row][col] == '^') {
                guard_pos.row = row;
                guard_pos.col = col;
                array[row][col] = '.';
            }
        }
    }

    int capacity = 100;
    int visited_size = 0;
    Pos *visited_set = malloc(capacity * sizeof(Pos));

    while (1) {
        insert(&visited_set, &visited_size, &capacity, guard_pos);
        int r2 = guard_pos.row + dirs[dir][0];
        int c2 = guard_pos.col + dirs[dir][1];
        if (!(0 <= r2 && r2 < MAX_SZ && 0 <= c2 && c2 < MAX_SZ)) {
            break;  // out of bounds
        }
        if (array[r2][c2] == '.') {
            guard_pos.row = r2;
            guard_pos.col = c2;
        } else {
            dir = (dir + 1) % 4;  // changing dir when a block is detected
        }
    }

    printf("Result: %d\n", visited_size);

    return 0;
}
