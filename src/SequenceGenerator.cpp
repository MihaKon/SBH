#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include "SequenceGenerator.hpp"

std::string getSequence(int len)
{
    const std::string nucleotides = "ACGT";

    std::string sequence;

    std::random_device rd;
    std::mt19937 gen(rd());


    std::uniform_int_distribution<> dis(0, nucleotides.size() - 1);

    for (int i = 0; i < len; i++)
    {
        sequence += nucleotides[dis(gen)];
    }

    return sequence;
}

std::vector<std::string> getSubSequences(std::string &sequence, const int &subSeqLen, const int& pePercent, const int& nePercent)
{
    std::vector<std::string> subSequences;
    for (size_t i = 0; i < sequence.length() - subSeqLen + 1; i++){
        subSequences.push_back(sequence.substr(i, subSeqLen));
    }

    if (pePercent!=0){
        for (int peC = 0; peC < float(pePercent)/sequence.length(); peC++){
            subSequences.push_back(getSequence(subSeqLen));
         }
    }
     if (nePercent != 0) {
        int numElementsToRemove = static_cast<int>(float(nePercent) / 100.0 * subSequences.size());
        numElementsToRemove = std::min(numElementsToRemove, static_cast<int>(subSequences.size()));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(subSequences.begin(), subSequences.end(), gen);
        subSequences.resize(subSequences.size() - numElementsToRemove);
    }

    return subSequences;
}
