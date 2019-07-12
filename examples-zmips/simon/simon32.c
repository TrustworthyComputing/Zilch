#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define ROUNDS 32
#define KEY_LEN 4
#define WORD_SIZE 16
#define WORD_MASK ((0x1ull << (WORD_SIZE&63)) - 1)
#define C ((0xffffffffffffffffull ^ 0x3ull) & WORD_MASK)

#define ROR(x, r) ((x >> r) | (x << ((sizeof(uint16_t) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(uint16_t) * 8) - r)))


uint16_t z[5][62] = {
    {1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
    {1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
    {1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
    {1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
    {1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
};

void simon_expand(uint16_t K[static ROUNDS]) {
    uint16_t tmp;
    for (int i = KEY_LEN; i < ROUNDS; ++i) {
        tmp = ROR(K[i-1], 3);
        tmp ^= K[i-3];
        tmp ^= ROR(tmp, 1);
        K[i] = K[i-KEY_LEN] ^ z[0][(i-KEY_LEN) % 62] ^ tmp ^ C;
    }
}

void simon_32_64_encrypt(uint16_t const pt[static 2], uint16_t ct[static 2], uint16_t const K[static ROUNDS]) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    uint16_t tmp;
    for (int i = 0; i < ROUNDS; ++i) {
        tmp = ct[0];
        ct[0] = ct[1] ^ (ROL(ct[0], 1) & ROL(ct[0], 8)) ^ ROL(ct[0], 2) ^ K[i];
        ct[1] = tmp;
    }
}

void simon_32_64_decrypt(uint16_t const ct[static 2], uint16_t pt[static 2], uint16_t const K[static ROUNDS]) {
    uint16_t tmp;
    pt[0] = ct[0];
    pt[1] = ct[1];
    for (int i = 0; i < ROUNDS; ++i) {
        tmp = pt[1];
        pt[1] = pt[0] ^ (ROL(pt[1], 1) & ROL(pt[1], 8)) ^ ROL(pt[1], 2) ^ K[ROUNDS-i-1];
        pt[0] = tmp;
    }
}

int main(void) {
    uint16_t plain_text[2] = { 0x6565, 0x6877 };
    const uint16_t cipher_text[2] = { 0xc69b, 0xe9bb };
    uint16_t buffer[2] = { 0 };

    uint16_t key[ROUNDS] = { 0x0100, 0x0908, 0x1110, 0x1918 };
    simon_expand(key);
    uint16_t exp[ROUNDS];
    for (size_t i = 0; i < ROUNDS; i++) {
        exp[i] = key[i];
    }
    // uint16_t exp[ROUNDS] = { 256, 2312, 4368, 6424, 29123, 46665, 22228, 57456, 61786, 50485, 56724, 16400, 9482, 28518, 59755, 19416, 4069, 31815, 57583, 15905, 1627, 17292, 62058, 46528, 34313, 40846, 55487, 2476, 59410, 10000, 11434, 36116};

    printf("Plain text: %" PRIu16 " %" PRIu16 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SIMON ENCRYPTION ==================\n");
    simon_32_64_encrypt(plain_text, buffer, exp);
    printf("==> Cipher text: %" PRIu16 " %" PRIu16 "\n\n", buffer[0], buffer[1]);
    if (buffer[0] != cipher_text[0] || buffer[1] != cipher_text[1]) {
        printf("Encryption failed!\n");
        return EXIT_FAILURE;
    }

    printf("================== SIMON DECRYPTION ==================\n");
    simon_32_64_decrypt(cipher_text, buffer, exp);
    if (buffer[0] != plain_text[0] || buffer[1] != plain_text[1]) {
        printf("Decryption failed\n");
        return EXIT_FAILURE;
    }
    printf("==> Plain text: %" PRIu16 " %" PRIu16 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);
    printf("Encryption and decryption success\n");

    return EXIT_SUCCESS;
}
