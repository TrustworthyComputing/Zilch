#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <inttypes.h>
#include "hyperion-api.hpp"

#define KEY_LEN 4
#define ROUNDS 22

#define ROR(x, r) ((x >> r) | (x << ((sizeof(uint16_t) * 8) - r)))
#define ROL(x, r) ((x << r) | (x >> ((sizeof(uint16_t) * 8) - r)))
#define R(x, y, k) (x = ROR(x, 7), x += y, x ^= k, y = ROL(y, 2), y ^= x)
#define RR(x, y, k) (y ^= x, y = ROR(y, 2), x ^= k, x -= y, x = ROL(x, 7))

using namespace std;

// Example:
// A 100
// B 200
// C 150
// --> Winner B, pays 150
// A 100 : key = {1, 2, 3, 4}; hash = {42890, 17020}; exp = {1, 1029, 6672, 19026, 25872, 63841, 63527, 18982, 41886, 28075, 22436, 14369, 15207, 41657, 35181, 29852, 27972, 13898, 21461, 61460, 26458, 65298} 
// B 200: key = {2, 3, 4, 5}; hash = {12832, 28134}; exp = {2, 1546, 5667, 30893, 40463, 52241, 15891, 19270, 34736, 12550, 33405, 56366, 5181, 36401, 1815, 4018, 42408, 56981, 49402, 33225, 63340, 36678} 
// C 150: key = {3, 4, 5, 6}; hash = {21252, 58351}; exp = {3, 2063, 12850, 63224, 9960, 55464, 23426, 7539, 22578, 64101, 1749, 6254, 4112, 54690, 7982, 3239, 58177, 41155, 23740, 27324, 23029, 55413} 

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

void speck32_encrypt(const uint16_t pt[2], uint16_t ct[2], const uint16_t K[ROUNDS]) {
    ct[0] = pt[0];
    ct[1] = pt[1];
    for (int i = 0; i < ROUNDS; i++) {
        R(ct[1], ct[0], K[i]);
    }
}

void speck32_hash(uint16_t hash[2], const uint16_t pt[2], const uint16_t ct[2]) {
    hash[0] = pt[0] ^ ct[0];
    hash[1] = pt[1] ^ ct[1];
}

int main(int argc, char const *argv[]) {
    ofstream privtape_file("vickrey.privtape");
    
    size_t participants = 3;
    std::cout << "Vickrey Auction for " << participants << " participants\n";

    uint16_t auctions[participants];
    uint16_t commits[participants][2];
    for (size_t i = 0; i < participants; i++) {
        std::cout << "Enter bid for participant " << i << ": ";
        cin >> auctions[i];
        uint16_t key[4];
        uint16_t exp[ROUNDS];
        uint16_t temp[2] = { 0 };
        uint16_t buf[2] = {0, auctions[i]};
        std::cout << "Enter secret key to generate commitment:\n";
        for (size_t j = 0; j < 4; j++) {
            std::cout << "\t(uint16_t) key[" << j << "] : ";
            cin >> key[j];
        }
        speck_expand(key, exp);
        speck32_encrypt(buf, temp, exp);
        speck32_hash(commits[i], buf, temp);
        cout << "==> Speck hash: " << commits[i][0] << " " << commits[i][1] << "\n\n";
        privtape_file << auctions[i] << '\n';
    }
    privtape_file.close();
    std::cout << "Sending bids and commits to the server...\n\n";

    std::cout << "Public commits:\n";
    for (size_t i = 0; i < participants; i++) {
        cout << i << ") " << commits[i][0] << " " << commits[i][1] << "\n";
    }
    std::cout << "\n";

    std::cout << "Running Vickrey Auction in Hyperion...\n";

    int ans = hyperion_local_prover_verifier("./vickrey.zmips", "", "./vickrey.privtape", "../framework/hyperion/src/macros.json", 60, false, false);
    
    for (size_t i = 0; i < participants; i++) {
        if (auctions[i] > ans) {
            std::cout << "Executed vickrey.zmips and the winner is bidder #" << i << "!\n\n";
            
            uint16_t key[4];
            uint16_t exp[ROUNDS];
            std::cout << "Participant " << i << " enter secret key:\n";
            for (size_t j = 0; j < 4; j++) {
                std::cout << "\t(uint16_t) key[" << j << "] : ";
                cin >> key[j];
            }
            speck_expand(key, exp);
            ofstream privtape_key_file("speck-key.privtape");
            privtape_key_file << 0 << '\n';
            privtape_key_file << auctions[i] << '\n';
            for (size_t j = 0; j < ROUNDS; j++) {
                privtape_key_file << exp[j] << '\n';
            }
            privtape_key_file.close();
            
            std::cout << "Running Speck32 Davies-Meyer to verify the commitment in Hyperion...\n";

            int ans = hyperion_local_prover_verifier("../examples-zmips/speck_DM_hash/speck_DM_hash.zmips", "", "speck-key.privtape", "../framework/hyperion/src/macros.json", 60, false, false);
            if (ans == commits[i][0]) {
                std::cout << "Commitment verified!\n";
                return 0;
            } else {
                std::cout << "Commitment " << commits[i][0] << " does not match with " << ans << "!\n";
            }
        }
    }
    
    return 1;
}
