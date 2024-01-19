#include <iostream>
#include <string>
#include <vector>

#include "SequenceStructure.hpp"
#include "SequenceGenerator.hpp"
#include "BruteForceAlgorithm.hpp"
#include "LevenshteinDistanceCalc.hpp"

int main()
{
    Sequence seq;
    seq.positiveError = true;
    seq.negativeError = true;
    seq.sequenceLen = 500;
    seq.subSequenceLen = 7;
    seq.sequence = getSequence(seq.sequenceLen);
    seq.subSequences = getSubSequences(seq.sequence, seq.subSequenceLen, seq.positiveError, seq.negativeError);
    seq.startingSubSequence = seq.sequence.substr(0, seq.subSequenceLen);

    std::string originalSequence = getOriginalSequence(seq.sequenceLen, seq.subSequenceLen, seq.subSequences, seq.startingSubSequence, seq.positiveError, seq.negativeError);

    std::cout << "Sequence: " << seq.sequence << std::endl;
    std::cout << "Sequence: " << originalSequence << std::endl;

    int levenshtein = getLevenshteinDistance(seq.sequence, originalSequence);
    int longer = seq.sequence.length() > originalSequence.length() ? seq.sequence.length() : originalSequence.length();
    int percentage = (longer - levenshtein) / longer;
    
    std::cout << "Levenshtein distance: " << levenshtein << std::endl;

    return 0;
}