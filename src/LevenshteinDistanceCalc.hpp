#pragma once

#include <string>
#include <vector>

int getLevenshteinDistance(std::string sequenceOne, std::string sequenceTwo);
double levenshteinPercentage(int levenshteinDistance, int maxLength);