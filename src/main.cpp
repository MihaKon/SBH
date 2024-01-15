#include <iostream>
#include <string>

#include "./SequenceGenerator.hpp"
#include "./SequenceGenerator.cpp"

#include "./GreedyHeuristicAlgorithm.hpp"
#include "./GreedyHeuristicAlgorithm.cpp"


int main(){

    // 300
    // CCTGGCATATCGGGACAGGACGAAGTGAGTCTAGATCATTGAACCAAATTCGTGCACCTCCCCTCTCTCGTACTCCACCTCGTGGTCCTTTCCGCACCGGACTCGGCGTTAGTCACGAGGCGCCCGCGCGGCCGGATGACGTAGTTTCGGCTTTGCCGTGAATCGATATTCCTTTGCATGACCTAACGAAAACCCGTAGGGCTAAGGAATATTCATGCCGGGTTATCCCTAGAATACGTGACGGAGGCGCGAACTAATGAGACTACGAGTGAAGGATCCCAAACACTTCATTTCGATTAC
    int length = 300;
    std::vector<std::string> sequences;
    std::string sequence = getSequence(length);
    sequences.emplace_back(sequence);
    //         n=500 k=8
    //      sekwencja i  | spektrum 0 | spektrum N | spektrum P | spektrum N+P
    //% bledow 0%        |   [i][0]   |   [i][1]   |  [i][2]    | [i][3]
    //         2%        |            |   [i][4]   |  [i][5]    | [i][6]
    //         4%        |            |   [i][7]   |  [i][8]    | [i][9]
    //         6%        |            |   [i][10]  |  [i][11]   | [i][12]
    //         8%        |            |   [i][13]  |  [i][14]   | [i][15]
    //         10%       |            |   [i][16]  |  [i][17]   | [i][18]
    //         ...       |            |   [i][...] |  [i][...]  | [i][...]
    // wzór: 
    //      F= {N=1 P=2 N+P=3}, x = % bledow
    //      [] = (x/2)*3+F

   // std::string sequence = "ACTCTGG";
    int subSequencesLength = 7;
    std::vector<std::string> spectrum = getSubSequences(sequences[0], subSequencesLength, 0, 0);

   /* std::cout << "Spektrum: ";
    for (const auto& s : spectrum){
        std::cout << s << " ";
    }*/
    std::cout << std::endl << sequences[0] << std::endl;
    Graph G;
    G.constructGraph(spectrum, subSequencesLength);

    std::string initialOligo = sequences[0].substr(0,subSequencesLength);

    std::vector<std::string> kmers = SBH(G, length, subSequencesLength, initialOligo);
    std::string createdSequence = reproduceSequence(kmers, subSequencesLength);
    std::cout << createdSequence << std::endl;
    std::cout << float(getLevenshteinDistance(sequences[0], createdSequence)) / length;
    return 0;
}
