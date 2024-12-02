#include "puzzle_one.h"

typedef struct KeyValuePair {
    int key;
    int value;
    struct KeyValuePair *next;
} KeyValuePair;

typedef struct HashMap {
    KeyValuePair **buckets;
    size_t size;
} HashMap;

unsigned int hash(int key, size_t size) { return (unsigned int)key % size; }

HashMap *create_hash_map(size_t size) {
    HashMap *map = malloc(sizeof(HashMap));
    if (!map) {
        fprintf(stderr, "Failed to allocate hash map\n");
        exit(EXIT_FAILURE);
    }
    map->buckets = malloc(size * sizeof(KeyValuePair *));
    if (!map->buckets) {
        fprintf(stderr, "Failed to allocate buckets\n");
        free(map);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < size; i++) {
        map->buckets[i] = NULL;
    }
    map->size = size;
    return map;
}

void map_insert(HashMap *map, int key) {
    unsigned int idx = hash(key, map->size);
    KeyValuePair *curr = map->buckets[idx];

    while (curr) {
        if (curr->key == key) return;
        curr = curr->next;
    }

    KeyValuePair *new_pair = malloc(sizeof(KeyValuePair));
    if (!new_pair) {
        fprintf(stderr, "Failed to allocate key-value pair");
        exit(EXIT_FAILURE);
    }
    new_pair->key = key;
    new_pair->value = 0;
    new_pair->next = map->buckets[idx];
    map->buckets[idx] = new_pair;
}

int search_map(HashMap *map, int key) {
    unsigned int idx = hash(key, map->size);
    KeyValuePair *curr = map->buckets[idx];
    while (curr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }

    return 0;
}

void incr_map_vals(HashMap *map, const int *list, const size_t list_size) {
    for (size_t i = 0; i < list_size; ++i) {
        int key = list[i];
        unsigned int idx = hash(key, map->size);
        KeyValuePair *curr = map->buckets[idx];
        // printf("Looking for elm: %d, found in map(%d)\n", list[i], idx);

        while (curr) {
            if (curr->key == key) {
                curr->value++;
                // printf("Key found: %d -> %d\n", curr->key, curr->value);
                break;
            }
            curr = curr->next;
        }
    }
    return;
}

int calc_sim_score(HashMap *map, const int *list, const int list_size) {
    int sim_score = 0;

    for (int i = 0; i < list_size; ++i) {
        int elem = list[i];
        int num_of_occur = search_map(map, elem);
        sim_score += elem * num_of_occur;
    }
    return sim_score;
}

void free_map(HashMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        KeyValuePair *curr = map->buckets[i];
        while (curr) {
            KeyValuePair *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(map->buckets);
    free(map);
}

HashMap *list_to_map(int *list, size_t list_size) {
    HashMap *map = create_hash_map(list_size);

    for (size_t i = 0; i < list_size; ++i) {
        map_insert(map, list[i]);
    }
    return map;
}

void print_map(HashMap *map) {
    printf("HashMap : {\n");
    for (size_t i = 0; i < map->size; i++) {
        KeyValuePair *curr = map->buckets[i];
        while (curr) {
            printf("  %d: %d\n", curr->key, curr->value);
            curr = curr->next;
        }
    }
    printf("}\n");
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int *list, int low, int high) {
    int pivot = list[high];

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (list[j] < pivot) {
            i++;
            swap(&list[i], &list[j]);
        }
    }

    swap(&list[i + 1], &list[high]);
    return i + 1;
}

void quick_sort(int *list, int low, int high) {
    if (low < high) {
        int pivot_idx = partition(list, low, high);

        quick_sort(list, low, pivot_idx - 1);
        quick_sort(list, pivot_idx + 1, high);
    }
}

void print_list(int *list, int list_size, const char *list_name) {
    printf("%s: [", list_name);
    for (int i = 0; i < list_size; ++i) {
        printf("%d, ", list[i]);
    }
    printf("]\n");
}

int add_val_to_list(const char *line, const size_t list_size, int *left_list,
                    int *right_list) {
    int left_num, right_num;
    char *saveptr1;
    int idx = list_size - 1;
    int line_num = list_size;

    // printf("Processing line %d: %s\n", line_num, line);

    char *subtoken = strtok_r((char *)line, " ", &saveptr1);
    if (!subtoken || sscanf(subtoken, "%d", &left_num) != 1) {
        fprintf(stderr, "Failed to find left_num in line: %d\n", line_num);
        return -1;
    }

    subtoken = strtok_r(NULL, " ", &saveptr1);
    if (!subtoken || sscanf(subtoken, "%d", &right_num) != 1) {
        fprintf(stderr, "Failed to find right_num in line: %d\n", line_num);
        return -1;
    }

    if (left_list[idx] != 0) {
        fprintf(
            stderr,
            "Failed to add left_num: %d to left_list[%zu], already occupied "
            "with '%d'!\n",
            left_num, list_size, left_list[idx]);
        return -1;
    }

    if (right_list[idx] != 0) {
        fprintf(
            stderr,
            "Failed to add right_num: %d to right_list[%zu], already occupied "
            "with '%d'!\n",
            right_num, list_size, right_list[idx]);
        return -1;
    }

    left_list[idx] = left_num;
    right_list[idx] = right_num;

    return 0;
}

int calc_total_dist(const int *left_list, const int *right_list,
                    const size_t list_size) {
    int total_dist = 0;

    for (size_t i = 0; i < list_size; ++i) {
        total_dist += abs(left_list[i] - right_list[i]);
    }

    return total_dist;
}

int get_result(const char *filename) {
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

    int total_dist = 0;
    size_t line_num = 0;
    size_t list_size = 0;
    size_t list_capacity = INITIAL_ARRAY_SZ;

    int *left_list = malloc(list_capacity * sizeof(int));
    int *right_list = malloc(list_capacity * sizeof(int));

    if (!left_list || !right_list) {
        fprintf(stderr, "Error allocating inital lists\n");
        free(line);
        free(left_list);
        free(right_list);
        fclose(file);
        return -1;
    }

    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = '\0';
        line_num++;
        list_size = line_num;

        // Resize the lists
        if (list_size >= list_capacity) {
            size_t old_capacity = list_capacity;
            list_capacity *= 2;
            int *temp_left = realloc(left_list, list_capacity * sizeof(size_t));
            int *temp_right =
                realloc(right_list, list_capacity * sizeof(size_t));

            if (!temp_left || !temp_right) {
                fprintf(stderr, "Error reallocating lists\n");
                free(line);
                free(left_list);
                free(right_list);
                fclose(file);
                return -1;
            }

            left_list = temp_left;
            right_list = temp_right;

            for (size_t i = old_capacity; i < list_capacity; ++i) {
                left_list[i] = 0;
                right_list[i] = 0;
            }
            /* printf("Doubled the list capacity: %zu -> %zu\n", list_size,
                    list_capacity); */
        }

        // Add each side to its own list
        int val_added = add_val_to_list(line, list_size, left_list, right_list);
        if (val_added != 0) {
            fprintf(stderr, "Error adding values from line: %zu to lists!\n",
                    line_num);
            return -1;
        }
    }

    // Sort list in ascending order
    quick_sort(left_list, 0, list_size - 1);
    quick_sort(right_list, 0, list_size - 1);

    /*
    print_list(left_list, list_size, "Left_list");
    print_list(right_list, list_size, "Right_list");
    */

    // Calculate distance between lists
    total_dist = calc_total_dist(left_list, right_list, list_size);
    printf("Total distance: %d\n", total_dist);

    // Create hash map for left list
    HashMap *map = list_to_map(left_list, list_size);

    // Increment values in map according to number of occurrences of key in
    // right_list
    incr_map_vals(map, right_list, list_size);
    // print_map(map);

    // Calculate similarity score each element from left_list * its value in
    // occurrence map
    int sim_score = calc_sim_score(map, left_list, list_size);

    free_map(map);
    free(left_list);
    free(right_list);
    free(line);
    fclose(file);

    return sim_score;
}
