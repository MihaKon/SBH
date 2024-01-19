#pragma once

#include <string>
#include <vector>
#include <fstream>

std::string getSequence(int len, std::fstream *file = nullptr);
std::vector<std::string> getSubSequences(std::string &sequence, const int subSeqLen, const int pePercent, const int nePercent, std::fstream *file = nullptr);
