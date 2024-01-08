#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <optional>


#include "./SequenceGenerator.hpp"
#include "./LevenshteinDistanceCalc.hpp"
#include "./LevenshteinDistanceCalc.cpp"

struct Vertex;
struct Edge;
struct Graph;
struct Path;

std::vector<Vertex> findBestSubpath(const Path& path, const int& n);
Vertex chooseInitialOligo(const Graph& G);
Vertex chooseNext(const std::vector<Vertex>& S, const Path& path, const Graph& G);
int countPossibleSuccessors(Vertex oligo, const Path& path, const Graph& G);
float calculateScore(const Vertex& u, const Vertex& v, const Graph& G);
std::vector<Vertex> constructForwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, const int& n, const int& subSequencesLength);
std::string constructBackwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, Path& path, const std::vector<std::string>& spectrum, const int& n, const int& subSequencesLength);
void applyPheromoneUpdate();
void resetPheromoneValeus(std::vector<std::vector<float>>& pheromones);
void initializePheromoneValues(std::vector<std::vector<float>>& pheromones);
void antColonySBH(const Graph& G, const int& n);
