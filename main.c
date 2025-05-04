#include "authorized_keys.h"
#include <stdio.h>

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

    const char *good_key = "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIKm+gtptToHUWVIVd52pDPXcw4oDnno5ZrCkYIv79lUr jesse@jessemoore.dev";
    if (is_authorized_key(&keys, good_key)) {
        printf("The good key is authorized.\n");
    } else {
        printf("The good key is NOT authorized.\n");
    }

    const char *bad_key = "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIKm+gtptToHUWVIVd52pDPXcw4oDnno5ZrCkYIv79lUr BOGUS@jessemoore.dev";
    if (is_authorized_key(&keys, bad_key)) {
        printf("The bad key IS authorized.\n");
    } else {
        printf("The bad key is not authorized.\n");
    }

    free_key_list(&keys);
    return 0;
}
