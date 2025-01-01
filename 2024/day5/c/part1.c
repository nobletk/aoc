#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SZ 1024
#define MAX_PAGES 100

typedef struct Node {
    int page;
    struct Node *next;
} Node;

typedef struct {
    int page;
    Node *edges;
} RuleMap;

RuleMap rule_map[MAX_PAGES];
int rule_count = 0;

int find_or_create_map_entry(int page) {
    for (int i = 0; i < rule_count; i++) {
        if (rule_map[i].page == page) {
            return i;  // Key exists;
        }
    }

    if (rule_count < MAX_PAGES) {
        rule_map[rule_count].page = page;
        rule_map[rule_count].edges = NULL;
        return rule_count++;
    }

    fprintf(stderr, "Error: Too many unique pages in rules.\n");
    exit(EXIT_FAILURE);
}

void add_rule_to_map(int before, int after) {
    int idx = find_or_create_map_entry(before);

    Node *curr = rule_map[idx].edges;
    while (curr) {
        if (curr->page == after) {
            return;  // Rule already exists
        }
        curr = curr->next;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error allocating memory of rule node.\n");
        exit(EXIT_FAILURE);
    }
    new_node->page = after;
    new_node->next = rule_map[idx].edges;
    rule_map[idx].edges = new_node;
}

int is_rule_respected(int *pages, int page_count) {
    for (int i = 0; i < rule_count; i++) {
        int curr_page = rule_map[i].page;

        // Check if 'curr_page' exists in the update
        int curr_idx = -1;
        for (int j = 0; j < page_count; j++) {
            if (pages[j] == curr_page) {
                curr_idx = j;
                break;
            }
        }

        // Skip rules for 'curr_page' if it is not in the update
        if (curr_idx == -1) {
            continue;
        }

        // Traverse the "edges" list for the current page
        Node *curr = rule_map[i].edges;
        while (curr) {
            int after_page = curr->page;

            // Check if 'after_page' exists in the update
            int after_idx = -1;
            for (int j = 0; j < page_count; j++) {
                if (pages[j] == after_page) {
                    after_idx = j;
                    break;
                }
            }

            // Skip this rule if 'after_page' is not in the update
            if (after_idx == -1) {
                curr = curr->next;
                continue;
            }

            // If both pages are present, ensure the order is respected
            if (curr_idx >= after_idx) {
                printf("Rule violated: %d must come before %d\n", curr_page,
                       after_page);
                return 0;  // Rule violated
            }

            curr = curr->next;
        }
    }
    return 1;  // All rules respected
}

void process_rule_line(const char *line) {
    // printf("\nProcessing line: %s", line);
    int left, right;
    if (sscanf(line, "%d|%d", &left, &right) == 2) {
        // printf("Pair: %d | %d\n", left, right);
        add_rule_to_map(left, right);
    } else {
        fprintf(stderr, "Error: Invalid rule format in line: %s\n", line);
    }
}

int process_update_line(const char *line) {
    // printf("\nProcessing line: %s\n", line);
    int pages[MAX_PAGES];
    int idx = 0;

    char tmp[BUFFER_SZ];
    strncpy(tmp, line, BUFFER_SZ);
    tmp[BUFFER_SZ - 1] = '\0';

    char *tok = strtok(tmp, ",");
    // printf("Group: ");
    while (tok != NULL) {
        // printf("%s ", tok);
        pages[idx++] = atoi(tok);
        tok = strtok(NULL, ",");
    }
    // printf("\n");

    if (is_rule_respected(pages, idx)) {
        return pages[idx / 2];
        // printf("Update [%s] is in the correct order.\n", line);
    } else {
        printf("Update is NOT in the correct order: %s\n", line);
        return 0;
    }
}

void free_rule_map() {
    for (int i = 0; i < rule_count; i++) {
        Node *curr = rule_map[i].edges;
        while (curr) {
            Node *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename,
                strerror(errno));
        return 1;
    }

    char line[BUFFER_SZ];
    int in_update_sec = 0;
    int answer = 0;

    while (fgets(line, BUFFER_SZ, file)) {
        if (line[0] == '\n' || line[0] == '\0') {
            in_update_sec = 1;
            continue;
        }

        int is_pair = 1;
        if (strchr(line, ',') != NULL) {
            is_pair = 0;
        }

        if (!in_update_sec) {
            process_rule_line(line);
        } else {
            answer += process_update_line(line);
        }
    }

    printf("Answer: %d\n", answer);

    fclose(file);
    free_rule_map();
    return 0;
}
