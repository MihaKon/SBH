#include "./GreedyHeuristicAlgorithm.hpp"

struct Vertex {
    std::string label;
    int ID;

    bool operator<(const Vertex& other) const {
        return label < other.label;  
    }

    bool operator==(const Vertex& other) const {
        return label == other.label;
    }

    bool operator!=(const Vertex& other) const {
        return label != other.label;
    }

    Vertex() : ID(0) {}

    Vertex(const std::string& l, int id) : label(l), ID(id) {}
};

namespace std {
    template <>
    struct hash<Vertex> {
        size_t operator()(const Vertex& v) const {
            return hash<string>()(v.label) ^ hash<int>()(v.ID);
        }
    };
}

struct Edge {
    Vertex u; 
    Vertex v; 
    float weight; 

    Edge(const Vertex& u, const Vertex& v, float w) : u(u), v(v), weight(w) {}
};

struct Graph {
    std::vector<Vertex> V;
    std::vector<Edge> A;
    std::vector<std::vector<Vertex>> successorsList;
    std::multimap<std::pair<Vertex, Vertex>, int> edgeScores;
    std::vector<std::set<int>> adjList;

    void addEdge(Vertex u, Vertex v, float weight) {
        Edge edge(u, v, weight);
        A.emplace_back(edge);
    }

	void createAdjList() {
		adjList.resize(V.size());
		for (const auto& pair : A) {
			adjList[pair.u.ID].insert(pair.v.ID);
		}
	}

    void constructGraph(const std::vector<std::string>& spectrum, const int& subSequencesLength) {
        for (const auto& label : spectrum) {
            Vertex vertex(label, static_cast<int>(V.size()));  
            if (std::find(V.begin(), V.end(), vertex) == V.end()) {   
                 V.emplace_back(vertex);
            }
        }

        for (int i = 0; i < V.size(); i++) {
            for (int j = 0; j < V.size(); j++) {
                if (i != j) {
                    int matchingLength = 0;
                    std::string iLabel = "";
                    std::string jLabel = "";
                    for (int k = 1; k < subSequencesLength; k++) {
                        iLabel = V[i].label[V[i].label.length() - k] + iLabel;
                        jLabel += V[j].label[k - 1];
                        if (iLabel == jLabel)
                            matchingLength = iLabel.length();
                    }
                    addEdge(V[i], V[j], matchingLength);
                }
            }
        }

        for (Edge& edge : A) {
            edgeScores.insert(std::make_pair(std::make_pair(edge.u, edge.v), edge.weight));
        }

    }

    Graph() {}
};

struct Path {
    std::vector<Vertex> oligos;

    float getCost(const Graph& G, const int& l) const {
        int pathLength = oligos.size(); 
        float cost = pathLength * l; // l(p) * l

        for (int i = 0; i < pathLength - 1; i++) {
            Vertex oligo1 = oligos[i];
            Vertex oligo2 = oligos[i + 1];

            cost -= calculateScore(oligo1, oligo2, G);
        }
        return cost;
    }

    Path() {}
};

float calculateScore(const Vertex& u, const Vertex& v, const Graph& G) {
    auto range = G.edgeScores.equal_range(std::make_pair(u, v));
    for (auto it = range.first; it != range.second; ++it) {
        return it->second;
    }
    return 0.0f;  
}

Vertex chooseNext(const std::vector<Vertex>& S, const Vertex& u, const Graph& G, std::multimap<Vertex, Vertex>& candidates) {
    // wybierz następny oligonukleotyd, patrząc w przód na potencjalne przyszłe najlepsze sąsiedztwo
    Vertex bestNextOligo;
    int maxScore=0;
    int maxSumScore = 0;
    for (const Vertex& oligo : S) {
        float score = calculateScore(u,oligo,G);
        if (score > maxScore){
            for (const Vertex& oligo2 : S){
                float score2 = calculateScore(oligo,oligo2,G);
                if (score + score2 >= maxSumScore) {
                    candidates.insert(std::make_pair(oligo,oligo2));
                    maxScore = score;
                    maxSumScore = score+score2;
                    bestNextOligo=oligo;
                }
            }
        }
    }
	
    return bestNextOligo;
}
std::vector<Vertex> constructForwardSolution(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo) {
    Vertex oligo;
    for (Vertex v : G.V){
        if (v.label == initialOligo){
            oligo = v;
            break;
        }
    } //chooseInitialOligo(G.V); // i* =
    std::vector<Path> paths;
    Path path;
    std::multimap<Vertex, Vertex> candidates;
    path.oligos.emplace_back(oligo); // p = i*
    std::vector<Vertex> spectrumPrim = G.V; // S'=S
    std::cout << "Tworzona sciezka: ";
    while (path.getCost(G, subSequencesLength) < n) {
        auto it = std::find(spectrumPrim.begin(), spectrumPrim.end(), oligo);
        if (it != spectrumPrim.end()) {
            spectrumPrim.erase(it); // Usuń i* ze zbioru S'
        }
        oligo = chooseNext(spectrumPrim, path.oligos.back(), G, candidates); // i* = chooseNext(S'), do zmiany argumenty funkcji
        path.oligos.emplace_back(oligo); // Rozszerz ścieżkę p, dodając i* na jej koniec,
        std::cout << oligo.label << " ";
    }

    std::cout<<std::endl;
    return path.oligos;
}

std::vector<std::string> SBH(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo) {
    std::vector<Vertex> verticesSolution = constructForwardSolution(G, n, subSequencesLength, initialOligo);
    std::vector<std::string> solution;

    for (Vertex v : verticesSolution) {
        solution.emplace_back(v.label);
    }
    return solution;
}
