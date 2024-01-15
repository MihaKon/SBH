#include "./BruteForceAlgorithm.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>


int getWeight(const std::string &str1, const std::string &str2, int subSeqLen)
{
    std::string subStr1 = str1.substr(1, subSeqLen);
    std::string subStr2 = str2.substr(0, subSeqLen - 1);

    int commonElements = 0;

    for (int i = 0; i < subStr1.length(); i++)
    {
        if (subStr2[i] == subStr1[i])
        {
            commonElements++;
        }
    }
    return commonElements;
}

void constructGraph(const std::vector<std::string> &subSequences, Graph &graph, int subSeqLen, int minWeightThreshold)
{
    for (int i = 0; i < subSequences.size(); i++)
    {
        for (int j = 0; j < subSequences.size(); j++)
        {
            int weight = getWeight(subSequences[i], subSequences[j], subSeqLen);
            if (weight >= minWeightThreshold)
            {
                graph.adjList[subSequences[i]].insert(adjNode(subSequences[j], weight));
            }
        }
    }
}

std::string getOriginalSequence(
    int seqLen,
    int subSeqLen,
    const std::vector<std::string> &subSequences,
    const std::string &startingSubSeq,
    bool positiveErrors,
    bool negativeErrors)
{
    return "Test";
}