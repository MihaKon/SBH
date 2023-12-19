#pragma once

#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

std::string getSequence(const int &len);
std::vector<std::string> getSubSequences(std::string &sequence, const int &subSeqLen, bool addPositiveErrors, bool addNegativeErrors);