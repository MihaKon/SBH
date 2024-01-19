#pragma once

#include <random>
#include <vector>
#include <set>
#include <optional>
#include <map>
#include <unordered_map>

#include "./SequenceReproducer.hpp"
#include "./SequenceReproducer.cpp"

struct Vertex;
struct Edge;
struct Graph;
struct Path;

Vertex chooseNext(const std::vector<Vertex>& S, const Vertex& u, const Graph& G);
float calculateScore(const Vertex& u, const Vertex& v, const Graph& G);
std::vector<std::string> SBH(const Graph& G, const int& n, const int& subSequencesLength, const std::string& initialOligo);
