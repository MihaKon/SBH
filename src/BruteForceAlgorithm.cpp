#include "./BruteForceAlgorithm.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <memory>

struct adjNode
{
    std::string val;
    int weight;
    bool visited = false;

    adjNode(std::string v, int w) : val(v), weight(w) {}

    friend bool operator<(const adjNode& a, const adjNode& b) {
        return a.weight > b.weight;
    }
};

struct Graph
{
    std::string startingVertex;
    std::vector<adjNode> visited;
    std::map<std::string, std::vector<std::unique_ptr<adjNode>>> adjList;

    Graph(std::string startingVertex) : startingVertex(startingVertex) {}
};


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

void sortAdjList(Graph &graph)
{
    for (auto &adjList : graph.adjList)
    {
        std::sort(adjList.second.begin(), adjList.second.end(),
                      [](const std::unique_ptr<adjNode>& a, const std::unique_ptr<adjNode>& b) {
                          return *a < *b;
                      });
    }
}

void constructGraph(const std::vector<std::string> &subSequences, Graph &graph, int subSeqLen, int minWeightThreshold)
{
    
    for (int i = 0; i < subSequences.size(); i++)
    {
        adjNode node(subSequences[i], 0);
        graph.visited.push_back(node);

        for (int j = 0; j < subSequences.size(); j++)
        {
            if(i == j) continue;
            int weight = getWeight(subSequences[i], subSequences[j], subSeqLen);
            if (weight >= minWeightThreshold)
            {
                std::unique_ptr<adjNode> nodePtr = std::make_unique<adjNode>(adjNode(subSequences[j], weight));
                graph.adjList[subSequences[i]].push_back(std::move(nodePtr));
            }
        }
    }

    sortAdjList(graph);
}

std::string getOriginalSequence(
    int seqLen,
    int subSeqLen,
    const std::vector<std::string> &subSequences,
    const std::string &startingSubSeq,
    bool positiveErrors,
    bool negativeErrors)
{
    Graph graph(startingSubSeq);
    constructGraph(subSequences, graph, subSeqLen, subSeqLen - 3);

    std::string originalSequence = startingSubSeq;
    std::string currentVertex = startingSubSeq;

    for(int i = 0; i < seqLen - subSeqLen; i++)
    {
        if(graph.adjList[currentVertex].size() == 0)
        {
            break;
        }
        int nodeIndex = 0;
        while(graph.adjList[currentVertex][nodeIndex]->visited)
        {
            nodeIndex++;
        }
        originalSequence += graph.adjList[currentVertex][0]->val[subSeqLen - 1];
        currentVertex = graph.adjList[currentVertex][0]->val;
    } 
    
    return originalSequence;
}