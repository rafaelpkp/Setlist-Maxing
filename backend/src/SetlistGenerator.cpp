#include "SetlistGenerator.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>

double transitionScore(const Song& current, const Song& next) {
    bool validBpm = current.bpm > 0 && next.bpm > 0;
    bool validEnergy = current.energy >= 0 && current.energy <= 1 &&
                       next.energy >= 0 && next.energy <= 1;
    double totalScore = 0.0;
    double totalWeight = 0.0;

    if (validBpm) {
        double bpmScore = min(abs(current.bpm - next.bpm) / 50.0, 1.0);
        totalScore += 0.5 * bpmScore;
        totalWeight += 0.5;
    }
    if (validEnergy) {
        double energyScore = abs(current.energy - next.energy);
        totalScore += 0.5 * energyScore;
        totalWeight += 0.5;
    }

    if (totalWeight == 0.0) return -1; // no features to compare
    return totalScore / totalWeight;
}

Setlist generateSetlist(const vector<Song>& repertoire, long long maxDurationMs) {
    Setlist result;
    unordered_set<string> addedSongIds;
    if (maxDurationMs <= 0) return result;

    // add songs in order, skipping duplicates and anything that won't fit
    for (const auto& song : repertoire) {
        if (song.id.empty() || addedSongIds.count(song.id) || song.durationMs <= 0) {
            continue;
        }
        if (result.totalDurationMs + song.durationMs <= maxDurationMs) {
            result.songs.push_back(song);
            result.totalDurationMs += song.durationMs;
            addedSongIds.insert(song.id);
        }
    }

    // keep the opener, then pick the closest transition at each step
    for (size_t i = 0; i + 1 < result.songs.size(); i++) {
        size_t bestIndex = i + 1;
        for (size_t j = i + 2; j < result.songs.size(); j++) {
            double candidateScore = transitionScore(result.songs[i], result.songs[j]);
            double bestScore = transitionScore(result.songs[i], result.songs[bestIndex]);
            if (candidateScore >= 0 && (bestScore < 0 || candidateScore < bestScore)) {
                bestIndex = j;
            }
        }
        swap(result.songs[i + 1], result.songs[bestIndex]);
    }
    return result;
}
