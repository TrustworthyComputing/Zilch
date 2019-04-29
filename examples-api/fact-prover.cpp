#include <iostream>
#include <string>
#include "hyperion-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    string pubtape = "../examples-zmips/factorial/fact.pubtape";
    
    std::cout << "Running Prover for factorial...\n";

    int ans = hyperion_prover(zmips_asm, pubtape, "", "localhost", 34343, 10, 60, false);
    
    std::cout << "Executed " << zmips_asm << " and the answer is " << ans << '\n';
    
    return 0;
}
