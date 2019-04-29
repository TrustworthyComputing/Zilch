#include <iostream>
#include <string>
#include "hyperion-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    string pubtape = "../examples-zmips/factorial/fact.pubtape";
    
    std::cout << "Running fact...\n";

    int ans = hyperion_local_prover_verifier(zmips_asm, pubtape, "", 60, false, true);
    
    std::cout << "Executed " << zmips_asm << " and the answer is " << ans << '\n';
    
    return 0;
}
