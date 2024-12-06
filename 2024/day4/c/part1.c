#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SZ 4056
#define MX_SZ 140
#define TEMP "XMAS"

int calc_res(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return -1;
    }

    char line[BUFFER_SZ];
    char mx[MX_SZ][MX_SZ];
    int i = 0, line_num = MX_SZ;
    while (fgets(line, BUFFER_SZ, file) && i < line_num) {
        line[strcspn(line, "\n")] = '\0';
        for (int j = 0; j < MX_SZ; ++j) {
            if (j < strlen(line)) {
                mx[i][j] = line[j];
            } else {
                mx[i][j] = ' ';
            }
        }
        i++;
    }

    int dir[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1},
    };

    int res = 0;
    for (int row = 0; row < MX_SZ; ++row) {
        for (int col = 0; col < MX_SZ; ++col) {
            if (mx[row][col] == 'X') {
                for (int d = 0; d < 8; ++d) {
                    int valid = 1;
                    for (int i = 0; i < 4; ++i) {
                        int x = row + i * dir[d][0];
                        int y = col + i * dir[d][1];

                        if (x < 0 || x >= MX_SZ || y < 0 || y >= MX_SZ) {
                            valid = 0;
                            break;
                        }

                        if (TEMP[i] != mx[x][y]) {
                            valid = 0;
                            break;
                        }
                    }

                    if (valid && (strncmp(TEMP, "XMAS", 4) == 0)) {
                        res++;
                    }
                }
            }
        }
    }

    fclose(file);
    return res;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int res = calc_res(argv[1]);
    if (res <= 0) {
        fprintf(stderr, "Failed to calculate result");
        return 1;
    }
    printf("\nResult: %d\n", res);

    return 0;
}
