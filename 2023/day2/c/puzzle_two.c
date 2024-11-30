#include "puzzle_two.h"

#define BUFFER_SIZE 1024

int check_color(const int *count, const char *color, const int *red,
                const int *green, const int *blue) {
    printf("Checking for Color: %s with Count:%d\n", color, *count);
    printf("Count: %d, Red: %d, Green: %d, Blue: %d\n", *count, *red, *green,
           *blue);

    if (strcmp(color, "red") == 0 && (*count <= *red)) return 0;
    if (strcmp(color, "green") == 0 && (*count <= *green)) return 0;
    if (strcmp(color, "blue") == 0 && (*count <= *blue)) return 0;

    return -1;
}

int parse_cube(const char *cube, const int *red, const int *green,
               const int *blue) {
    int count;
    char color[20];
    char *saveptr;

    char *subtoken = strtok_r((char *)cube, " ", &saveptr);
    if (!subtoken || sscanf(subtoken, "%d", &count) != 1) {
        fprintf(stderr, "Failed to parse count for cube: %s\n", cube);
        return -1;
    }

    subtoken = strtok_r(NULL, " ", &saveptr);
    if (!subtoken || sscanf(subtoken, "%19s", color) != 1) {
        fprintf(stderr, "Failed to parse color for cube: %s\n", cube);
        return -1;
    }

    printf("Found count: %d, color: %s\n", count, color);

    return check_color(&count, color, red, green, blue);
}

int get_game_id(const char *line, const int *red, const int *green,
                const int *blue) {
    char buffer[BUFFER_SIZE];
    char *saveptr1, *saveptr2;
    int game_id;

    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("\nLine: %s", buffer);

    char *game_token = strtok_r(buffer, ":", &saveptr1);
    if (!game_token || sscanf(game_token, "Game %d", &game_id) != 1) {
        printf("Failed to parse game ID for line: %s\n", buffer);
        return 0;
    }

    printf("Game ID: %d\n", game_id);

    char *set_token = strtok_r(NULL, ";", &saveptr1);
    while (set_token) {
        char *cube_token = strtok_r(set_token, ",", &saveptr2);
        while (cube_token) {
            if (parse_cube(cube_token, red, green, blue) != 0) {
                fprintf(stderr, "Validation failed for cube: %s\n", cube_token);
                return 0;
            }
            cube_token = strtok_r(NULL, ",", &saveptr2);
        }
        set_token = strtok_r(NULL, ";", &saveptr1);
    }

    return game_id;
}

int sum_of_ids(const int *red, const int *green, const int *blue,
               const char *filename) {
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

    int sum = 0;
    while (fgets(line, BUFFER_SIZE, file)) {
        int game_id = 0;
        game_id = get_game_id(line, red, green, blue);
        /* printf("Line: %s", line); */
        printf("Game ID to be added: %d\n", game_id);
        sum += game_id;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file '%s': %s\n", filename,
                strerror(errno));
    }

    free(line);
    fclose(file);
    return sum;
}
