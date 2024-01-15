#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <optional>
#include <map>
#include <unordered_map>

#include "./SequenceGenerator.hpp"
#include "./LevenshteinDistanceCalc.hpp"
#include "./LevenshteinDistanceCalc.cpp"
#include "./SequenceReproducer.hpp"
#include "./SequenceReproducer.cpp"

struct Vertex;
struct Edge;
struct Graph;
struct Path;

void findBestSubpath(const Path& path, const int& n, const int& subSeqLen, std::multimap<Vertex, Vertex>& candidates);
Vertex chooseNext(const std::vector<Vertex>& S, const Vertex& u, const Graph& G, std::multimap<Vertex, Vertex>& candidates);
int countPossibleSuccessors(Vertex oligo, const Path& path, const Graph& G);
float calculateScore(const Vertex& u, const Vertex& v, const Graph& G);
std::vector<Vertex> constructForwardSolution(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo);
std::vector<std::string> SBH(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo);
