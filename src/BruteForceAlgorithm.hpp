#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

struct adjNode
{
    std::string val;
    int weight;

    adjNode(std::string v, int w) : val(v), weight(w) {}

    friend bool operator<(const adjNode& a, const adjNode& b) {
        return a.weight < b.weight;
    }
};

struct Graph
{
    std::string startingVertex;
    std::map<std::string, std::set<adjNode>> adjList;
};

int getWeight(const std::string &str1, const std::string &str2, int subSeqLen);
void constructGraph(const std::vector<std::string> &subSequences, Graph &graph, int subSeqLen, int minWeightThreshold);
std::string getOriginalSequence(
    int seqLen,
    int subSeqLen,
    const std::vector<std::string> &subSequences,
    const std::string &startingSubSeq,
    bool positiveErrors,
    bool negativeErrors
);