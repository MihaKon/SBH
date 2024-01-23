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

const int INSTANCES = 10;
const int STARTING_SEQ_LEN = 700;
const int STARTING_SUB_SEQ_LEN = 7;
const int STARTING_ERROR_PERCENTAGE = 0;
const int MAX_SEQ_LEN = 1000;
const int MAX_SUB_SEQ_LEN = 10;
const int MAX_ERROR_PERCENTAGE = 15;

int main(int argc, char *argv[])
{

    std::fstream result("result.txt", std::ios::out);
    result << "\"ALGORITHM\",\"SEQ_LENGTH\",\"SUB_SEQ_LENGTH\",\"POSITIVE_ERROR_PERCENTAGE\",\"NEGATIVE_ERROR_PERCENTAGE\",\"AVG. LEVENSHTEIN_DISTANCE PER " << INSTANCES << " INSTANCES\",\"AVG. LEVENSHTEIN_PERCENTAGE PER " << INSTANCES << " INSTANCES\"" << std::endl;

    std::fstream instance("instance.txt", std::ios::out);

    int totalBruteForceLevenshteinDistance = 0;
    int totalGreedyHeuristicLevenshteinDistance = 0;
    double totalBruteForceLevenshteinPercentage = 0;
    double totalGreedyHeuristicLevenshteinPercentage = 0;

    for (int e = STARTING_ERROR_PERCENTAGE; e <= MAX_ERROR_PERCENTAGE; e += 5)
    {
        for (int n = STARTING_SEQ_LEN; n <= MAX_SEQ_LEN; n += 100)
        {
            for (int k = STARTING_SUB_SEQ_LEN; k <= MAX_SUB_SEQ_LEN; k++)
            {
                for (int i = 1; i <= INSTANCES; i++)
                {
                    instance << "SEQ_LEN: " << n << std::endl;
                    instance << "SUB_SEQ_LENGTH: " << k << std::endl;
                    instance << "POSITIVE_ERROR_PERCENTAGE: " << e << std::endl;
                    instance << "NEGATIVE_ERROR_PERCENTAGE: " << e << std::endl;
                    Sequence sequenceStructure(n, k, e, e);
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

                    instance << "BRUTE_FORCE_RESULT: " << bruteForceResult << std::endl;

                    int tempBruteLeven = getLevenshteinDistance(bruteForceResult, sequenceStructure.sequence);
                    totalBruteForceLevenshteinDistance += tempBruteLeven;
                    totalBruteForceLevenshteinPercentage += levenshteinPercentage(tempBruteLeven, std::max(bruteForceResult.length(), sequenceStructure.sequence.length()));

                    Graph G;
                    G.constructGraph(sequenceStructure.subSequences, sequenceStructure.subSequenceLen);
                    std::vector<std::string> kmers = SBH(G, sequenceStructure.sequenceLen, sequenceStructure.subSequenceLen, sequenceStructure.startingSubSequence);
                    std::string greedyHeuristicResult = reproduceSequence(kmers, sequenceStructure.subSequenceLen);

                    instance << "GREEDY_HEURISTIC_RESULT: " << greedyHeuristicResult << std::endl;

                    int tempGreedyLeven = getLevenshteinDistance(greedyHeuristicResult, sequenceStructure.sequence);
                    totalGreedyHeuristicLevenshteinDistance += tempGreedyLeven;
                    totalGreedyHeuristicLevenshteinPercentage += levenshteinPercentage(tempGreedyLeven, std::max(greedyHeuristicResult.length(), sequenceStructure.sequence.length()));

                    instance << std::endl;
                }

                float avgBruteForceLevenshteinDistance = (float)totalBruteForceLevenshteinDistance / INSTANCES;
                float avgGreedyHeuristicLevenshteinDistance = (float)totalGreedyHeuristicLevenshteinDistance / INSTANCES;

                float avgBruteForceLevenshteinPercentage = (float)totalBruteForceLevenshteinPercentage / INSTANCES;
                float avgGreedyHeuristicLevenshteinPercentage = (float)totalGreedyHeuristicLevenshteinPercentage / INSTANCES;

                result << "\"BruteForce\",\"" << n << "\",\"" << k << "\",\"" << e << "\",\"" << e << "\",\"" << avgBruteForceLevenshteinDistance << "\",\"" << avgBruteForceLevenshteinPercentage << "\"" << std::endl;
                result << "\"GreedyHeuristic\",\"" << n << "\",\"" << k << "\",\"" << e << "\",\"" << e << "\",\"" << avgGreedyHeuristicLevenshteinDistance << "\",\"" << avgGreedyHeuristicLevenshteinPercentage << "\"" << std::endl;
            }
        }
    }
    result.close();
    instance.close();
    return 0;
}
