#include "functions.h"

#define BUFFER_SIZE 1024

const char *spelled_digits[] = {"one", "two",   "three", "four", "five",
                                "six", "seven", "eight", "nine"};
const int digit_replacement[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// find the first real and last real digits whether numerical or spelled
// parse from the beginning of the line looking for the first digit
int find_first_digit(const char *line, size_t *start_pos) {
    size_t line_len = strlen(line);

    while (*start_pos < line_len) {
        for (int i = 0; i < 9; i++) {
            size_t word_len = strlen(spelled_digits[i]);
            if (strncmp(line + *start_pos, spelled_digits[i], word_len) == 0) {
                *start_pos += word_len;
                return digit_replacement[i];
            }
        }

        if (isdigit(line[*start_pos])) {
            return line[*start_pos] - '0';
        }

        (*start_pos)++;
    }

    return -1;
}

// parse from the end of the line including the starting position looking for
// the last digit
int find_last_digit(const char *line, size_t *start_pos, size_t *end_pos) {
    while (*end_pos >= *start_pos) {
        for (int i = 0; i < 9; i++) {
            size_t word_len = strlen(spelled_digits[i]);
            if (*end_pos + 1 >= word_len &&
                strncmp(line + *end_pos - word_len + 1, spelled_digits[i],
                        word_len) == 0) {
                *end_pos -= word_len;
                return digit_replacement[i];
            }
        }

        if (isdigit(line[*end_pos])) {
            return line[*end_pos] - '0';
        }

        (*end_pos)--;
    }

    return -1;
}

int sum_first_and_last_digits(const char *line) {
    size_t line_len = strlen(line);

    size_t start = 0;
    size_t end = line_len - 1;

    int first_digit = find_first_digit(line, &start);
    printf("- First digit: %d", first_digit);

    int last_digit = find_last_digit(line, &start, &end);
    printf(", Last digit: %d ", last_digit);

    if (first_digit != -1 && last_digit == -1) {
        last_digit = first_digit;
    }

    if (first_digit != -1) {
        return first_digit * 10 + last_digit;
    }
    return 0;
}

int read_file_and_sum_digits(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return -1;
    }

    char *line = malloc(BUFFER_SIZE);
    if (line == NULL) {
        fprintf(stderr, "Error allocating memory: %s\n", strerror(errno));
        fclose(file);
        return -1;
    }

    int sum = 0;
    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = '\0';
        printf("Line: %s ", line);
        int combined = sum_first_and_last_digits(line);
        sum += combined;
        printf(", Digits: %d, Sum: %d\n", combined, sum);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file '%s' : %s\n", filename,
                strerror(errno));
    }

    free(line);
    fclose(file);
    return sum;
}
