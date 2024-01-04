#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <set>

#include "./SequenceGenerator.hpp"

struct Vertex;
struct Edge;
struct Graph;
struct Path;

std::vector<int> findBestSubpath(const Path& path);
int ChooseInitialOligonucleotide(const std::vector<int>& V);
int ChooseNext(const std::vector<int>& V, const int n);
std::string ConstructForwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, Path& path, const int& n, const int& subSequencesLength);
std::string ConstructBackwardSolution(const Graph& G, const std::vector<std::vector<float>>& pheromones, Path& path, const int& n, const int& subSequencesLength);
void ApplyPheromoneUpdate();
void ResetPheromoneValeus(std::vector<std::vector<float>>& pheromones);
void InitializePheromoneValues(std::vector<std::vector<float>>& pheromones);
void AntColonySBH(const Graph& graph, const int n);