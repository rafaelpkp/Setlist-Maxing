#pragma once
#include "Song.h"
#include <cstddef>
#include <vector>

struct BenchmarkResult {
    std::size_t songCount = 0;
    std::size_t found = 0;
    std::size_t deleted = 0;
    std::size_t storageBytes = 0;
    double insertMs = 0;
    double findMs = 0;
    double eraseMs = 0;
};

struct BenchmarkComparison {
    BenchmarkResult hash;
    BenchmarkResult tree;
};

// runs the same operations on separate, temporary structures
BenchmarkComparison compareStructures(const std::vector<Song>& songs);
