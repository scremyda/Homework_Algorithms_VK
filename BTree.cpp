#include <queue>
#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>

template <class Key, class Compare>
class BTree {
 public:
    explicit BTree(size_t minDegree);
    ~BTree();
    BTree(const BTree& other) = delete;
    BTree& operator=(const BTree& other) = delete;

    void insert(const Key &key);
    bool find(const Key &key);
    void printByLevel();

 private:
    struct Node {
        explicit Node(bool leaf): leaf(leaf) {}
        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }
        Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;

        bool leaf;
        std::vector<Key> keys;
        std::vector<Node*> children;
    };

    bool isNodeFull(Node *node);
    bool findAux(Node* node, const Key &key);
    void split(Node *parent, int index);
    void insertNonFull(Node *node, const Key &key);

    Compare cmp;
    Node *root;
    size_t t;
};

template <class Key, class Compare>
BTree<Key, Compare>::BTree(size_t minDegree): root(nullptr), t(minDegree) {
    assert(minDegree >= 2);
}

template <class Key, class Compare>
BTree<Key, Compare>::~BTree() {
    if (root)
        delete root;
}

template <class Key, class Compare>
void BTree<Key, Compare>::insert(const Key &key) {
    if (!root)
        root = new Node(true);

    if (isNodeFull(root)) {
        Node *newRoot = new Node(false);
        newRoot->children.push_back(root);
        root = newRoot;
        split(root, 0);
    }

    insertNonFull(root, key);
}

template <class Key, class Compare>
bool BTree<Key, Compare>::find(const Key &key) {
    return findAux(root, key);
}

template <class Key, class Compare>
void BTree<Key, Compare>::printByLevel() {
    if (!root)
        return;

    std::queue<Node*> nodes;
    nodes.push(root);
    size_t size;
    while (!nodes.empty()) {
        size = nodes.size();
        for (size_t i = 0; i < size; ++i) {
            Node* current = nodes.front();
            nodes.pop();
            for (const Key& key : current->keys) {
                std::cout << key << " ";
            }

            if (!current->leaf) {
                for (Node* child : current->children) {
                    nodes.push(child);
                }
            }
        }
        std::cout << std::endl;
    }
}

template <class Key, class Compare>
bool BTree<Key, Compare>::isNodeFull(Node *node) {
    return node->keys.size() == 2*t - 1;
}

template <class Key, class Compare>
bool BTree<Key, Compare>::findAux(Node* node, const Key &key) {
    int i = 0;
    while (i < node->keys.size() && cmp(node->keys[i], key)) {
        i++;
    }
    if (i < node->keys.size() && key == node->keys[i]) {
        return true;
    } else if (node->leaf) {
        return false;
    } else {
        return findAux(node->children[i], key);
    }
}

template <class Key, class Compare>
void BTree<Key, Compare>::split(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *newNode = new Node(child->leaf);
    newNode->keys.resize(t - 1);

    for (int j = 0; j < t - 1; j++) {
        newNode->keys[j] = child->keys[j + t];
    }

    if (!child->leaf) {
        newNode->children.resize(t);
        for (int j = 0; j < t; j++) {
            newNode->children[j] = child->children[j + t];
        }
    }

    child->keys.resize(t - 1);
    if (!child->leaf) {
        newNode->children.resize(t);
        for (int j = 0; j < t; j++) {
            newNode->children[j] = child->children[j + t];
        }
    }

    child->keys.resize(t - 1);
    if (!child->leaf) {
        child->children.resize(t);
    }

    parent->children.insert(parent->children.begin() + index + 1, newNode);
    parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
}

template <class Key, class Compare>
void BTree<Key, Compare>::insertNonFull(Node *node, const Key &key) {
    int pos = node->keys.size() - 1;
    if (node->leaf) {
        node->keys.resize(node->keys.size() + 1);
        while (pos >= 0 && cmp(key, node->keys[pos])) {
            node->keys[pos + 1] = node->keys[pos];
            pos--;
        }
        node->keys[pos + 1] = key;
    } else {
        while (pos >= 0 && cmp(key, node->keys[pos])) {
            pos--;
        }
        if (isNodeFull(node->children[pos + 1])) {
            split(node, pos + 1);
            if (key > node->keys[pos + 1]) {
                pos++;
            }
        }
        insertNonFull(node->children[pos + 1], key);
    }
}

struct IntCmp {
    bool operator()(const int& l, const int& r) { return l < r; }
};


int main() {
    std::string string;
    std::getline(std::cin, string);
    std::stringstream sStreamFirst(string);
    int element;
    sStreamFirst >> element;
    BTree<int, IntCmp> tree(element);

    std::getline(std::cin, string);
    std::stringstream sStreamSecond(string);
    while (sStreamSecond >> element) {
        tree.insert(element);
    }

    tree.printByLevel();

    return 0;
}
