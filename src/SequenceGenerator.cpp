#include <random>
#include <algorithm>

#include "SequenceGenerator.hpp"

std::string getSequence(int len, std::fstream *file)
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

    if (file != nullptr && file->is_open())
    {
        *file << "SEQ: " << sequence << std::endl;
    }
    return sequence;
}

std::vector<std::string> getSubSequences(std::string &sequence, const int subSeqLen, const int pePercent, const int nePercent, std::fstream *file)
{
    std::vector<std::string> subSequences;
    for (size_t i = 0; i < sequence.length() - subSeqLen + 1; i++)
    {
        subSequences.push_back(sequence.substr(i, subSeqLen));
    }

    if (pePercent != 0)
    {
        for (int peC = 0; peC < float(pePercent) / sequence.length(); peC++)
        {
            std::string subSeq = getSequence(subSeqLen);
            subSequences.push_back(subSeq);
            if(file->is_open()) {
                *file << "PE: " << subSeq << std::endl;
            }
        }
    }
    if (nePercent != 0)
    {
        int numElementsToRemove = static_cast<int>(float(nePercent) / 100.0 * subSequences.size());
        numElementsToRemove = std::min(numElementsToRemove, static_cast<int>(subSequences.size()));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(subSequences.begin(), subSequences.end(), gen);
        subSequences.resize(subSequences.size() - numElementsToRemove);
    }

    std::sort(subSequences.begin(), subSequences.end());

    if (file->is_open())
    {
        *file << "SUBSEQ: ";
        for (const auto &subSeq : subSequences)
        {
            *file << subSeq << " ";
        }
        *file << std::endl;
    }

    return subSequences;
}
