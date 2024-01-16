#include <iostream>
#include <string>

#include "./SequenceGenerator.hpp"
#include "./SequenceGenerator.cpp"
#include "./LevenshteinDistanceCalc.hpp"
#include "./LevenshteinDistanceCalc.Cpp"
#include "./GreedyHeuristicAlgorithm.hpp"
#include "./GreedyHeuristicAlgorithm.cpp"


int main(){
    int length = 300;
    std::vector<std::string> sequences;
    std::string sequence = getSequence(length);
    sequences.emplace_back(sequence);
    int subSequencesLength = 7;
    std::vector<std::string> spectrum = getSubSequences(sequences[0], subSequencesLength, 0, 0);
    std::cout << std::endl << sequences[0] << std::endl;
    Graph G;
    G.constructGraph(spectrum, subSequencesLength);

    std::string initialOligo = sequences[0].substr(0,subSequencesLength);

    std::vector<std::string> kmers = SBH(G, length, subSequencesLength, initialOligo);
    std::string createdSequence = reproduceSequence(kmers, subSequencesLength);
    std::cout << createdSequence << std::endl;
    std::cout << float(getLevenshteinDistance(sequences[0], createdSequence)) / length;
    return 0;
}
