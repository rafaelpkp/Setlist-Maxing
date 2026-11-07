#include "Benchmark.h"
#include "HashTable.h"
#include "RedBlackTree.h"
#include <chrono>
#include <stdexcept>

namespace {
using Clock = std::chrono::steady_clock;

double milliseconds(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template <typename Structure>
BenchmarkResult runBenchmark(const std::vector<Song>& songs, std::size_t count) {
    Structure structure;
    BenchmarkResult result;
    result.songCount = count;

    auto start = Clock::now();
    for (std::size_t i = 0; i < count; ++i) {
        if (!structure.insert(songs[i])) throw std::runtime_error("Benchmark songs must have unique IDs");
    }
    auto end = Clock::now();
    result.insertMs = milliseconds(start, end);
    result.storageBytes = structure.storageBytes();

    Song foundSong;
    start = Clock::now();
    for (std::size_t i = 0; i < count; ++i) {
        if (structure.find(songs[i].id, foundSong)) ++result.found;
    }
    end = Clock::now();
    result.findMs = milliseconds(start, end);

    start = Clock::now();
    for (std::size_t i = 0; i < count; ++i) {
        if (structure.erase(songs[i].id)) ++result.deleted;
    }
    end = Clock::now();
    result.eraseMs = milliseconds(start, end);
    if (result.found != count || result.deleted != count || structure.size() != 0) {
        throw std::runtime_error("Benchmark operation failed");
    }
    return result;
}
}

BenchmarkComparison compareStructures(const std::vector<Song>& songs) {
    std::size_t count = songs.size();
    BenchmarkComparison comparison;
    comparison.hash = runBenchmark<HashTable>(songs, count);
    comparison.tree = runBenchmark<RedBlackTree>(songs, count);
    return comparison;
}
