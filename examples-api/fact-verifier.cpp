#include <iostream>
#include <string>
#include "hyperion-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    string pubtape = "../examples-zmips/factorial/fact.pubtape";
    
    std::cout << "Running Verifier for factorial...\n";

    hyperion_verifier(zmips_asm, pubtape, "", "../framework/hyperion/src/macros.json", 34343, 10, 60, false);

    std::cout << "Executed Verifier for " << zmips_asm << '\n';
    
    return 0;
}
