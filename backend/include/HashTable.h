#pragma once
#include <vector>
#include <string>
#include "Song.h"
using namespace std;

class HashTable {
    vector<vector<Song>> table;
    size_t count = 0;        // how many songs are stored
    size_t primeIndex = 0;   // where we are in the PRIMES list
    const double MAX_LOAD = 0.75;

    size_t hashFunction(const string& id) const;
    size_t getIndex(const string& id) const;
    void rehash();

public:
    HashTable();

    bool insert(const Song& song);
    bool find(const string& id, Song& song) const;
    bool erase(const string& id);
    size_t size() const;
    size_t storageBytes() const;

    // extra stuff for testing / the report
    bool isEmpty() const;
    size_t bucketCount() const;
    double loadFactor() const;
    void printTable() const;
};

