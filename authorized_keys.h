#ifndef AUTHORIZED_KEYS_H
#define AUTHORIZED_KEYS_H

#include <stddef.h>
#include <stdbool.h>

// Structure to hold the list of keys
typedef struct {
    char **keys;
    size_t count;
} KeyList;

// Reads authorized keys from a file.
// Returns 0 on success, non-zero on failure.
int read_authorized_keys(const char *filename, KeyList *key_list);

// Checks if a specific key is in the list.
bool is_authorized_key(const KeyList *key_list, const char *key);

// Frees memory allocated for the KeyList
void free_key_list(KeyList *key_list);

#endif

