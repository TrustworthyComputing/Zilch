#include <iostream>
#include <string>
#include "hyperion-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    // string pubtape = "../examples-zmips/factorial/fact.pubtape";
    
    std::cout << "Running Verifier for factorial...\n";
    
    string session("5849b8af");
    bool result = hyperion_verifier(zmips_asm, "", "", "../framework/hyperion/src/macros.json", 34343, session, 10, 60, false);

    std::cout << "Executed Verifier for " << zmips_asm << '\n';
    std::cout << "Verifier's result " << result << '\n';
    
    return 0;
}
