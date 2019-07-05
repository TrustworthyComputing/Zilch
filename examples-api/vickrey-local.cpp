#include <iostream>
#include <fstream>
#include <string>
#include "hyperion-api.hpp"

using namespace std;

// Example:
// A 100
// B 95
// C 125
// --> Winner C, pays 100

int main(int argc, char const *argv[]) {
    string zmips_asm = "vickrey.zmips";
    string privtape = "vickrey.privtape";
    ofstream privtape_file(privtape);
    
    size_t participants = 5;
    std::cout << "Vickrey Auction for " << participants << "participants\n";
    // std::cout << "Enter the number of participants: ";
    // cin >> participants;
    int auctions[participants];
    for (size_t i = 0; i < participants; i++) {
        std::cout << "Enter bid for participant " << i << ": ";
        cin >> auctions[i];
        privtape_file << auctions[i] << '\n';
    }
    privtape_file.close();

    std::cout << "Running Vickrey Auction in Hyperion...\n";

    int ans = hyperion_local_prover_verifier(zmips_asm, "", privtape, 60, false, true);
    
    for (size_t i = 0; i < participants; i++) {
        if (auctions[i] == ans) {
            std::cout << "Executed " << zmips_asm << " and the winner is bidder #" << i << "!\n";
            return 0;
        }
    }
    
    return 1;
}
