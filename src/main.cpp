#include <iostream>
#include <string>
#include <vector>

#include "SequenceStructure.hpp"
#include "SequenceGenerator.hpp"
#include "BruteForceAlgorithm.hpp"

int main()
{
    Sequence seq;
    seq.sequenceLen = 100;
    seq.subSequenceLen = 10;
    seq.sequence = getSequence(seq.sequenceLen);
    seq.subSequences = getSubSequences(seq.sequence, seq.subSequenceLen, false, false);
    seq.startingSubSequence = seq.sequence.substr(0, seq.subSequenceLen - 1);

    Graph graph;
    constructGraph(seq.subSequences, graph, seq.subSequenceLen, 7);

    return 0;
}