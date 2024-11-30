#include "puzzle_two.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s Cube numbers followed by file directory\n", argv[0]);
        printf("<red> <green> <blue> <filename>\n");
        printf("Example: 12 13 14 input.txt\n");
        return 1;
    }

    int red, green, blue;
    if (sscanf(argv[1], "%d", &red) != 1) {
        printf("Invalid input: %s is not a valid number.\n", argv[1]);
        return 1;
    }

    if (sscanf(argv[2], "%d", &green) != 1) {
        printf("Invalid input: %s is not a valid number.\n", argv[2]);
        return 1;
    }

    if (sscanf(argv[3], "%d", &blue) != 1) {
        printf("Invalid input: %s is not a valid number.\n", argv[3]);
        return 1;
    }

    int sum = sum_of_ids(&red, &green, &blue, argv[4]);
    if (sum < 0) {
        printf("Failed to calculate sum of game IDs!");
        return 1;
    }
    printf("\nTotal sum of game IDs: %d\n", sum);
    return 0;
}
