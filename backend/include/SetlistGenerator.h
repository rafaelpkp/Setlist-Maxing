#pragma once
#include <vector>
#include "Song.h"

using namespace std;

struct Setlist {
    vector<Song> songs;
    long long totalDurationMs = 0;
};

Setlist generateSetlist(
    const vector<Song>& repertoire,
    long long maxDurationMs
);
