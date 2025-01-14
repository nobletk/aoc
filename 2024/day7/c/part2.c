#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SZ 1024

void check_exp(int i, long long sum, long long target, const long long *numbers,
               const int count, int *ok) {
    if (i == count) {
        if (sum == target) {
            *ok = 1;
        }
        return;
    }
    check_exp(i + 1, sum + numbers[i], target, numbers, count, ok);
    check_exp(i + 1, sum * numbers[i], target, numbers, count, ok);

    char sum_str[50];
    char num_str[50];
    snprintf(sum_str, sizeof(sum_str), "%lld", sum);
    snprintf(num_str, sizeof(num_str), "%lld", numbers[i]);

    char concat[100];
    snprintf(concat, sizeof(concat), "%s%s", sum_str, num_str);
    check_exp(i + 1, atoll(concat), target, numbers, count, ok);
}

void process_line(const char *line, long long *total) {
    long long target;
    long long numbers[100];
    int count = 0;

    char *colon = strchr(line, ':');
    if (!colon) {
        fprintf(stderr, "Invalid format: no colon found!\n");
        return;
    }

    *colon = '\0';
    target = atoll(line);

    char *tok = strtok(colon + 1, " ");
    while (tok) {
        numbers[count++] = atoi(tok);
        tok = strtok(NULL, " ");
    }

    if (count < 2) {
        printf("Invalid format: less than 2 numbers after colon!\n");
        return;
    }

    int ok = 0;
    check_exp(1, numbers[0], target, numbers, count, &ok);
    if (ok) {
        *total += target;
    }

    return;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
    }

    char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return 1;
    }

    char line[BUFFER_SZ];
    long long total = 0;

    while (fgets(line, BUFFER_SZ, file)) {
        line[strcspn(line, "\n")] = '\0';
        process_line(line, &total);
    }

    fclose(file);

    printf("\nSum: %lld\n", total);

    return 0;
}
