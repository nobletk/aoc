#include "puzzle_one.h"

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int result = get_result(argv[1]);
    if (result <= 0) {
        fprintf(stderr, "Failed to calculate total distance!\n");
        return 1;
    }
    printf("\nFinal result: %d\n", result);

    return 0;
}
