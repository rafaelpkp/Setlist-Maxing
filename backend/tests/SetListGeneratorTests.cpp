#include <cassert>
#include <iostream>
#include <vector>
#include "SetlistGenerator.h"

using namespace std;

int main() {
    vector<Song> emptyRepertoire;
    vector<Song> repertoire = {
        {"1", "Song 1", "Artist A", 200000, 120.0, 5, 1, 0.8},
        {"2", "Song 2", "Artist B", 180000, 130.0, 7, 0, 0.6},
        {"3", "Song 3", "Artist C", 240000, 110.0, 2, 1, 0.9},
        {"4", "Song 4", "Artist D", 150000, -1.0, -1, -1, -1.0}, // missing bpm and energy
        {"5", "Song 5", "Artist E", -100000, 140.0, 3, 1, 0.7}, // invalid duration
        {"6", "", "Artist F", 210000, 125.0, 4, 0, 0.5}, // empty title
        {"7", "Song 7", "", 190000, 115.0, -1, -1, -1.0}, // empty artist
        {"8", "Song 8", "Artist H", 220000, -10.0, -1, -1, -1.0}, // invalid bpm
        {"9", "Song 9", "Artist I", 230000, -1.0, -1, -1, -10.0} // invalid energy
    };
    Setlist result = generateSetlist(emptyRepertoire, 600000);
    Setlist expected = generateSetlist(repertoire, 600000);
    assert(result.songs.empty());
    assert(result.totalDurationMs == 0);
    assert(expected.songs.size() == 3);
    assert(expected.totalDurationMs == 530000);
    assert(expected.songs[0].id == "1");
    assert(expected.songs[1].id == "2");
    assert(expected.songs[2].id == "4");
    assert(expected.totalDurationMs <= 600000);

    // no songs should fit with a zero or negative limit
    for (long long limit : {0LL, -1000LL}) {
        Setlist list = generateSetlist(repertoire, limit);
        assert(list.songs.empty());
        assert(list.totalDurationMs == 0);
    }

    // skip a song that doesn't fit and add one that fits exactly
    vector<Song> durations = {
        {"a", "First", "Artist", 400, 120, 0, 1, 0.5},
        {"b", "Too long", "Artist", 700, 120, 0, 1, 0.5},
        {"c", "Fits", "Artist", 600, 120, 0, 1, 0.5}
    };
    Setlist exact = generateSetlist(durations, 1000);
    assert(exact.songs.size() == 2);
    assert(exact.songs[0].id == "a");
    assert(exact.songs[1].id == "c");
    assert(exact.totalDurationMs == 1000);
    assert(generateSetlist(durations, 100).songs.empty());

    // skip invalid ids and durations, and don't add the same id twice
    vector<Song> invalidAndDuplicates = {
        {"", "No ID", "Artist", 100, 120, 0, 1, 0.5},
        {"zero", "Zero", "Artist", 0, 120, 0, 1, 0.5},
        {"negative", "Negative", "Artist", -100, 120, 0, 1, 0.5},
        {"same", "Original", "Artist", 100, 120, 0, 1, 0.5},
        {"same", "Duplicate", "Artist", 100, 120, 0, 1, 0.5}
    };
    Setlist unique = generateSetlist(invalidAndDuplicates, 1000);
    assert(unique.songs.size() == 1);
    assert(unique.songs[0].title == "Original");
    assert(unique.totalDurationMs == 100);

    // pick a known transition over an unknown one
    vector<Song> mixed = {
        {"1", "Opener", "Artist", 100, 120, 0, 1, 0.5},
        {"2", "Unknown", "Artist", 100, -1, 0, 1, -1},
        {"3", "Known", "Artist", 100, 130, 0, 1, 0.6}
    };
    Setlist knownFirst = generateSetlist(mixed, 300);
    assert(knownFirst.songs.size() == 3);
    assert(knownFirst.songs[0].id == "1");
    assert(knownFirst.songs[1].id == "3");
    assert(knownFirst.songs[2].id == "2");
    assert(knownFirst.totalDurationMs == 300);

    // if every transition is unknown, pick the first remaining song
    vector<Song> unknown = {
        {"1", "Opener", "Artist", 100, -1, 0, 1, -1},
        {"2", "Second", "Artist", 100, -1, 0, 1, -1},
        {"3", "Third", "Artist", 100, -1, 0, 1, -1}
    };
    Setlist unknownOrder = generateSetlist(unknown, 300);
    assert(unknownOrder.songs.size() == 3);
    assert(unknownOrder.songs[0].id == "1");
    assert(unknownOrder.songs[1].id == "2");
    assert(unknownOrder.songs[2].id == "3");
    assert(unknownOrder.totalDurationMs == 300);

    // bpm or energy alone can help pick the next song
    vector<Song> bpmOnly = {
        {"1", "Opener", "Artist", 100, 120, 0, 1, -1},
        {"2", "Far", "Artist", 100, 160, 0, 1, -1},
        {"3", "Close", "Artist", 100, 125, 0, 1, -1}
    };
    Setlist bpmOrder = generateSetlist(bpmOnly, 300);
    assert(bpmOrder.songs.size() == 3);
    assert(bpmOrder.songs[1].id == "3");
    vector<Song> energyOnly = {
        {"1", "Opener", "Artist", 100, -1, 0, 1, 0.5},
        {"2", "Far", "Artist", 100, -1, 0, 1, 0.9},
        {"3", "Close", "Artist", 100, -1, 0, 1, 0.6}
    };
    Setlist energyOrder = generateSetlist(energyOnly, 300);
    assert(energyOrder.songs.size() == 3);
    assert(energyOrder.songs[1].id == "3");

    cout << "All tests passed!\n";
    return 0;
}
