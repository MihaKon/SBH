#include <string>
#include <vector>
#include <random>
#include <algorithm>

#include "SequenceGenerator.hpp"

std::string getSequence(const int &len)
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

std::vector<std::string> getSubSequences(std::string &sequence, const int &subSeqLen, bool addPositiveErrors, bool addNegativeErrors)
{
    std::vector<std::string> subSequences;

    for (size_t i = 0; i < sequence.length() - subSeqLen + 1; i++)
    {
        subSequences.push_back(sequence.substr(i, subSeqLen));
    }

    if (addPositiveErrors)
    {
        subSequences.push_back(getSequence(subSeqLen));
    }

    if (addNegativeErrors)
    {
        int randomIndex = rand() % sequence.size();
        subSequences.erase(subSequences.begin() + randomIndex);
    }

    std::sort(subSequences.begin(), subSequences.end());

    return subSequences;
}