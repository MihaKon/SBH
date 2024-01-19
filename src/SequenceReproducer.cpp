#include "SequenceReproducer.hpp"

std::string reproduceSequence(const std::vector<std::string>& kmers, const int& subSeqLen) {
    std::string sequence = kmers[0];
    int matchingLength = 0;
    for (int i = 2; i <= kmers.size(); i++) {
        matchingLength = 0;
        std::string iLabel = "";
        std::string jLabel = "";
        for (int k = 1; k <= subSeqLen; k++) {
            iLabel = kmers[i - 2][subSeqLen - k] + iLabel;
            jLabel += kmers[i - 1][k - 1];
            if (iLabel == jLabel){
                matchingLength=k;
            }
        }
        std::string kLabel = "";
        kLabel+=kmers[i-1].substr(matchingLength, subSeqLen-matchingLength);
        sequence += kLabel;
    }
    
    return sequence;
}


