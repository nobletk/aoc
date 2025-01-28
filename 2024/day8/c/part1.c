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

                int row = r2 + (r2 - r);
                int col = c2 + (c2 - c);

                if (0 <= row && row < height && 0 <= col && col < width) {
                    if (!anti[row][col]) {
                        anti[row][col] = 1;
                        answer++;
                    }
                }
            }
        }
    }

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
