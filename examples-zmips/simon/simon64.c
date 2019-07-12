#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define ROUNDS 44
#define KEY_WORDS 4
#define WORD_SIZE 32

#define ROL(x, r) ((x << r) | (x >> (WORD_SIZE - r)))
#define ROR(x, r) ((x >> r) | ((x & 7) << (WORD_SIZE - r)))

uint64_t z_arrays[5] = {0b0001100111000011010100100010111110110011100001101010010001011111,
                        0b0001011010000110010011111011100010101101000011001001111101110001,
                        0b0011001101101001111110001000010100011001001011000000111011110101,
                        0b0011110000101100111001010001001000000111101001100011010111011011,
                        0b0011110111001001010011000011101000000100011011010110011110001011};

void simon_expand(uint32_t *key_schedule, uint32_t *sub_keys) {
    uint32_t mod_mask = ULLONG_MAX >> (64 - WORD_SIZE);
    uint64_t tmp1,tmp2;
    uint64_t c = 0xFFFFFFFFFFFFFFFC; 
    
    memcpy(key_schedule, &sub_keys[0], KEY_WORDS);
    for (int i = 0; i < ROUNDS - 1; i++) {
        tmp1 = ROR(sub_keys[3], 3);
        tmp1 ^= sub_keys[1];
        tmp2 = ROR(tmp1, 1);
        tmp1 ^= sub_keys[0];
        tmp1 ^= tmp2;
        tmp2 = c ^ ((z_arrays[3] >> (i % 62)) & 1);
        tmp1 ^= tmp2;
        for (int j = 0; j < 3; j++) {
            sub_keys[j] = sub_keys[j + 1];
        }
        sub_keys[3] = tmp1 & mod_mask;
        memcpy(&key_schedule[i+1], &sub_keys[0], KEY_WORDS);
    }
}

void simon_encrypt_64(const uint32_t *round_key_ptr, uint32_t *pt, uint32_t *ct) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    for (size_t i = 0; i < ROUNDS; i++) {
        uint32_t temp = ROL(ct[1], 1) & ROL(ct[1], 8) ^ ct[0] ^ ROL(ct[1], 2);
        ct[0] = ct[1];
        ct[1] = temp ^ round_key_ptr[i];
    }
}

void simon_decrypt_64(const uint32_t *round_key_ptr, uint32_t *ct, uint32_t *pt){
    ct[0] = pt[0];
    ct[1] = pt[1];
    for(int8_t i = ROUNDS -1 ; i >= 0; i--) {
        uint32_t temp = (ROL(pt[0], 1) & ROL(pt[0], 8)) ^ pt[1] ^ ROL(pt[0], 2);
        pt[1] = pt[0];
        pt[0] = temp ^ *(round_key_ptr + i);
    }
}

void simon_64_128_hash(uint32_t hash[2], const uint32_t pt[2], const uint32_t ct[2]) {
    hash[0] = pt[0] ^ ct[0];
    hash[1] = pt[1] ^ ct[1];
}

int main(void){
    uint32_t buffer[2], hash[2];    
    uint32_t plain_text[2] = { 543452789, 1701538156 };     // 656b696c 20646e75
    uint32_t cipher_text[2] = { 3118440570, 1154022432 };   // 44c8fc20 b9dfa07a
    const uint32_t hash_result[2] = { 2579222031, 564368716 };
    uint32_t key[KEY_WORDS] = { 50462976, 185207048, 319951120, 454695192 }; // test-vector 1b1a1918 13121110 0b0a0908 03020100
    printf("Plain text: %" PRIu32 " %" PRIu32 "\n\n", plain_text[0], plain_text[1]);
    printf("================== SIMON ENCRYPTION ==================\n");
    uint32_t key_schedule[ROUNDS] = { 50462976, 185207048, 319951120, 454695192, 1889538499, 3077631049, 1474553909, 3549936706, 2497509407, 3209387800, 2388503481, 3690244195, 3440126204, 1555470609, 2045842085, 1998608483, 2575830034, 485063768, 3370983749, 3087063992, 3899270998, 2080822493, 2874334986, 612329128, 1398308006, 3529249777, 1166259787, 2107414029, 4022321907, 2314044691, 814611534, 3206162998, 3779698032, 1290064639, 850914287, 3296003521, 3504941544, 2414118073, 1107643374, 2943859426, 404331522, 1906196252, 203224979, 366954134 };
    // uint32_t key_schedule[ROUNDS];
    // simon_expand(key_schedule, key);

    simon_encrypt_64(key_schedule, plain_text, buffer);
    printf("==> Cipher text: %" PRIu32 " %" PRIu32 "\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == 3118440570);
    assert(buffer[1] == 1154022432);
    
    printf("================== SPECK HASH ========================\n");
    simon_64_128_hash(hash, plain_text, buffer);
    assert(hash[0] == hash_result[0]);
    assert(hash[1] == hash_result[1]);
    printf("==> Speck hash: %" PRIu32 " %" PRIu32 "\n\n", hash[0], hash[1]);
    
    printf("================== SIMON DECRYPTION ==================\n");    
    simon_decrypt_64(key_schedule, cipher_text, buffer);
    printf("==> Plain text: %" PRIu32 " %" PRIu32 " (decrypted cipher text)\n\n", buffer[0], buffer[1]);
    assert(buffer[0] == 543452789);
    assert(buffer[1] == 1701538156);

    printf("Encryption and decryption success\n");
    return EXIT_SUCCESS;
}
