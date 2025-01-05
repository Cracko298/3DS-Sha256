#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

void sha256_hash(const uint8_t *data, size_t length, uint8_t *hash) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, length);
    sha256_final(&ctx, hash);
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    fsInit();

    FILE *file = fopen("sdmc:/testFile.txt", "rb");
    if (!file) {
        printf("Failed to open file\n");
        svcSleepThread(5000000000LL);
        gfxExit();
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *buffer = (uint8_t *)malloc(fileSize);
    if (!buffer) {
        printf("Failed to allocate memory\n");
        fclose(file);
        gfxExit();
        return 1;
    }
    fread(buffer, 1, fileSize, file);
    fclose(file);

    uint8_t hash[32];
    sha256_hash(buffer, fileSize, hash);

    printf("SHA-256 hash: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    svcSleepThread(5000000000LL);
    free(buffer);
    gfxExit();
    return 0;
}
