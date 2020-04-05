#include <iostream>
#include <string>
#include "zilch-api.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    string zmips_asm = "../examples-zmips/factorial/fact.zmips";
    // string pubtape = "../examples-zmips/factorial/fact.pubtape";
    
    std::cout << "Running Prover for factorial...\n";

    string session("5849b8af");
    int ans = zilch_prover(zmips_asm, "", "", "../framework/zilch/src/macros.json", "localhost", 34343, session, 10, 60, false);
    
    std::cout << "Executed " << zmips_asm << " and the answer is " << ans << '\n';
    
    return 0;
}
