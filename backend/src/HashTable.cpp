#include <iostream>
#include <vector>
#include <string>
#include "HashTable.h"
using namespace std;

// songs are keyed by Spotify ID, with collisions stored in the same bucket

// primes that roughly double each time, used as the bucket count (the mod value)
const vector<size_t> PRIMES = {
    53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,
    49157, 98317, 196613, 393241, 786433, 1572869, 3145739
};

HashTable::HashTable() {
    table.resize(PRIMES[0]);
}

// polynomial rolling hash: turns the id string into a big number
size_t HashTable::hashFunction(const string& id) const {
    size_t hash = 0;
    for (char c : id) {
        hash = hash * 31 + (unsigned char)c;
    }
    return hash;
}

// division method: mod by the number of buckets (a prime)
size_t HashTable::getIndex(const string& id) const {
    return hashFunction(id) % table.size();
}

bool HashTable::insert(const Song& song) {
    size_t index = getIndex(song.id);

    // don't allow duplicate ids
    for (const Song& s : table[index]) {
        if (s.id == song.id) {
            return false;
        }
    }

    table[index].push_back(song);
    count++;

    if (loadFactor() > MAX_LOAD) {
        rehash();
    }
    return true;
}

bool HashTable::find(const string& id, Song& song) const {
    size_t index = getIndex(id);

    for (const Song& s : table[index]) {
        if (s.id == id) {
            song = s;
            return true;
        }
    }
    return false; // song is left unchanged
}

bool HashTable::erase(const string& id) {
    size_t index = getIndex(id);
    vector<Song>& bucket = table[index];

    for (size_t i = 0; i < bucket.size(); i++) {
        if (bucket[i].id == id) {
            // order inside a bucket doesn't matter, so swap with the last one and pop
            bucket[i] = bucket.back();
            bucket.pop_back();
            count--;
            return true;
        }
    }
    return false;
}

size_t HashTable::size() const {
    return count;
}

bool HashTable::isEmpty() const {
    return count == 0;
}

size_t HashTable::bucketCount() const {
    return table.size();
}

double HashTable::loadFactor() const {
    return (double)count / table.size();
}

// grow to the next prime and put every song in its new bucket
void HashTable::rehash() {
    if (primeIndex + 1 >= PRIMES.size()) {
        return; // already at the biggest size in the list
    }
    primeIndex++;

    // swap instead of copying so we don't duplicate every song
    vector<vector<Song>> oldTable;
    oldTable.swap(table);
    table.resize(PRIMES[primeIndex]);

    for (const vector<Song>& bucket : oldTable) {
        for (const Song& s : bucket) {
            table[getIndex(s.id)].push_back(s);
        }
    }
    // count stays the same since we just moved the songs
}

// prints only the buckets that have something in them
void HashTable::printTable() const {
    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].empty()) continue;

        cout << "Bucket " << i << ": ";
        for (const Song& s : table[i]) {
            cout << "[" << s.id << " - " << s.title << "] ";
        }
        cout << '\n';
    }
    cout << "Songs: " << count << ", buckets: " << table.size()
         << ", load factor: " << loadFactor() << '\n';
}

// counts the table and reserved song slots, not string allocations or allocator overhead
size_t HashTable::storageBytes() const {
    size_t bytes = sizeof(*this) + table.capacity() * sizeof(vector<Song>);
    for (const auto& bucket : table) bytes += bucket.capacity() * sizeof(Song);
    return bytes;
}
