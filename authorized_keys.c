#include "authorized_keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 1024
#define INITIAL_KEY_COUNT 10

char *read_public_key_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open public key file");
        return NULL;
    }

    char buffer[MAX_KEY_LENGTH];
    if (!fgets(buffer, sizeof(buffer), file)) {
        perror("Failed to read public key line");
        fclose(file);
        return NULL;
    }

    fclose(file);

    // Remove newline if present
    buffer[strcspn(buffer, "\r\n")] = '\0';

    // Duplicate the string to return
    char *key = strdup(buffer);
    if (!key) {
        perror("Memory allocation failed");
        return NULL;
    }

    return key;
}

int read_authorized_keys(const char *filename, KeyList *key_list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    size_t capacity = INITIAL_KEY_COUNT;
    key_list->keys = malloc(capacity * sizeof(char *));
    if (!key_list->keys) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    key_list->count = 0;
    char buffer[MAX_KEY_LENGTH];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = 0;  // strip newline

        if (strlen(buffer) == 0) continue; // if line empty, keep going

        // realloc more capacity if more space needed
        if (key_list->count >= capacity) {
            capacity *= 2;
            char **temp = realloc(key_list->keys, capacity * sizeof(char *));
            if (!temp) {
                perror("Memory reallocation failed");
                fclose(file);
                return 1;
            }
            key_list->keys = temp;
        }

        key_list->keys[key_list->count] = strdup(buffer);
        if (!key_list->keys[key_list->count]) {
            perror("Memory allocation failed for key");
            fclose(file);
            return 1;
        }

        key_list->count++;
    }

    fclose(file);
    return 0;
}

bool is_authorized_key(const KeyList *key_list, const char *key) {
    if (!key_list || !key || !key_list->keys) {
        printf("Key or Key list null!\n");
        return false;
    }

    printf("keys: %d\n", key_list->count);
    for (size_t i = 0; i < key_list->count; ++i) {
        if (strcmp(key_list->keys[i], key) == 0) {
            printf("key authorized!\n");
            return true; // Key found
        }
    }

    printf("key not authorized!\n");
    return false; // Not found
}

bool is_authorized_key_in_file(const char *filename, const char* key) {
    KeyList keys;
    bool res;

    if (read_authorized_keys(filename, &keys) != 0) {
        fprintf(stderr, "Failed to read authorized keys.\n");
        return 1;
    }

    res = is_authorized_key(&keys, key);

    free_key_list(&keys);
    return res;
}

void free_key_list(KeyList *key_list) {
    for (size_t i = 0; i < key_list->count; ++i) {
        free(key_list->keys[i]);
    }
    free(key_list->keys);
    key_list->keys = NULL;
    key_list->count = 0;
}

