#pragma once

#include <vector>
#include <string>

struct Sequence {
    int sequenceLen;
    int subSequenceLen;
    std::string sequence;
    std::vector<std::string> subSequences;
};