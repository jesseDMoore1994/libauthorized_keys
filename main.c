#include "authorized_keys.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static bool is_authorized(KeyList* keys, const char* pubfile) {
    char *key = read_public_key_file(pubfile);
    if (key) {
        printf("Read public key: %s\n", key);
    } else {
        fprintf(stderr, "Failed to read key from %s\n", pubfile);
        return false;
    }
    if (is_authorized_key(keys, key)) {
        printf("The good key is authorized.\n");
        free(key);
        return true;
    } else {
        printf("The good key is NOT authorized.\n");
        free(key);
        return false;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <authorized_keys_file>\n", argv[0]);
        return 1;
    }

    KeyList keys;
    if (read_authorized_keys(argv[1], &keys) != 0) {
        fprintf(stderr, "Failed to read authorized keys.\n");
        return 1;
    }

    printf("Loaded %zu authorized keys:\n", keys.count);
    for (size_t i = 0; i < keys.count; ++i) {
        printf("[%zu] %s\n", i + 1, keys.keys[i]);
    }

    if(!is_authorized(&keys, "good.pub")) {
        free_key_list(&keys);
        return 1;
    }
    if(is_authorized(&keys, "bad.pub")) {
        free_key_list(&keys);
        return 1;
    }

    free_key_list(&keys);

    char *key = read_public_key_file("good.pub");
    if(!is_authorized_key_in_file(argv[1], key)) {
        printf("good.pub is NOT authorized in %s!", argv[1]);
        return 1;
    }
    free(key);

    key = read_public_key_file("bad.pub");
    if(is_authorized_key_in_file(argv[1], key)) {
        printf("bad.pub IS authorized in %s!", argv[1]);
        return 1;
    }
    free(key);

    return 0;
}
