#pragma once

#include <vector>
#include <string>

struct Sequence {
    int sequenceLen;
    int subSequenceLen;
    bool positiveError;
    bool negativeError;
    std::string sequence;
    std::string startingSubSequence;
    std::vector<std::string> subSequences;
};