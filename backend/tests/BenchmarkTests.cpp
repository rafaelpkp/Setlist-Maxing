#include "Benchmark.h"
#include "SongLoader.h"
#include <cassert>
#include <stdexcept>

int main() {
    auto songs = loadSongs("backend/tests/fixtures/sample_tracks.csv");
    auto comparison = compareStructures(songs);
    assert(comparison.hash.songCount == 5 && comparison.tree.songCount == 5);
    assert(comparison.hash.found == 5 && comparison.tree.found == 5);
    assert(comparison.hash.deleted == 5 && comparison.tree.deleted == 5);
    assert(comparison.hash.storageBytes > 0 && comparison.tree.storageBytes > 0);
    assert(songs.size() == 5);

    auto empty = compareStructures({});
    assert(empty.hash.songCount == 0 && empty.tree.songCount == 0);

    songs.push_back(songs.front());
    bool rejectedDuplicate = false;
    try { compareStructures(songs); }
    catch (const std::runtime_error&) { rejectedDuplicate = true; }
    assert(rejectedDuplicate);
}
