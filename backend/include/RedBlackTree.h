#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include "Song.h"

class RedBlackTree {
public:
    RedBlackTree() = default;
    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    bool insert(const Song& song);
    bool find(const std::string& id, Song& song) const;
    bool erase(const std::string& id);
    std::size_t size() const { return count; }

private:
    struct Node {
        explicit Node(const Song& value) : song(value) {}
        Song song;
        bool red = true;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };
    std::unique_ptr<Node> root;
    std::size_t count = 0;

    const Node* findNode(const std::string& id) const;
    static bool isRed(const std::unique_ptr<Node>& node);
    static void rotateLeft(std::unique_ptr<Node>& node);
    static void rotateRight(std::unique_ptr<Node>& node);
    static void flipColors(Node& node);
    static void fixUp(std::unique_ptr<Node>& node);
    static void moveRedLeft(std::unique_ptr<Node>& node);
    static void moveRedRight(std::unique_ptr<Node>& node);
    static bool insertNode(std::unique_ptr<Node>& node, const Song& song);
    static void eraseMinimum(std::unique_ptr<Node>& node);
    static void eraseNode(std::unique_ptr<Node>& node, const std::string& id);

    friend struct RedBlackTreeTestAccess;
};
