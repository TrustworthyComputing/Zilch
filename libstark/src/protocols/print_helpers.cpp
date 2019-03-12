#include "print_helpers.hpp"

namespace Colors{
    
    void startColor(const char* color){
        std::cout<<color;
    }
    
    void resetColor(){
        startColor(RESET);
    }

    void successColor(){
        startColor(GREEN);
    }
    
    void decisionColor(){
        startColor(YELLOW);
    }
    
    void sentDecisionColor(){
        startColor(MAGENTA);
    }
    
    void failureColor(){
        startColor(RED);
    }
    
    void startSpecs(){
        startColor(CYAN);
    }

    std::string numBytesToString(size_t numBytes){
        std::string suffix[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EByte", "ZByte"};
        
        int i=0;
        double currSize = numBytes;
        while(currSize > 1024LL){
            currSize /= 1024LL;
            i++;
        }
        
        return std::to_string(currSize) + " " + suffix[i];
    }
    
    std::string secondsToString(double seconds){
        if(seconds >= 60LL*60LL*24LL){
            return std::to_string(seconds/(60LL*60LL*24LL)) + " Days";
        }
        
        if(seconds >= 60LL*60LL){
            return std::to_string(seconds/(60LL*60LL)) + " Hours";
        }
        
        if(seconds >= 60LL){
            return std::to_string(seconds/(60LL)) + " Minutes";
        }
        
        return std::to_string(seconds) + " Seconds";
    }
    
    void printSpecs(const double proverTime, const double verifierTime, const size_t proofGeneratedBytes, const size_t proofSentBytes, const size_t queriedDataBytes){
        startSpecs();
        libstark::specsPrinter specs("Protocol execution measurements");
        specs.addLine("Prover time",secondsToString(proverTime));
        specs.addLine("Verifier time",secondsToString(verifierTime));
        specs.addLine("Total proof oracles size",numBytesToString(proofGeneratedBytes));
        specs.addLine("Total communication complexity",numBytesToString(proofSentBytes));
        specs.addLine("Query complexity",numBytesToString(queriedDataBytes));
        specs.print();
        
        resetColor();
    }
    
    void printProverSpecs(const double proverTime, const double communicationTime){
        startSpecs();
        libstark::specsPrinter specs("Protocol execution measurements");
        specs.addLine("Prover time",secondsToString(proverTime));
        specs.addLine("Prover Communication time",secondsToString(communicationTime));
        specs.print();
        resetColor();
    }
    
    void printVerifierSpecs(const double verifierTime, const double communicationTime, const size_t proofGeneratedBytes, const size_t proofSentBytes, const size_t queriedDataBytes){
        startSpecs();
        libstark::specsPrinter specs("Protocol execution measurements");
        specs.addLine("Verifier time",secondsToString(verifierTime));
        specs.addLine("Verifier Communication time",secondsToString(communicationTime));
        specs.addLine("Total proof oracles size",numBytesToString(proofGeneratedBytes));
        specs.addLine("Total communication complexity",numBytesToString(proofSentBytes));
        specs.addLine("Query complexity",numBytesToString(queriedDataBytes));
        specs.print();
        resetColor();
    }
    
    void printSpecsCSV(const double proverTime, const double verifierTime, const size_t proofGeneratedBytes, const size_t proofSentBytes, const size_t queriedDataBytes){
        return;
        startSpecs();
        std::cout<<"Comma Separated Valued (CSV) specifications:"<<std::endl;
        std::cout<<"Prover time (seconds), Verifier time (seconds), Proof size (Bytes), Proof sent (Bytes), Queried data (Bytes)"<<std::endl;
        std::cout<<proverTime<<","<<verifierTime<<","<<proofGeneratedBytes<<","<<proofSentBytes<<","<<queriedDataBytes<<std::endl;
        
        resetColor();
    }
    
}
