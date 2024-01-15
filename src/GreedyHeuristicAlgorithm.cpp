#include "./GreedyHeuristicAlgorithm.hpp"
//  DANE
//  n - liczba nukleotydów docelowej sekwencji st
//  S = {1,...,m} spektrum, l - długość oligonukleotydów
//  G=(V,A), V=S, i∈S - oligonukleotydy, l(i)- dlugosc oligonukleotydow, each a(i,j)∈A ma wagę o(i,j), która jest długością nici DNA o sufixie i oraz prefixie j
//  p = (i1,...,ik) - sciezka hamiltona, l(p) - dlugosc sciezki jako lizcba oligonukleotydow, 
//  koszt ścieżki p==>>   c(p) = l(p) * l - suma wszystkich o(p[r], p[r+1]) w zakresie r=1:l(p)-1
//  szukamy: ściezki Hamlitona p* w G o l(p*) >= l(p) dla wszysztkich możliwych ścieżek p które spełniają c(p) <= n
//
//  ACO
//  pib - iteration best solution -  the best solution generated in the current iteration by the ants
//  pbs - best so far solution - the best solution generated since the start of the algorithm;
//  prb - the restart-best solution -  the best solution generated since the last restart of the algorithm
//  the convergence factor cf, 0 ≤ cf ≤ 1: a measure of how far the algorithm is from convergence;
//  the Boolean variable bs update: it becomes true when the algorithm reaches convergence.
// operator (f*) : f(p) > f(p') <=> l(p) > l(p') or (l(p) = l(p')) and c(p) < c*(p'))
// nf - number of forward ants; od lewej do prawej
// nb - number of backward ants; od prawej do lewej


struct Vertex {
    std::string label;
    int ID;

    bool operator<(const Vertex& other) const {
        return label < other.label;  // You may need to adjust the comparison based on your requirements
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
    Vertex u; // pierwszy
    Vertex v; // drugi
    float weight; // waga = długość oligonukleotydu - długość najdłuższej sekwencji, której sufiks u pokrywa się z prefiksem v

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
		for (auto& pair : A) {
			adjList[pair.u.ID].insert(pair.v.ID);
		}
	}
    void constructGraph(const std::vector<std::string>& spectrum, const int& subSequencesLength) {
        // dodaj wierzchołki do zbioru V na podstawie spektrum
        for (const auto& label : spectrum) {
            Vertex vertex(label, static_cast<int>(V.size()));  
            if (std::find(V.begin(), V.end(), vertex) == V.end()) {   // dodaj wierzchołek tylko jeśli nie istnieje jeszcze w wektorze
                 V.emplace_back(vertex);
            }
        }

        // dodajemy krawędzie 
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

            for (const auto& edge : G.A) {
                if (edge.u == oligo1 && edge.v == oligo2) {
                    cost -= edge.weight;
                    break;
                }
            }
        }
        return cost;
    }

    Path() {}
};

// funkcja do przebudowy albo usunieciaa
void findBestSubpath(const Path& path, const int& n, const int& subSeqLen, std::multimap<Vertex, Vertex>& candidates) {
    Graph newG;
    std::vector<std::string> spectrum;
    for (const Vertex& v : path.oligos){
        spectrum.emplace_back(v.label);
    }

    newG.constructGraph(spectrum,subSeqLen);

    std::string baseSequence = reproduceSequence(spectrum, subSeqLen);
    int maxSize = baseSequence.length();

    Path tempPath;
    tempPath.oligos.emplace_back(path.oligos[0]);
    // jezeli dlugosc p jest wieksza niz dlugosc p* to p*=p
    // jezeli dlugosc p* <= n to zwroc p*


    // jezeli p* jest za dlugie to zwracamy nic
}

float calculateScore(const Vertex& u, const Vertex& v, const Graph& G) {
    int matchingLength = 0;
    for (const Edge& edge : G.A) {
        if ((edge.u == u && edge.v == v)) {
            matchingLength = edge.weight;
            break;
        }
    }
    return matchingLength;
}

Vertex chooseNext(const std::vector<Vertex>& S, const Vertex& u, const Graph& G, std::multimap<Vertex, Vertex>& candidates) {
    // wybierz następny oligonukleotyd, patrząc w przód na potencjalne przyszłe najlepsze sąsiedztwo
    // i* := argmax{op[l(p)]i + oi suc(i) | i ∈ Sˆ}
    // Funkcja do obliczania oceny oligonukleotydu
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
    }//chooseInitialOligo(G.V); // i* =
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
    // path.oligos = findBestSubpath(path, n, subSequencesLength, candidates);

    return path.oligos;
}

std::vector<std::string> SBH(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo) {
    std::vector<Vertex> verticesSolution = constructForwardSolution(G, n, subSequencesLength, initialOligo);
    std::vector<std::string> solution;

    std::cout << "Sciezka: ";
    for (Vertex v : verticesSolution) {
        std::cout << v.label << " ";
        solution.emplace_back(v.label);
    }
    std::cout << std::endl;
    return solution;
}
