#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#define KEY_LEN 4 // 4 * 32 = 128 bits
#define ROUNDS 27

#define ROR(x, r) ((x >> r) | (x << ((sizeof(uint32_t) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(uint32_t) * 8) - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)

#define RR(x, y, k) (y ^= x, y = ROR(y, 3), x ^= k, x -= y, x = ROL(x, 8))

void speck_64_128_expand(uint32_t const K[KEY_LEN], uint32_t S[ROUNDS]) {
    uint32_t b = K[0];
    uint32_t a[KEY_LEN - 1];
    for (int i = 0; i < KEY_LEN - 1 ; i++) {
        a[i] = K[i + 1];
    }
    S[0] = b;  
    for (int i = 0; i < ROUNDS - 1; i++) {
        R(a[i % (KEY_LEN - 1)], b, i);
        S[i + 1] = b;
    }
}

void speck_64_128_encrypt(const uint32_t pt[2], uint32_t ct[2], const uint32_t K[ROUNDS]) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    for (int i = 0; i < ROUNDS; i++) {
        R(ct[1], ct[0], K[i]);
    }
}

void speck_64_128_decrypt(const uint32_t ct[2], uint32_t pt[2], const uint32_t K[ROUNDS]) {
    pt[0] = ct[0];
    pt[1] = ct[1];
    for (int i = 0; i < ROUNDS; i++) {
        RR(pt[1], pt[0], K[(ROUNDS - 1) - i]);
    }
}

void speck_64_128_hash(uint32_t hash[2], const uint32_t pt[2], const uint32_t ct[2]) {
    hash[0] = pt[0] ^ ct[0];
    hash[1] = pt[1] ^ ct[1];
}


int main(void) {
    const uint32_t plain_text[2] = { 1953841965, 997352820 };                    // 3b726574 7475432d
    const uint32_t cipher_text[2] = { 1162740363, 2356127048 };                  // 8c6fa548 454e028b
    const uint32_t hash_result[2] = { 825967014, 3072180284 };
    const uint32_t key[KEY_LEN] = { 50462976, 185207048, 319951120, 454695192 }; // test-vector 1b1a1918 13121110 0b0a0908 03020100
    uint32_t exp[ROUNDS];
    uint32_t buffer[2] = { 0 };
    uint32_t hash[2] = { 0 };

    speck_64_128_expand(key, exp);
    for (int i = 0; i < ROUNDS; ++i) {
        printf("%u ", exp[i]);
    }
    printf("\n");

    printf("Plain text: %" PRIu32 " %" PRIu32 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SPECK ENCRYPTION ==================\n");
    speck_64_128_encrypt(plain_text, buffer, exp);
    printf("==> Cipher text: %" PRIu32 " %" PRIu32 "\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == cipher_text[0]);
    assert(buffer[1] == cipher_text[1]);

    printf("================== SPECK HASH ========================\n");
    speck_64_128_hash(hash, plain_text, buffer);
    assert(hash[0] == hash_result[0]);
    assert(hash[1] == hash_result[1]);
    printf("==> Speck hash: %" PRIu32 " %" PRIu32 "\n\n", hash[0], hash[1]);

    printf("================== SPECK DECRYPTION ==================\n");
    speck_64_128_decrypt(buffer, buffer, exp);
    assert(buffer[0] == plain_text[0]);
    assert(buffer[1] == plain_text[1]);
    printf("==> Plain text: %" PRIu32 " %" PRIu32 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);

    printf("Encryption, hash and decryption success\n");

    return EXIT_SUCCESS;
}
