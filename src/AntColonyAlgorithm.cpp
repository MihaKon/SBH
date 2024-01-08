#include "./AntColonyAlgorithm.hpp"
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
    int id;

    bool operator<(const Vertex& other) const {
        return label < other.label;  // You may need to adjust the comparison based on your requirements
    }

    bool operator==(const Vertex& other) const {
        return label == other.label;
    }

    bool operator!=(const Vertex& other) const {
        return label != other.label;
    }
};

struct Edge {
    Vertex u; // pierwszy
    Vertex v; // drugi
    float weight; // waga = długość oligonukleotydu - długość najdłuższej sekwencji, której sufiks u pokrywa się z prefiksem v

    Edge(Vertex u, Vertex v, int weight) : u(u), v(v), weight(weight) {}
};

struct Graph {
    std::vector<Vertex> V;
    std::vector<Edge> A;
    std::vector<std::vector<Vertex>> successorsList;

    std::optional<Vertex> getVbyID(int ID) {
        for (const Vertex& v : V) {
            if (v.id == ID) {
                return v;
            }
        }

        return std::nullopt;
    }

    void addEdge(Vertex u, Vertex v, float weight) {
        Edge edge(u, v, weight);
        A.emplace_back(edge);
    }

    void constructGraph(const std::vector<std::string>& spectrum, const int& subSequencesLength) {
        // dodaj wierzchołki do zbioru V na podstawie spektrum
        for (const auto& label : spectrum) {
            Vertex vertex{label, static_cast<int>(V.size())};  // Przypisz unikalne ID na podstawie indeksu
            if (std::find(V.begin(), V.end(), vertex) == V.end()) {
                // dodaj wierzchołek tylko jeśli nie istnieje jeszcze w wektorze
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

        successorsList = getSuccessorList();
    }

    // tworzenie listy następników dla każdego wierzchołka
    std::vector<std::vector<Vertex>> getSuccessorList() {
        std::vector<std::vector<Vertex>> successors(V.size());

        for (const auto& edge : A) {
            successors[edge.u.id].emplace_back(edge.v);
        }

        return successors;
    }
};

struct Path {
    std::vector<Vertex> oligos;

    float getCost(const Graph& G, const int& l) const {
        int pathLength = oligos.size(); 
        float cost = pathLength * l; // l(p) * l

        std::cout << "poczatkowy koszt: " << cost << std::endl;
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

        std::cout << "koncowy koszt: " << cost << std::endl;
        return cost;
    }
};

// funkcja do przebudowy
std::vector<Vertex> findBestSubpath(const Path& path, const int& n) {
    std::vector<Vertex> subpath;

    for (const Vertex& oligo : path.oligos) {
        // oblicz dlugosc podsciezki p <-- ale to juz mamy.
        int pLength = 0;
        for (const Vertex& nextOligo : path.oligos) {
            if (nextOligo.label != oligo.label) {
                pLength++;
            }
        }

        // jezeli dlugosc p jest wieksza niz dlugosc p* to p*=p
        if (pLength > subpath.size()) {
            subpath = path.oligos;
        }
        // jezeli dlugosc p* <= n to zwroc p*
        if (subpath.size() <= n) {
            return subpath;
        }
    }

    // jezeli p* jest za dlugie to zwracamy nic
    return {};
}

float calculateScore(const Vertex& u, const Vertex& v, const Graph& G) {
       int matchingLength = 0;
    for (const Edge& edge : G.A) {
        if ((edge.u == u && edge.v == v) || (edge.u == v && edge.v == u)) {
            matchingLength = edge.weight;
        }
    }
    return matchingLength;
}

Vertex chooseInitialOligo(const Graph& G) {
    // Define the sets Sbs and Swp
    std::vector<Vertex> Sbs;
    std::vector<Vertex> Swp;

    for (const Vertex& oligo : G.V) {
        float bestSuccessorScore = 0.0f;;

        for (const Vertex& nextOligo : G.V) {
            if (!(nextOligo == oligo)) {
                Path path;
                path.oligos.emplace_back(oligo);
                float successorScore = calculateScore(nextOligo, path.oligos.back(), G);

                if (successorScore >= bestSuccessorScore) {
                    bestSuccessorScore = successorScore;
                }
            }
        }
         Sbs.emplace_back(oligo);
    }

// swp from sbs
        for (const Vertex& oligo : Sbs) {
        float bestPredecessorScore = 0.0f;

        for (const Vertex& nextOligo : Sbs) {
            if (!(nextOligo == oligo)) {
                Path path;
                path.oligos.emplace_back(oligo);

                path.oligos.clear();
                path.oligos.emplace_back(nextOligo);
                float predecessorScore = calculateScore(oligo, path.oligos.back(), G);

                if (predecessorScore <= bestPredecessorScore) {
                    bestPredecessorScore = predecessorScore;
                }
            }
        }

         Swp.emplace_back(oligo);
    }

    // Choose a starting oligo from Swp
    std::vector<Vertex> SwpVector;
    for (const Vertex& oligo : Swp) {
        SwpVector.emplace_back(oligo);
    }

    // As starting oligonucleotide, choose the one (from Swp) that is found first.
    Vertex bestOligo;
    for (const Vertex& oligo : Swp) {
        bestOligo = oligo;
        break;
    }
    std::cout << "startowy: " << bestOligo.label << std::endl;
    return bestOligo;
}

Vertex chooseNext(const std::vector<Vertex>& S, const Path& path, const Graph& G) {
    // wybierz następny oligonukleotyd, patrząc w przód na potencjalne przyszłe najlepsze sąsiedztwo
    // i* := argmax{op[l(p)]i + oi suc(i) | i ∈ Sˆ}

    // Funkcja do obliczania oceny oligonukleotydu
    Vertex bestNextOligo;
    float maxScore = -1.0f;

    for (const Vertex& oligo : S) {
        //float score = calculateScore(oligo, path, G);
        float score = calculateScore(oligo, path.oligos.back(), G);
        if (score > maxScore) {
            maxScore = score;
            bestNextOligo = oligo;
        }
    }

    return bestNextOligo;
}

std::vector<Vertex> constructForwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, const int& n, const int& subSequencesLength) {
    Vertex oligo = chooseInitialOligo(G);//chooseInitialOligo(G.V); // i* =
    Path path;
    path.oligos.emplace_back(oligo); // p = i*

    std::vector<Vertex> spectrumPrim = G.V; // S'=S
    std::cout << "start: " << path.oligos[0].label << std::endl;
    std::cout << "n=" << n << std::endl;
    while (path.getCost(G, subSequencesLength) < n) {
        auto it = std::find(spectrumPrim.begin(), spectrumPrim.end(), oligo);
        if (it != spectrumPrim.end()) {
            spectrumPrim.erase(it); // Usuń i* ze zbioru S'
        }
        oligo = chooseNext(spectrumPrim, path, G); // i* = chooseNext(S'), do zmiany argumenty funkcji
        path.oligos.emplace_back(oligo); // Rozszerz ścieżkę p, dodając i* na jej koniec,
        std::cout << "dodaje: " << oligo.label << std::endl;
    }

    for (Vertex v : path.oligos) {
        std::cout << "koniec: " << v.label << " ";
    }
    std::cout << std::endl;

    path.oligos = findBestSubpath(path, n);

    return path.oligos;
}

std::string constructBackwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones,
                                      const Path& path) {
    // todo: implementacja konstrukcji odwrotnej ścieżki
    return "";
}

void applyPheromoneUpdate() {
    // todo: aktualizacja feromonów
}

void resetPheromoneValues(std::vector<std::vector<float>>& pheromones) {
    // todo: resetowanie wartości feromonów
}

void initializePheromoneValues(std::vector<std::vector<float>>& pheromones) {
    int n = pheromones.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pheromones[i][j] = 0.5f;
        }
    }
}

void antColonySBH(const Graph& G, const int& n) {
    Path pib; // iteration best solution
    Path pbs; // best-so-far solution
    Path prb; // restart-best solution 
    float cf = 0.0f; //convergence factor 0<=cf<=1
    bool bsUpdate = false; // becomes true when the algorithm reaches cf

    int nf = 3;
    int nb = 3; 

    std::vector<std::vector<float>> pheromoneMatrix(G.V.size(), std::vector<float>(G.V.size()));
    initializePheromoneValues(pheromoneMatrix);

    std::vector<Vertex> solution = constructForwardSolution(G, pheromoneMatrix, n, 3);

    for (Vertex v : solution) {
        std::cout << v.label << " ";
    }
    // Jak określić, kiedy metaheurystyka ma się zatrzymać?
    // Jeżeli algorytm jest zdefiniowany tak, że znamy optymalną wartość funkcji celu, to algorytm może zatrzymać się po jej osiągnięciu.
    // Jeżeli optimum nie jest znane, to trzeba zatrzymać się po pewnym czasie. Ze względów praktycznych algorytmy zachłanne nie powinny działać dłużej niż kilka sekund,
    // a algorytmy takie jak BB, GA, Tabu nie powinny działać dłużej niż kilka (góra kilkanaście) minut, aby możliwa była weryfikacja ich działania na zajęciach.
    // Trzeba tak dobrać liczbę iteracji, aby ten czas nie był przekraczany.
    /*while (pbs.oligos.size() != n) {
        for (int j = 1; j <= nf; j++) {
            Path pj = constructForwardSolution(G, pheromoneMatrix, n, 3);
        }
    }*/
}
