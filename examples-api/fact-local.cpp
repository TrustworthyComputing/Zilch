#include <iostream>
#include <string>
#include "zilch-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    
    std::cout << "Running fact...\n";

    int ans = zilch_local_prover_verifier(zmips_asm, "", "", "../framework/zilch/src/macros.json", 60, false, true);
    
    std::cout << "Executed " << zmips_asm << " and the answer is " << ans << '\n';
    
    return 0;
}
