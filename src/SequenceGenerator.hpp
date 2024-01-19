#pragma once

#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

std::string getSequence(int len);
std::vector<std::string> getSubSequences(std::string &sequence, int subSeqLen, bool addPositiveErrors, bool addNegativeErrors);