#pragma once

#include <vector>
#include <string>

struct Sequence
{
    int sequenceLen;
    int subSequenceLen;
    int positiveErrorPercentage;
    int negativeErrorPercentage;
    std::string sequence;
    std::string startingSubSequence;
    std::vector<std::string> subSequences;

    Sequence(int len, int subLen, int pePercent, int nePercent)
        : sequenceLen(len), subSequenceLen(subLen),
          positiveErrorPercentage(pePercent), negativeErrorPercentage(nePercent) {}
};