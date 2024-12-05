#include "puzzle.h"

int calc_res(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return -1;
    }

    char *line = malloc(BUFFER_SIZE);
    if (!line) {
        fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
        fclose(file);
        return -1;
    }

    int res = 0, line_num = 0, enabled = 1;
    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = '\0';
        line_num++;

        int x, y;
        char sep;
        for (const char *i = line; *i != '\0'; ++i) {
            int advptr = 0;

            if (strncmp(i, "do()", 4) == 0) {
                enabled = 1;
                // printf("Found 'do()'\n");
            }

            if (strncmp(i, "don't())", 7) == 0) {
                enabled = 0;
                // printf("Found 'don\'t()'\n");
            }

            if (enabled && strncmp(i, "mul(", 4) == 0) {
                if (sscanf(i + 4, "%d%c%d%n", &x, &sep, &y, &advptr) == 3 &&
                    sep == ',' && *(i + 4 + advptr) == ')') {
                    /*
                    printf("x: %d, sep: '%c', y: %d, consumed: %d\n", x, sep, y,
                           advptr);
                    printf(">> %d * %d = %d, enabled: %d\n", x, y, x * y,
                           enabled);
                    */
                    res += x * y;
                    i += 4 + advptr;
                } else {
                    i += 3 + advptr;
                }
            }
        }
    }

    free(line);
    fclose(file);
    return res;
}
