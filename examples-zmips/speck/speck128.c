#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#define KEY_LEN 2 // 4 * 64 = 128 bits
#define ROUNDS 32

#define ROR(x, r) ((x >> r) | (x << ((sizeof(uint64_t) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(uint64_t) * 8) - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)

#define RR(x, y, k) (y ^= x, y = ROR(y, 3), x ^= k, x -= y, x = ROL(x, 8))

void speck_128_128_expand(uint64_t const K[KEY_LEN], uint64_t S[ROUNDS]) {
    uint64_t b = K[0];
    uint64_t a[KEY_LEN - 1];
    for (int i = 0; i < KEY_LEN - 1 ; i++) {
        a[i] = K[i + 1];
    }
    S[0] = b;  
    for (int i = 0; i < ROUNDS - 1; i++) {
        R(a[i % (KEY_LEN - 1)], b, i);
        S[i + 1] = b;
    }
}

void speck_128_128_encrypt(const uint64_t pt[2], uint64_t ct[2], const uint64_t K[ROUNDS]) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    
    printf("%d\n", ((sizeof(uint64_t) * 8) - 8));
    for (int i = 0; i < ROUNDS; i++) {
        R(ct[1], ct[0], K[i]);
    }
}

void speck_128_128_decrypt(const uint64_t ct[2], uint64_t pt[2], const uint64_t K[ROUNDS]) {
    pt[0] = ct[0];
    pt[1] = ct[1];
    for (int i = 0; i < ROUNDS; i++) {
        RR(pt[1], pt[0], K[(ROUNDS - 1) - i]);
    }
}

int main(void) {
    const uint64_t plain_text[2] = { 8388271400802151712U, 7809653424151160096U };      // 3b726574 7475432d
    const uint64_t cipher_text[2] = { 8674213117595946264U, 11987905258827821669U };    // 8c6fa548 454e028b
    const uint64_t key[KEY_LEN] = { 506097522914230528U, 1084818905618843912U };        // test-vector 1b1a1918 13121110 0b0a0908 03020100
    uint64_t exp[ROUNDS];
    uint64_t buffer[2] = { 0 };
    uint64_t hash[2] = { 0 };

    speck_128_128_expand(key, exp);
    for (int i = 0; i < ROUNDS; ++i) {
        printf("%" PRIu64 " ", exp[i]);
    }
    printf("\n");

    printf("Plain text: %" PRIu64 " %" PRIu64 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SPECK ENCRYPTION ==================\n");
    speck_128_128_encrypt(plain_text, buffer, exp);
    printf("==> Cipher text: %" PRIu64 " %" PRIu64 "\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == cipher_text[0]);
    assert(buffer[1] == cipher_text[1]);

    printf("================== SPECK DECRYPTION ==================\n");
    speck_128_128_decrypt(buffer, buffer, exp);
    assert(buffer[0] == plain_text[0]);
    assert(buffer[1] == plain_text[1]);
    printf("==> Plain text: %" PRIu64 " %" PRIu64 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);

    printf("Encryption, hash and decryption success\n");

    return EXIT_SUCCESS;
}
