#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "./SequenceStructure.hpp"
#include "./SequenceGenerator.hpp"
#include "./BruteForceAlgorithm.hpp"
#include "./SequenceReproducer.hpp"
#include "./GreedyHeuristicAlgorithm.hpp"
#include "./GreedyHeuristicAlgorithm.cpp"
#include "./LevenshteinDistanceCalc.hpp"

int main(int argc, char *argv[])
{
    // if (argc != 6)
    // {
    //     std::cout << "Wrong number of arguments! " << argc << std::endl;
    //     return 1;
    // }

    // int instances = atoi(argv[1]);
    // int SeqLen = atoi(argv[2]);
    // int SubSeqLen = atoi(argv[3]);
    // int positiveErrorPercentage = atoi(argv[4]);
    // int negativeErrorPercentage = atoi(argv[5]);

    int instances = 5;
    int SeqLen = 700;
    int SubSeqLen = 8;
    int positiveErrorPercentage = 0;
    int negativeErrorPercentage = 20;

    std::fstream result("result.txt", std::ios::out);
    result << "\"ALGORITHM\",\"SEQ_LENGTH\",\"SUB_SEQ_LENGTH\",\"POSITIVE_ERROR_PERCENTAGE\",\"NEGATIVE_ERROR_PERCENTAGE\",\"AVG. LEVENSHTEIN_DISTANCE PER " << instances << " INSTANCES\",\"AVG. LEVENSHTEIN_PERCENTAGE PER " << instances << " INSTANCES\"" << std::endl;

    std::fstream instance("instance.txt", std::ios::out);


    int totalBruteForceLevenshteinDistance = 0;
    int totalGreedyHeuristicLevenshteinDistance = 0;
    double totalBruteForceLevenshteinPercentage = 0;
    double totalGreedyHeuristicLevenshteinPercentage = 0;

    for (int i = 1; i <= instances; i++)
    {
        instance << "SUB_SEQ_LENGTH: " << SubSeqLen << std::endl;
        Sequence sequenceStructure(SeqLen, SubSeqLen, positiveErrorPercentage, negativeErrorPercentage);
        sequenceStructure.sequence = getSequence(sequenceStructure.sequenceLen, &instance);
        sequenceStructure.startingSubSequence = sequenceStructure.sequence.substr(0, sequenceStructure.subSequenceLen);
        sequenceStructure.subSequences = getSubSequences(
            sequenceStructure.sequence,
            sequenceStructure.subSequenceLen,
            sequenceStructure.positiveErrorPercentage,
            sequenceStructure.negativeErrorPercentage,
            &instance);

        std::string bruteForceResult = getOriginalSequence(
            sequenceStructure.sequenceLen,
            sequenceStructure.subSequenceLen,
            sequenceStructure.subSequences,
            sequenceStructure.startingSubSequence);

        int tempBruteLeven = getLevenshteinDistance(bruteForceResult, sequenceStructure.sequence);
        totalBruteForceLevenshteinDistance += tempBruteLeven;
        totalBruteForceLevenshteinPercentage += levenshteinPercentage(tempBruteLeven, std::max(bruteForceResult.length(), sequenceStructure.sequence.length()));

        Graph G;
        G.constructGraph(sequenceStructure.subSequences, sequenceStructure.subSequenceLen);
        std::vector<std::string> kmers = SBH(G, sequenceStructure.sequenceLen, sequenceStructure.subSequenceLen, sequenceStructure.startingSubSequence);
        std::string greedyHeuristicResult = reproduceSequence(kmers, sequenceStructure.subSequenceLen);

        int tempGreedyLeven = getLevenshteinDistance(greedyHeuristicResult, sequenceStructure.sequence);
        totalGreedyHeuristicLevenshteinDistance += tempGreedyLeven;
        totalGreedyHeuristicLevenshteinPercentage += levenshteinPercentage(tempGreedyLeven, std::max(greedyHeuristicResult.length(), sequenceStructure.sequence.length()));

        instance << std::endl;
    }

    float avgBruteForceLevenshteinDistance = (float)totalBruteForceLevenshteinDistance / instances;
    float avgGreedyHeuristicLevenshteinDistance = (float)totalGreedyHeuristicLevenshteinDistance / instances;
    
    float avgBruteForceLevenshteinPercentage = (float)totalBruteForceLevenshteinPercentage / instances;
    float avgGreedyHeuristicLevenshteinPercentage = (float)totalGreedyHeuristicLevenshteinPercentage / instances;


    result << "\"BruteForce\",\"" << SeqLen << "\",\"" << SubSeqLen << "\",\"" << positiveErrorPercentage << "\",\"" << negativeErrorPercentage << "\",\"" << avgBruteForceLevenshteinDistance<< "\",\"" << avgBruteForceLevenshteinPercentage << "\"" << std::endl;
    result << "\"GreedyHeuristic\",\"" << SeqLen << "\",\"" << SubSeqLen << "\",\"" << positiveErrorPercentage << "\",\"" << negativeErrorPercentage << "\",\"" << avgGreedyHeuristicLevenshteinDistance<< "\",\"" << avgGreedyHeuristicLevenshteinPercentage << "\"" << std::endl;

    result.close();
    instance.close();

    return 0;
}
