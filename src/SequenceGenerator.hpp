#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <iostream>

std::vector<std::string> getSequence(const int &totalLen, const int &subSeqLen);
void exportSequence(std::string &sequence);
char getRandomNucleotide();