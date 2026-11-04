#pragma once

#include <string>
#include <vector>
#include "Song.h"

using namespace std;

// skips rows without an id or positive duration; missing audio values use Song defaults
// throws runtime_error if the file, header, or csv structure can't be read
vector<Song> loadSongs(const string& filePath);
