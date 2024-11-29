#include "functions.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int sum = read_file_and_sum_digits(argv[1]);
    if (sum != -1) {
        printf("Total sum of digits: %d\n", sum);
    }
    return 0;
}
