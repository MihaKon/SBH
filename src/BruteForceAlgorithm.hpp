#pragma once

#include <string>
#include <vector>


std::string getOriginalSequence(
    int seqLen,
    int subSeqLen,
    const std::vector<std::string> &subSequences,
    const std::string &startingSubSeq,
    bool positiveErrors,
    bool negativeErrors
);