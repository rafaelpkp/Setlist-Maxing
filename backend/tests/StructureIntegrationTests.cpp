#include "HashTable.h"
#include "RedBlackTree.h"
#include "SongLoader.h"
#include "SongLibrary.h"
#include "SetlistGenerator.h"
#include <cassert>
int main() {
    auto songs = loadSongs("backend/tests/fixtures/sample_tracks.csv");
    SongLibrary library(songs);
    assert(library.size() == songs.size());
    auto hashMatches = library.search("SATELLITES", "hash");
    auto treeMatches = library.search("SATELLITES", "tree");
    assert(hashMatches.size() == 3 && treeMatches.size() == 3);
    for (size_t i = 0; i < hashMatches.size(); ++i) {
        assert(hashMatches[i].id == treeMatches[i].id);
        assert(hashMatches[i].title == treeMatches[i].title);
    }
    assert(library.search("stay", "hash").size() == 1);
    assert(library.search("stay", "tree").size() == 1);
    assert(library.search("missing", "hash").empty());
    assert(library.search("", "tree").empty());
    assert(library.search("satellites", "other").empty());
    Song found;
    assert(library.find(songs[0].id, "hash", found));
    assert(found.title == songs[0].title);
    assert(library.find(songs[0].id, "tree", found));
    assert(!library.find(songs[0].id, "other", found));
    auto duplicates = songs;
    duplicates.push_back(songs[0]);
    SongLibrary deduplicated(duplicates);
    assert(deduplicated.size() == songs.size());
    HashTable hash; RedBlackTree tree;
    for (const auto& song : songs) { assert(hash.insert(song)); assert(tree.insert(song)); }
    assert(!hash.insert(songs[0])); assert(!tree.insert(songs[0]));
    assert(hash.size() == tree.size());
    std::vector<Song> a, b;
    for (const auto& original : songs) { Song x, y; assert(hash.find(original.id, x)); assert(tree.find(original.id, y)); a.push_back(x); b.push_back(y); }
    auto hs = generateSetlist(a, 500000), ts = generateSetlist(b, 500000);
    assert(hs.totalDurationMs == ts.totalDurationMs && hs.songs.size() == ts.songs.size());
    for (size_t i = 0; i < hs.songs.size(); ++i) assert(hs.songs[i].id == ts.songs[i].id);
    assert(hash.erase(songs[0].id)); assert(tree.erase(songs[0].id));
    Song unused; assert(!hash.find(songs[0].id, unused)); assert(!tree.find(songs[0].id, unused));
}
