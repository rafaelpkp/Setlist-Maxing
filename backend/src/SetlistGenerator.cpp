#include "SetlistGenerator.h"
#include <unordered_set>
#include<algorithm>
#include <cmath>
double transitionScore(const Song& current, const Song& next) {
    bool validBpm = current.bpm > 0 && next.bpm > 0; // Check if both songs have valid BPM values
    bool validEnergy = current.energy >= 0 && current.energy <= 1 && next.energy >= 0 && next.energy <= 1; // Check if energy values are within the valid range [0, 1]

    double totalScore = 0.0;
    double totalWeight = 0.0;

    if (validBpm) {
        double difference = abs(current.bpm - next.bpm);
        double bpmScore = min(difference / 50.0, 1.0);
        totalScore += 0.5 * bpmScore;
        totalWeight += 0.5; 
    }

    if (validEnergy) {
        double energyScore = abs(current.energy - next.energy);
        totalScore += 0.5 * energyScore;
        totalWeight += 0.5;
    }

    if (totalWeight == 0.0) {
        return 0.0; // Neither feature can be compared.
    }

    return totalScore / totalWeight;
}
Setlist generateSetlist(const vector<Song>& repertoire, long long maxDurationMs) {
    // Go through the songs in the repertoire.
    // If a song fits in the remaining time, add it to result.songs.
    // Update result.totalDurationMs when you add a song.
    Setlist result;
    unordered_set<string> addedSongIds; // To track added songs and avoid duplicates
    if (maxDurationMs <= 0) {
        return result; // Return empty setlist if maxDurationMs is not valid
    }

    for (const auto& song : repertoire) {
        if (song.id.empty()) {
            continue; // Skip songs with empty IDs
        }
        if (addedSongIds.count(song.id)) {
            continue; // Skip if the song is already added
        }
        if(song.durationMs <= 0) {
            continue; // Skip songs with not valid duration
        }
        if (result.totalDurationMs + song.durationMs <= maxDurationMs) {
            result.songs.push_back(song);
            result.totalDurationMs += song.durationMs;
            addedSongIds.insert(song.id);
        }
    }
    // ordering selected songs by transition score
    for (size_t i = 0; i + 1 < result.songs.size(); i++) {
        size_t bestIndex = i +1;

        for (size_t j = i + 2; j < result.songs.size(); j++) {
            if (transitionScore(result.songs[i], result.songs[j]) < transitionScore(result.songs[i], result.songs[bestIndex])) {
                bestIndex = j;
            }
        }
        swap(result.songs[i + 1], result.songs[bestIndex]);

    
    }
    return result;
}