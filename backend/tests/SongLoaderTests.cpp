#include <cassert>
#include <iostream>
#include <stdexcept>
#include "SongLoader.h"

using namespace std;

int main() {
    vector<Song> songs = loadSongs("backend/tests/fixtures/sample_tracks.csv");
    assert(songs.size() == 5);
    assert(songs[0].id == "sample01");
    assert(songs[0].durationMs == 210000);
    assert(songs[0].bpm == 120.0 && songs[0].energy == 0.65);
    assert(songs[1].title == "Stay, Stay");
    assert(songs[1].artists == "['June Reed', 'The Satellites']");
    assert(songs[2].title == "Say \"Hello\"");
    assert(songs[2].key == 9 && songs[2].mode == 0);

    bool failed = false;
    try {
        loadSongs("backend/tests/fixtures/file-that-does-not-exist.csv");
    } catch (const runtime_error&) {
        failed = true;
    }
    assert(failed);
    cout << "song loader tests passed\n";
}
