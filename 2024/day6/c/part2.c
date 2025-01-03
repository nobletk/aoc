#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SZ 1024
#define MAX_SZ 130

typedef struct {
    int row;
    int col;
    int dir;
} State;

typedef struct {
    int row;
    int col;
} Pos;

int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int sim_guard(char grid[MAX_SZ][MAX_SZ], Pos guard_start, Pos obstacle) {
    int visited[MAX_SZ * MAX_SZ * 4] = {0};

    grid[obstacle.row][obstacle.col] = '#';

    State guard_state = {guard_start.row, guard_start.col, 0};

    while (1) {
        int idx =
            (guard_state.row * MAX_SZ + guard_state.col) * 4 + guard_state.dir;

        if (visited[idx]) {
            grid[obstacle.row][obstacle.col] = '.';
            return 1;
        }

        visited[idx] = 1;

        int r2 = guard_state.row + dirs[guard_state.dir][0];
        int c2 = guard_state.col + dirs[guard_state.dir][1];

        if (!(0 <= r2 && r2 < MAX_SZ && 0 <= c2 && c2 < MAX_SZ)) {
            break;
        }

        if (grid[r2][c2] == '.') {
            guard_state.row = r2;
            guard_state.col = c2;
        } else {
            guard_state.dir = (guard_state.dir + 1) % 4;
        }
    }

    grid[obstacle.row][obstacle.col] = '.';

    return 0;
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
    char grid[MAX_SZ][MAX_SZ];
    int row = 0;
    while (fgets(line, BUFFER_SZ, file) && row < MAX_SZ) {
        line[strcspn(line, "\n")] = '\0';
        for (int col = 0; col < MAX_SZ; ++col) {
            grid[row][col] = line[col];
        }
        row++;
    }

    fclose(file);

    Pos guard_start = {-1, -1};
    int dir = 0;
    for (int row = 0; row < MAX_SZ; row++) {
        for (int col = 0; col < MAX_SZ; col++) {
            if (grid[row][col] == '^') {
                guard_start.row = row;
                guard_start.col = col;
                grid[row][col] = '.';
            }
        }
    }

    int valid_obstacle_count = 0;

    for (int row = 0; row < MAX_SZ; row++) {
        for (int col = 0; col < MAX_SZ; col++) {
            if (grid[row][col] == '.') {
                Pos obstacle = {row, col};
                if (sim_guard(grid, guard_start, obstacle)) {
                    valid_obstacle_count++;
                }
            }
        }
    }

    printf("Result: %d\n", valid_obstacle_count);

    return 0;
}
