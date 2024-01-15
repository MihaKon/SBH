#include <iostream>
#include <string>

#include "./SequenceGenerator.hpp"
#include "./AntColonyAlgorithm.hpp"
int main(){
    int length = 20;
    std::string sequence = getSequence(length);
    
    int subSequencesLength = 3;
    std::vector<std::string> spectrum = getSubSequences(sequence, subSequencesLength, 0, 0);


    return 0;
}
