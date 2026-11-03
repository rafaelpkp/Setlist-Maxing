#include <iostream>
#include<vector>
#include "Song.h"
#include "SetlistGenerator.h"

using namespace std;

int main() {
    
    vector<Song> repertoire = {
    {"song-1", "Mr. Brightside", "The Killers",
     222000, 148.0, 1, 1, 0.91},

    {"song-2", "Take Me Out", "Franz Ferdinand",
     237000, 105.0, 4, 0, 0.68},

    {"song-3", "Seven Nation Army", "The White Stripes",
     232000, 124.0, 4, 0, 0.46},

    {"song-4", "Dreams", "Fleetwood Mac",
     257000, 120.0, 0, 1, 0.49},

    {"song-5", "Reptilia", "The Strokes",
     219000, 158.0, 2, 1, 0.91},

    {"song-6", "Everlong", "Foo Fighters",
     250000, 158.0, 2, 1, 0.88},

    {"song-7", "Use Somebody", "Kings of Leon",
     231000, 137.0, 0, 1, 0.72},

    {"song-8", "Do I Wanna Know?", "Arctic Monkeys",
     273000, 85.0, 5, 0, 0.53},

    {"song-9", "Are You Gonna Be My Girl", "Jet",
     213000, 105.0, 9, 1, 0.95},

    {"song-10", "Creep", "Radiohead",
     238000, 92.0, 7, 1, 0.43}
};

    for (const auto& song : repertoire) {
        cout << song.title << " by " << song.artists << '\n';
        cout << "Duration: " << song.durationMs / 1000 << " seconds\n";
    }

    Setlist setlist = generateSetlist(repertoire, 1000000);
    cout << "\nGenerated Setlist:\n";
    for (const auto& song : setlist.songs) {
        cout << song.title << " by " << song.artists << '\n';
    }
    cout << setlist.totalDurationMs / 1000 << " seconds total duration\n";
    cout << "Total duration in minutes: " << setlist.totalDurationMs / 60000 << " minutes\n";
    return 0;
}
