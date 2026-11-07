#pragma once

#include "HashTable.h"
#include "RedBlackTree.h"
#include <string>
#include <vector>

// keeps the same songs in both structures and a small list for searching
class SongLibrary {
public:
    explicit SongLibrary(const std::vector<Song>& songs);

    std::vector<Song> search(const std::string& query, const std::string& structure) const;
    bool find(const std::string& id, const std::string& structure, Song& song) const;
    std::size_t size() const;
    const std::vector<Song>& songs() const;

private:
    struct SearchEntry {
        std::string id;
        std::string title;
        std::string artists;
    };

    HashTable hash;
    RedBlackTree tree;
    std::vector<Song> catalog; // kept for the benchmark
    std::vector<SearchEntry> searchIndex;
};
