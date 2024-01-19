#include <iostream>
#include <string>
#include <fstream>

#include "./SequenceStructure.hpp"
#include "./SequenceGenerator.hpp"
#include "./BruteForceAlgorithm.hpp"
#include "./SequenceReproducer.hpp"
#include "./GreedyHeuristicAlgorithm.hpp"
#include "./GreedyHeuristicAlgorithm.cpp"
#include "./LevenshteinDistanceCalc.hpp"

const int INSTANCES = 10;
const int STARTING_SEQ_LEN = 500;
const int MAX_SEQ_LEN = 1000;
const int STARTING_SUB_SEQ_LEN = 7;
const int MAX_SUB_SEQ_LEN = 10;
const int POSITIVE_ERROR_PERCENTAGE = 10;
const int NEGATIVE_ERROR_PERCENTAGE = 10;

int main()
{
    std::cout << "Starting..." << std::endl;

    std::fstream result("result.txt", std::ios::out);
    result << "\"ALGORITHM\",\"SEQ_LENGTH\",\"SUB_SEQ_LENGTH\",\"POSITIVE_ERROR_PERCENTAGE\",\"NEGATIVE_ERROR_PERCENTAGE\",\"AVG. LEVENSHTEIN_DISTANCE PER "<< INSTANCES << " INSTANCES\",\"AVG. LEVENSHTEIN_PERCENTAGE PER "<< INSTANCES <<" INSTANCES\"" << std::endl;

    std::fstream instance("instance.txt", std::ios::out);
    for (int n = STARTING_SEQ_LEN; n < MAX_SEQ_LEN; n += 50)
    {
        instance << "SEQ_LENGTH: " << n << std::endl;
        for (int k = STARTING_SUB_SEQ_LEN; k <= MAX_SUB_SEQ_LEN; k++)
        {
            instance << "SUB_SEQ_LENGTH: " << k << std::endl;
            
            int totalBruteForceLevenshteinDistance = 0;
            int totalGreedyHeuristicLevenshteinDistance = 0;

            for (int i = 1; i <= INSTANCES; i++)
            {
                std::cout << n << " " << k <<" Instance " << i << " of " << INSTANCES << std::endl;
                Sequence sequenceStructure(n, k, POSITIVE_ERROR_PERCENTAGE, NEGATIVE_ERROR_PERCENTAGE);
                sequenceStructure.sequence = getSequence(sequenceStructure.sequenceLen);
                sequenceStructure.startingSubSequence = sequenceStructure.startingSubSequence.substr(0, sequenceStructure.subSequenceLen);
                sequenceStructure.subSequences = getSubSequences(
                    sequenceStructure.sequence,
                    sequenceStructure.subSequenceLen,
                    sequenceStructure.positiveErrorPercentage,
                    sequenceStructure.negativeErrorPercentage,
                    &instance
                );
                
                std::string bruteForceResult = getOriginalSequence(
                    sequenceStructure.sequenceLen,
                    sequenceStructure.subSequenceLen,
                    sequenceStructure.subSequences,
                    sequenceStructure.startingSubSequence
                );

                totalBruteForceLevenshteinDistance += getLevenshteinDistance(bruteForceResult, sequenceStructure.sequence);

                Graph G;
                G.constructGraph(sequenceStructure.subSequences, sequenceStructure.subSequenceLen);
                std::vector<std::string> kmers = SBH(G, sequenceStructure.sequenceLen, sequenceStructure.subSequenceLen, sequenceStructure.startingSubSequence);
                std::string greedyHeuristicResult = reproduceSequence(sequenceStructure.subSequences, sequenceStructure.subSequenceLen);

                totalGreedyHeuristicLevenshteinDistance += getLevenshteinDistance(greedyHeuristicResult, sequenceStructure.sequence);
            }
            result << "\"BruteForce\",\"" << n << "\",\"" << k << "\",\"" << POSITIVE_ERROR_PERCENTAGE << "\",\""<<NEGATIVE_ERROR_PERCENTAGE<<"\",\"" << totalBruteForceLevenshteinDistance / INSTANCES << "\",\"" << (totalBruteForceLevenshteinDistance / INSTANCES) / n << "\"" << std::endl;
            result << "\"GreedyHeuristic\",\"" << n << "\",\"" << k << "\",\"" << POSITIVE_ERROR_PERCENTAGE << "\",\""<<NEGATIVE_ERROR_PERCENTAGE<<"\",\"" << totalGreedyHeuristicLevenshteinDistance / INSTANCES << "\",\"" << (totalGreedyHeuristicLevenshteinDistance / INSTANCES) / n << "\"" << std::endl;
        }
    }

    result.close();
    instance.close();

    std::cout << "Finished!" << std::endl;
    return 0;
}
