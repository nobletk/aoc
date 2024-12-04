#include "puzzle.h"

void print_arr(const int *arr, const int arr_sz, const char *arr_name,
               const int line_num) {
    printf("%s %d: [ ", arr_name, line_num);
    for (int i = 0; i < arr_sz; ++i) {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

void remove_lvl(int *arr, size_t *arr_sz, size_t idx) {
    if (idx > *arr_sz) {
        fprintf(stderr, "Memory allocation failed in remove_lvl");
        exit(EXIT_FAILURE);
    }

    // printf("Removing '%d' at %zu\n", arr[idx], idx);

    for (size_t i = idx; i < *arr_sz; ++i) {
        arr[i] = arr[i + 1];
    }

    (*arr_sz)--;
}

int is_rep_safe(const int *arr, const size_t arr_sz) {
    int positive = 1;
    int negative = 1;

    for (size_t i = 0; i < arr_sz - 1; ++i) {
        int diff = arr[i] - arr[i + 1];
        if (diff > 0) {
            negative = 0;
        }
        if (diff < 0) {
            positive = 0;
        }

        if (!(1 <= abs(diff) && abs(diff) <= 3)) {
            return 0;
        }
    }
    return 1 && (positive || negative);
}

void rmv_chk(int *ok, const int *arr, const size_t arr_sz, const int idx) {
    if (*ok) {
        return;
    }

    size_t cp_sz = arr_sz;
    int *arr_cpy = malloc(arr_sz * sizeof(int));

    for (size_t i = 0; i < arr_sz; ++i) {
        arr_cpy[i] = arr[i];
    }

    remove_lvl(arr_cpy, &cp_sz, idx);

    if (is_rep_safe(arr_cpy, cp_sz)) {
        *ok = 1;
    }
    free(arr_cpy);
    return;
}

// parse report and check for safety
int safe_chk(const char *line, const int line_num) {
    size_t arr_sz = 0;
    size_t arr_cap = 20;
    char *saveptr;

    int *arr = malloc(arr_cap * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Failed to allocate memory for rep_arr\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok_r((char *)line, " ", &saveptr);
    if (!token || sscanf(token, "%d", &arr[0]) != 1) {
        fprintf(stderr, "Failed to find level in line: %d\n", line_num);
        exit(EXIT_FAILURE);
    }
    arr_sz++;

    token = strtok_r(NULL, " ", &saveptr);
    int idx = 1;
    while (token) {
        if (arr_sz > arr_cap) {
            fprintf(stderr,
                    "Out of bounds: arr_sz is exceeding beyond capacity\n");
            exit(EXIT_FAILURE);
        }

        if (sscanf(token, "%d", &arr[idx]) != 1) {
            fprintf(stderr, "Failed to find level in line: %d\n", line_num);
            exit(EXIT_FAILURE);
        }

        idx++;
        arr_sz++;
        token = strtok_r(NULL, " ", &saveptr);
    }

    printf("\n");
    print_arr(arr, arr_sz, "Report", line_num);

    int safe = 0;
    rmv_chk(&safe, arr, arr_sz, 0);

    for (size_t i = 0; i < arr_sz; ++i) {
        int diff = arr[i] - arr[i + 1];
        if (abs(diff) < 1 || abs(diff) > 3) {
            rmv_chk(&safe, arr, arr_sz, i);
            rmv_chk(&safe, arr, arr_sz, i + 1);
            break;
        }
        if (i + 2 < arr_sz) {
            int diff2 = arr[i + 1] - arr[i + 2];
            if ((diff > 0) != (diff2 > 0)) {
                rmv_chk(&safe, arr, arr_sz, i);
                rmv_chk(&safe, arr, arr_sz, i + 1);
                rmv_chk(&safe, arr, arr_sz, i + 2);
                break;
            }
        }
    }

    if (safe) {
        printf("Report %d is SAFE\n", line_num);
    } else {
        printf("Report %d is UNSAFE\n", line_num);
    }

    free(arr);
    return safe;
}

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
        free(line);
        fclose(file);
        return -1;
    }

    int safe_rep = 0;
    int line_num = 0;

    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = '\0';
        line_num++;

        if (safe_chk(line, line_num)) {
            safe_rep++;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file '%s': %s\n", filename,
                strerror(errno));
    }

    free(line);
    fclose(file);
    return safe_rep;
}
