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

struct Vertex{
    std::string label;
    int id;
};
struct Edge {
    int u; // first
    int v; // second
    float weight; // weight = length of DNA strand

};

struct Graph {
    std::vector<Vertex> V;
    std::vector<Edge> A; 

    void addEdge(int u, int v, float weight){
        A.emplace_back(u,v,weight);
    }
};

struct Path{
    std::vector<int> OligoIDs;

    float getCost(const std::vector<int>& path, const Graph& G, const int& l) {
        int pathLength = path.size();

        if (pathLength < 2) {
            std::cerr << "Path should contain at least two oligonucleotides.\n";
            return 0.0f; 
        }

        float cost = pathLength * l; // l(p) * l

        for (int i = 0; i < pathLength - 1; ++i) {
            int oligo1 = path[i];
            int oligo2 = path[i + 1];

            for (const auto& edge : G.A) {
                if (edge.u == oligo1 && edge.v== oligo2) {
                    cost -= edge.weight;
                    break;
                }
            }
        }

        return cost;
    }
};

std::vector<int> findBestSubpath(const Path& path){
    std::vector<int> subpath;
    return subpath;
}

int ChooseInitialOligonucleotide(const std::vector<Vertex>& V){ // OK
    // wybierz losowo oligonukleotyd z S
    if (V.empty()){
        std::cerr << "Empty set of oligonucleotides.\n";
        return 0;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, V.size() - 1);

    int randomIndex = dis(gen);

    int initialOligonucleotide = V[randomIndex].id;

    return initialOligonucleotide;

}

int chooseNext(const std::vector<Vertex>& V, const int& n){
    return 0;
}
std::string ConstructForwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, Path& path, const std::vector<std::string>& spectrum, const int& n, const int& subSequencesLength){
    int oligonucleotideID = ChooseInitialOligonucleotide(G.V); // i* =
    
    path.OligoIDs.emplace_back(oligonucleotideID); // p = i*
    
    std::vector<std::string> tempSpectrum = spectrum; // S'=S

    while (path.getCost(path.OligoIDs, G, subSequencesLength)){
        //tempSpectrum.erase(oligonucleotideID); //auto it-> do zmiany // S' = S'\{i*}
        oligonucleotideID = chooseNext(G.V, n); // i* = chooseNext(S'), do zmiany argumenty funkcji
        path.OligoIDs.push_back(oligonucleotideID); // Extend path p by adding i*  to its end, OK
    }

    path.OligoIDs = findBestSubpath(path);
    
    std::string sequence="";
    //todo: dna sequence that is obtained from p
    return sequence;
}
std::string ConstructBackwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, const Path& path){
    return "";
}

void ApplyPheromoneUpdate(){}
void ResetPheromoneValeus(std::vector<std::vector<float>>& pheromones){}

void InitializePheromoneValues(std::vector<std::vector<float>>& pheromones) { // OK
    int n = pheromones.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pheromones[i][j] = 0.5f;
        }
    }
}
void AntColonySBH(const Graph& graph, const int n){
    Graph pbs;
    Graph prb;
    float cf = 0.0f;
    bool bs_update = false;

    int nf = 3; 
    int nb = 3;
 
    std::vector<std::vector<float>> pheromone_matrix(graph.A.size(), std::vector<float>(graph.A.size()));
    InitializePheromoneValues(pheromone_matrix);

//Jak określić kiedy metaheurystyka ma się zatrzymać?
//Jeżeli algorytm jest zdefiniowany tak, że znamy optymalną wartość funkcji celu, to algorytm może zatrzymać się po jej osiągnięciu.
//Jeżeli optimum nie jest znane, to trzeba zatrzymać się po pewnym czasie. Ze względów praktycznych algorytmy zachłanne nie powinny działać dłużej niż kilka sekund, a algorytmy takie jak BB, GA, Tabu nie powinny działać dłużej niż kilka (góra kilkanaście) minut, aby możliwa była weryfikacja ich działania na zajęciach. Trzeba tak dobrać liczbę iteracji, aby ten czas nie był przekraczany.
    while (pbs.V.size() != n ){
        for (int j = 1; j<= nf; j++){

        }

    }
}
