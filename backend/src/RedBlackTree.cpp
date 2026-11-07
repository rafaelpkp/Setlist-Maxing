#include "RedBlackTree.h"
#include <utility>

bool RedBlackTree::isRed(const std::unique_ptr<Node>& node) {
    return node && node->red;
}

void RedBlackTree::rotateLeft(std::unique_ptr<Node>& node) {
    auto pivot = std::move(node->right);
    node->right = std::move(pivot->left);
    pivot->red = node->red;
    node->red = true;
    pivot->left = std::move(node);
    node = std::move(pivot);
}

void RedBlackTree::rotateRight(std::unique_ptr<Node>& node) {
    auto pivot = std::move(node->left);
    node->left = std::move(pivot->right);
    pivot->red = node->red;
    node->red = true;
    pivot->right = std::move(node);
    node = std::move(pivot);
}

void RedBlackTree::flipColors(Node& node) {
    node.red = !node.red;
    node.left->red = !node.left->red;
    node.right->red = !node.right->red;
}

void RedBlackTree::fixUp(std::unique_ptr<Node>& node) {
    if (isRed(node->right) && !isRed(node->left)) rotateLeft(node);
    if (isRed(node->left) && isRed(node->left->left)) rotateRight(node);
    if (isRed(node->left) && isRed(node->right)) flipColors(*node);
}

bool RedBlackTree::insertNode(std::unique_ptr<Node>& node, const Song& song) {
    if (!node) {
        node = std::make_unique<Node>(song);
        return true;
    }
    if (song.id == node->song.id) return false;
    bool inserted = insertNode(song.id < node->song.id ? node->left : node->right, song);
    if (inserted) fixUp(node);
    return inserted;
}

bool RedBlackTree::insert(const Song& song) {
    if (!insertNode(root, song)) return false;
    root->red = false;
    ++count;
    return true;
}

const RedBlackTree::Node* RedBlackTree::findNode(const std::string& id) const {
    const Node* node = root.get();
    while (node) {
        if (id == node->song.id) return node;
        node = (id < node->song.id ? node->left : node->right).get();
    }
    return nullptr;
}

bool RedBlackTree::find(const std::string& id, Song& song) const {
    const Node* node = findNode(id);
    if (!node) return false;
    song = node->song;
    return true;
}

void RedBlackTree::moveRedLeft(std::unique_ptr<Node>& node) {
    flipColors(*node);
    if (isRed(node->right->left)) {
        rotateRight(node->right);
        rotateLeft(node);
        flipColors(*node);
    }
}

void RedBlackTree::moveRedRight(std::unique_ptr<Node>& node) {
    flipColors(*node);
    if (isRed(node->left->left)) {
        rotateRight(node);
        flipColors(*node);
    }
}

void RedBlackTree::eraseMinimum(std::unique_ptr<Node>& node) {
    if (!node->left) {
        node.reset();
        return;
    }
    if (!isRed(node->left) && !isRed(node->left->left)) moveRedLeft(node);
    eraseMinimum(node->left);
    fixUp(node);
}

void RedBlackTree::eraseNode(std::unique_ptr<Node>& node, const std::string& id) {
    if (id < node->song.id) {
        if (!isRed(node->left) && !isRed(node->left->left)) moveRedLeft(node);
        eraseNode(node->left, id);
    } else {
        if (isRed(node->left)) rotateRight(node);
        if (id == node->song.id && !node->right) {
            node.reset();
            return;
        }
        if (!isRed(node->right) && !isRed(node->right->left)) moveRedRight(node);
        if (id == node->song.id) {
            const Node* successor = node->right.get();
            while (successor->left) successor = successor->left.get();
            node->song = successor->song;
            eraseMinimum(node->right);
        } else {
            eraseNode(node->right, id);
        }
    }
    fixUp(node);
}

bool RedBlackTree::erase(const std::string& id) {
    if (!findNode(id)) return false;
    if (!isRed(root->left) && !isRed(root->right)) root->red = true;
    eraseNode(root, id);
    if (root) root->red = false;
    --count;
    return true;
}

// counts the tree and nodes, not string allocations or allocator overhead
std::size_t RedBlackTree::storageBytes() const {
    return sizeof(*this) + count * sizeof(Node);
}
