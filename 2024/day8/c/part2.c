#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 50
#define MAX_COLS 50
#define MAX_CHARS 256

typedef struct {
    int row, col;
} Pair;

typedef struct {
    Pair *pairs;
    int size;
    int capacity;
} PairArray;

void initPairArray(PairArray *arr) {
    arr->size = 0;
    arr->capacity = 4;
    arr->pairs = malloc(arr->capacity * sizeof(Pair));
}

void addPair(PairArray *arr, int row, int col) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->pairs = realloc(arr->pairs, arr->capacity * sizeof(Pair));
    }
    arr->pairs[arr->size].row = row;
    arr->pairs[arr->size].col = col;
    arr->size++;
}

void freePairArray(PairArray *arr) { free(arr->pairs); }

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);

    if (a == 0) {
        return b;
    }

    if (b == 0) {
        return a;
    }

    if (a == b) {
        return a;
    }

    if (a > b) {
        return gcd(a - b, b);
    }

    return gcd(a, b - a);
}

int main() {
    int height = -1;
    int width = -1;
    int answer = 0;

    char arr[MAX_ROWS][MAX_COLS];
    PairArray pos_arr[MAX_CHARS];

    for (int i = 0; i < MAX_CHARS; i++) {
        initPairArray(&pos_arr[i]);
    }

    for (int row = 0; scanf(" %s", arr[row]) != EOF; row++) {
        height = row + 1;
        width = strlen(arr[row]);
        for (int col = 0; col < width; col++) {
            if (arr[row][col] != '.') {
                addPair(&pos_arr[(int)arr[row][col]], row, col);
            }
        }
    }

    int **anti = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        anti[i] = calloc(width, sizeof(int));
    }

    for (int i = 0; i < MAX_CHARS; i++) {
        const PairArray *p = &pos_arr[i];
        for (int j = 0; j < p->size; j++) {
            for (int k = 0; k < p->size; k++) {
                if (j == k) {
                    continue;
                }
                int r = p->pairs[j].row;
                int c = p->pairs[j].col;
                int r2 = p->pairs[k].row;
                int c2 = p->pairs[k].col;
                int dr = r2 - r;
                int dc = c2 - c;
                int g = gcd(dr, dc);
                dr /= g;
                dc /= g;

                for (int rep = 0; rep < 2; rep++) {
                    for (int m = 0; 1; m++) {
                        int row = r + dr * m;
                        int col = c + dc * m;

                        if (0 <= row && row < height && 0 <= col &&
                            col < width) {
                            if (!anti[row][col]) {
                                anti[row][col] = 1;
                                answer++;
                            }
                        } else {
                            break;
                        }
                    }
                    dr *= -1;
                    dc *= -1;
                }
            }
        }
    }

    /*
    printf("\nFinal Grid state:\n");
    for (int row = 0; row < height; row++) {
        printf("%d: ", row);
        for (int col = 0; col < width; col++) {
            if (anti[row][col] && arr[row][col] == '.') {
                printf("# ");
            } else {
                printf("%c ", arr[row][col]);
            }
        }
        printf("\n");
    }
    */

    for (int i = 0; i < height; i++) {
        free(anti[i]);
    }
    free(anti);

    for (int i = 0; i < MAX_CHARS; i++) {
        freePairArray(&pos_arr[i]);
    }

    printf("\nAnswer: %d\n", answer);

    return 0;
}
