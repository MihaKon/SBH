#include <iostream>
#include <string>

#include "./SequenceGenerator.hpp"
#include "./SequenceGenerator.cpp"

#include "./AntColonyAlgorithm.hpp"
#include "./AntColonyAlgorithm.cpp"

int main(){
    int length = 7;
    //std::string sequence = getSequence(length);
    
    std::string sequence = "ACTCTGG";
    int subSequencesLength = 3;
    std::vector<std::string> spectrum = getSubSequences(sequence, subSequencesLength, 0, 0);

    std::cout << "sequence: " << sequence << std::endl;


    Graph G;
    G.constructGraph(spectrum, subSequencesLength);
    for (Edge E : G.A ){
        std::cout << E.u.label << "->" << E.v.label << ":" << E.weight << std::endl;
    }

    antColonySBH(G, length);


    return 0;
}
