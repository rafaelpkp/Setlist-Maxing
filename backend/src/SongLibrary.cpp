#include "SongLibrary.h"
#include <algorithm>
#include <cctype>

namespace {
std::string lowercase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return text;
}
}

SongLibrary::SongLibrary(const std::vector<Song>& songs) {
    for (const Song& song : songs) {
        // use the hash table to skip IDs we've already added
        if (hash.insert(song)) {
            tree.insert(song);
            catalog.push_back(song);
            searchIndex.push_back({song.id, lowercase(song.title), lowercase(song.artists)});
        }
    }
}

std::vector<Song> SongLibrary::search(const std::string& query, const std::string& structure) const {
    std::vector<Song> matches;
    std::string searchText = lowercase(query);
    if (searchText.empty()) return matches;

    for (const SearchEntry& entry : searchIndex) {
        bool titleMatches = entry.title.find(searchText) != std::string::npos;
        bool artistMatches = entry.artists.find(searchText) != std::string::npos;
        if (titleMatches || artistMatches) {
            Song song;
            // retrieve the full song using the selected structure
            if (find(entry.id, structure, song)) matches.push_back(song);
            if (matches.size() == 50) break;
        }
    }
    return matches;
}

bool SongLibrary::find(const std::string& id, const std::string& structure, Song& song) const {
    if (structure == "hash") return hash.find(id, song);
    if (structure == "tree") return tree.find(id, song);
    return false;
}

std::size_t SongLibrary::size() const {
    return catalog.size();
}

const std::vector<Song>& SongLibrary::songs() const {
    return catalog;
}
