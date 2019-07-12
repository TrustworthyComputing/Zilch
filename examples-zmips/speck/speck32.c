#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#define KEY_LEN 4 // 4 * 16 = 64 bits
#define ROUNDS 22

#define ROR(x, r) ((x >> r) | (x << ((sizeof(uint16_t) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(uint16_t) * 8) - r)))
#define R(x, y, k) (x = ROR(x, 7), x += y, x ^= k, y = ROL(y, 2), y ^= x)
#define RR(x, y, k) (y ^= x, y = ROR(y, 2), x ^= k, x -= y, x = ROL(x, 7))

void speck_expand(uint16_t const K[KEY_LEN], uint16_t S[ROUNDS]) {
    uint16_t b = K[0];
    uint16_t a[KEY_LEN - 1];
    for (int i = 0; i < KEY_LEN - 1 ; i++) {
        a[i] = K[i + 1];
    }
    S[0] = b;  
    for (int i = 0; i < ROUNDS - 1; i++) {
        R(a[i % (KEY_LEN - 1)], b, i);
        S[i + 1] = b;
    }
}

void speck_32_64_encrypt(const uint16_t pt[2], uint16_t ct[2], const uint16_t K[ROUNDS]) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    for (int i = 0; i < ROUNDS; i++) {
        R(ct[1], ct[0], K[i]);
    }
}

void speck_32_64_decrypt(const uint16_t ct[2], uint16_t pt[2], const uint16_t K[ROUNDS]) {
    pt[0] = ct[0];
    pt[1] = ct[1];
    for (int i = 0; i < ROUNDS; i++) {
        RR(pt[1], pt[0], K[(ROUNDS - 1) - i]);
    }
}

void speck_32_64_hash(uint16_t hash[2], const uint16_t pt[2], const uint16_t ct[2]) {
    hash[0] = pt[0] ^ ct[0];
    hash[1] = pt[1] ^ ct[1];
}


int main(void) {
    const uint16_t plain_text[2] = { 26956, 25972 };
    const uint16_t cipher_text[2] = { 17138, 43112 };
    const uint16_t hash_result[2] = { 11198, 52508 };
    const uint16_t key[KEY_LEN] = { 256, 2312, 4368, 6424 };
    uint16_t exp[ROUNDS];
    // uint16_t exp[ROUNDS] = { 256, 5394, 24957, 5208, 26905, 30690, 3209, 52443, 61418, 20019, 30452, 22902, 61067, 56068, 17943, 62334, 34740, 36554, 60827, 14930, 33321, 60772 };
    uint16_t buffer[2] = { 0 };
    uint16_t hash[2] = { 0 };

    speck_expand(key, exp);
    for (int i = 0; i < ROUNDS; ++i) {
        printf("%u ", exp[i]);
    }
    printf("\n");

    printf("Plain text: %" PRIu16 " %" PRIu16 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SPECK ENCRYPTION ==================\n");
    speck_32_64_encrypt(plain_text, buffer, exp);
    assert(buffer[0] == cipher_text[0]);
    assert(buffer[1] == cipher_text[1]);
    printf("==> Cipher text: %" PRIu16 " %" PRIu16 "\n\n", buffer[0], buffer[1]);

    printf("================== SPECK HASH ========================\n");
    speck_32_64_hash(hash, plain_text, buffer);
    assert(hash[0] == hash_result[0]);
    assert(hash[1] == hash_result[1]);
    printf("==> Speck hash: %" PRIu16 " %" PRIu16 "\n\n", hash[0], hash[1]);

    printf("================== SPECK DECRYPTION ==================\n");
    speck_32_64_decrypt(buffer, buffer, exp);
    assert(buffer[0] == plain_text[0]);
    assert(buffer[1] == plain_text[1]);
    printf("==> Plain text: %" PRIu16 " %" PRIu16 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);

    printf("Encryption, hash and decryption success\n");

    return EXIT_SUCCESS;
}
