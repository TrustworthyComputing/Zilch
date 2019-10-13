#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define ROUNDS 68
#define KEY_WORDS 2
#define WORD_SIZE 64

#define ROL(x, r) ((x << r) | (x >> (WORD_SIZE - r)))
#define ROR(x, r) ((x >> r) | ((x & 7) << (WORD_SIZE - r)))

uint64_t z_arrays[5] = {0b0001100111000011010100100010111110110011100001101010010001011111,
                        0b0001011010000110010011111011100010101101000011001001111101110001,
                        0b0011001101101001111110001000010100011001001011000000111011110101,
                        0b0011110000101100111001010001001000000111101001100011010111011011,
                        0b0011110111001001010011000011101000000100011011010110011110001011};

void simon_expand(uint64_t *key_schedule, uint64_t *sub_keys) {
    uint64_t mod_mask = ULLONG_MAX >> (64 - WORD_SIZE);
    uint64_t tmp1,tmp2;
    uint64_t c = 0xFFFFFFFFFFFFFFFC; 
    
    memcpy(key_schedule, &sub_keys[0], WORD_SIZE/8);
    for (int i = 0; i < ROUNDS - 1; i++) {
        tmp1 = ROR(sub_keys[KEY_WORDS-1], 3);
        tmp2 = ROR(tmp1, 1);
        tmp1 ^= sub_keys[0];
        tmp1 ^= tmp2;
        tmp2 = c ^ ((z_arrays[2] >> (i % 62)) & 1);
        tmp1 ^= tmp2;
        for (int j = 0; j < KEY_WORDS-1; j++) {
            sub_keys[j] = sub_keys[j + 1];
        }
        sub_keys[KEY_WORDS-1] = tmp1 & mod_mask;
        memcpy(&key_schedule[i+1], &sub_keys[0], WORD_SIZE/8);
    }
}

void simon_encrypt_128(const uint64_t *round_key_ptr, uint64_t *pt, uint64_t *ct) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    for (size_t i = 0; i < ROUNDS; i++) {
        uint64_t temp = ROL(ct[1], 1) & ROL(ct[1], 8) ^ ct[0] ^ ROL(ct[1], 2);
        ct[0] = ct[1];
        ct[1] = temp ^ round_key_ptr[i];
    }
}

void simon_decrypt_128(const uint64_t *round_key_ptr, uint64_t *ct, uint64_t *pt){
    ct[0] = pt[0];
    ct[1] = pt[1];
    for(int8_t i = ROUNDS -1 ; i >= 0; i--) {
        uint64_t temp = (ROL(pt[0], 1) & ROL(pt[0], 8)) ^ pt[1] ^ ROL(pt[0], 2);
        pt[1] = pt[0];
        pt[0] = temp ^ *(round_key_ptr + i);
    }
}

int main(void){
    uint64_t buffer[2], hash[2];    
    // Simon 128/128 test-vector
    uint64_t plain_text[2] = { 7812731012696405024U, 7166182912792162917U };
    uint64_t cipher_text[2] = { 7325811964449262524U, 5289507578518502975U };
    uint64_t key[KEY_WORDS] = { 506097522914230528U, 1084818905618843912U };
    printf("Plain text: %" PRIu64 " %" PRIu64 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SIMON ENCRYPTION ==================\n");
    uint64_t key_schedule[ROUNDS];
    simon_expand(key_schedule, key);
    
    simon_encrypt_128(key_schedule, plain_text, buffer);
    printf("==> Cipher text: %" PRIu64 " %" PRIu64 "\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == cipher_text[0]);
    assert(buffer[1] == cipher_text[1]);
    
    printf("================== SIMON DECRYPTION ==================\n");    
    simon_decrypt_128(key_schedule, cipher_text, buffer);
    printf("==> Plain text: %" PRIu64 " %" PRIu64 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == plain_text[0]);
    assert(buffer[1] == plain_text[1]);

    printf("Encryption and decryption success\n");
    return EXIT_SUCCESS;
}
