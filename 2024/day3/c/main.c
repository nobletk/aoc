#include "puzzle.h"

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
