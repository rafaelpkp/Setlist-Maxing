#include "RedBlackTree.h"
#include "SetlistGenerator.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>

void check(bool condition) {
    if (!condition) throw std::runtime_error("red-black tree check failed");
}

struct RedBlackTreeTestAccess {
    static int validate(const std::unique_ptr<RedBlackTree::Node>& node,
                        const std::string* lower, const std::string* upper,
                        std::size_t& count) {
        if (!node) return 0;
        check(!lower || *lower < node->song.id);
        check(!upper || node->song.id < *upper);
        ++count;
        int left = validate(node->left, lower, &node->song.id, count);
        int right = validate(node->right, &node->song.id, upper, count);
        check(left == right); // every path has the same black height
        check(!RedBlackTree::isRed(node->right)); // left-leaning representation
        if (node->red) {
            check(!RedBlackTree::isRed(node->left));
            check(!RedBlackTree::isRed(node->right));
        }
        return left + (node->red ? 0 : 1);
    }
    static void validate(const RedBlackTree& tree) {
        std::size_t count = 0;
        check(!RedBlackTree::isRed(tree.root));
        validate(tree.root, nullptr, nullptr, count);
        check(count == tree.size());
    }
};

Song makeSong(const std::string& id) {
    return {id, "Title " + id, "Artist " + id, 1000, 120.0, 3, 1, 0.5};
}

bool sameSong(const Song& a, const Song& b) {
    return a.id == b.id && a.title == b.title && a.artists == b.artists &&
           a.durationMs == b.durationMs && a.bpm == b.bpm &&
           a.key == b.key && a.mode == b.mode && a.energy == b.energy;
}

int main() {
    // try every insertion and deletion order for a small tree
    std::vector<std::string> insertion = {"a", "b", "c", "d", "e"};
    do {
        std::vector<std::string> deletion = {"a", "b", "c", "d", "e"};
        do {
            RedBlackTree small;
            for (const auto& id : insertion) {
                check(small.insert(makeSong(id)));
                RedBlackTreeTestAccess::validate(small);
            }
            for (const auto& id : deletion) {
                check(small.erase(id));
                RedBlackTreeTestAccess::validate(small);
            }
            check(small.size() == 0);
        } while (std::next_permutation(deletion.begin(), deletion.end()));
    } while (std::next_permutation(insertion.begin(), insertion.end()));

    // try the different insertion rotations, then delete the root
    for (const auto& ids : {std::vector<std::string>{"c", "b", "a"},
                            {"a", "b", "c"}, {"c", "a", "b"}, {"a", "c", "b"}}) {
        RedBlackTree tree;
        for (const auto& id : ids) {
            check(tree.insert(makeSong(id)));
            RedBlackTreeTestAccess::validate(tree);
        }
        check(tree.erase("b"));
        RedBlackTreeTestAccess::validate(tree);
        for (const auto& id : {"a", "c"}) {
            Song result;
            check(tree.find(id, result) && sameSong(result, makeSong(id)));
            check(tree.erase(id));
            RedBlackTreeTestAccess::validate(tree);
        }
        check(tree.size() == 0);
    }

    RedBlackTree tree;
    Song result = makeSong("unchanged");
    check(!tree.find("missing", result));
    check(sameSong(result, makeSong("unchanged")));
    check(!tree.erase("missing"));
    // the tree uses the same ID rules as the shared interface
    check(tree.insert(makeSong("")));
    check(tree.erase(""));

    std::map<std::string, Song> reference;
    std::mt19937 random(3530);
    for (int i = 0; i < 12000; ++i) {
        std::string id = std::to_string(random() % 500);
        Song song = makeSong(id);
        song.title = "Version " + std::to_string(i);
        if (random() % 3 != 0) {
            check(tree.insert(song) == reference.emplace(id, song).second);
        } else {
            check(tree.erase(id) == (reference.erase(id) != 0));
        }
        RedBlackTreeTestAccess::validate(tree);
        check(tree.size() == reference.size());
        for (const auto& entry : reference) {
            check(tree.find(entry.first, result));
            check(sameSong(result, entry.second));
        }
    }
    for (const auto& entry : reference) {
        check(tree.erase(entry.first));
        RedBlackTreeTestAccess::validate(tree);
    }
    // sorted input should stay balanced and work with the generator
    std::vector<Song> repertoire;
    for (int i = 0; i < 2000; ++i) {
        auto song = makeSong(std::to_string(10000 + i));
        check(tree.insert(song));
        RedBlackTreeTestAccess::validate(tree);
        check(tree.find(song.id, result));
        repertoire.push_back(result);
    }
    auto setlist = generateSetlist(repertoire, 3000);
    check(setlist.songs.size() == 3 && setlist.totalDurationMs == 3000);
    for (auto it = repertoire.rbegin(); it != repertoire.rend(); ++it) {
        check(tree.erase(it->id));
        RedBlackTreeTestAccess::validate(tree);
    }
    check(tree.size() == 0);
    std::cout << "red-black tree tests passed\n";
}
