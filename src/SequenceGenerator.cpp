#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>
#include "SequenceGenerator.hpp"

std::vector<std::string> getSequence(const int &totalLen, const int &subSeqLen)
{
    std::string seq;
    std::vector<std::string> subSequences;
    for (int i = 0; i < totalLen; i++)
    {
        seq += getRandomNucleotide();
    }

    exportSequence(seq);

    for (size_t i = 0; i < seq.length(); i += subSeqLen)
    {
        subSequences.push_back(seq.substr(i, subSeqLen));
    }

    std::sort(subSequences.begin(), subSequences.end());

    return subSequences;
}

char getRandomNucleotide()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const std::string nucleotides = "ACGT";

    std::uniform_int_distribution<> dis(0, nucleotides.size() - 1);
    int randomIndex = dis(gen);

    return nucleotides[randomIndex];
}

void exportSequence(std::string &sequence)
{
    std::ofstream sequenceFileStream;
    sequenceFileStream.open("sequence.txt", std::ios::out);
    sequenceFileStream << sequence;
    sequenceFileStream.close();
}