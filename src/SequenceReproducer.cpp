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
       //     std::cout << iLabel << ":" << jLabel << std::endl;
            if (iLabel == jLabel){
                matchingLength=k;
            }
        }
        std::string kLabel = "";
       // std::cout << matchingLength << std::endl;
        kLabel+=kmers[i-1].substr(matchingLength, subSeqLen-matchingLength);
        sequence += kLabel;
    }
  //  std::cout << std::endl;

    //sequence+=kmers[kmers.size()-1].substr(subSeqLen-matchingLength+1, subSeqLen-1);
    return sequence;
}


