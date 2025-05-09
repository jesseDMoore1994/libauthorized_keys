#ifndef AUTHORIZED_KEYS_H
#define AUTHORIZED_KEYS_H

#include <stddef.h>
#include <stdbool.h>

// Reads a single key from a .pub file.
// Returns a newly allocated string (caller must free), or NULL on failure.
char *read_public_key_file(const char *filename);

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

// Checks if a specific key is in the list.
bool is_authorized_key_in_file(const char *filename, const char *key);

// Frees memory allocated for the KeyList
void free_key_list(KeyList *key_list);

#endif

