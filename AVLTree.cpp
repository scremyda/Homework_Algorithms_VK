#include <iostream>
#include <string>
#include <vector>

struct DefaultComporator {
    bool operator()(const int& l, const int& r) const {
        return l > r;
    }
};

template<class T, class Compare>
class AVLTree {
 public:
    AVLTree() = default;

    ~AVLTree() {
        nodeVisitAndDelete(root);
    }

    AVLTree(const AVLTree& other) = delete;

    AVLTree& operator=(AVLTree other) = delete;

    void insert(const T &key) {
        root = nodeInsert(root, key);
    }

    void erase(const T &key) {
        root = nodeErase(root, key);
    }

    T kStatistics(const size_t position) {
        return nodeKStatistics(root, position)->key;
    }

    int Position(const T &key) {
        return solderPosition(root, key);
    }

 private:
    struct Node {
        Node *right;
        Node *left;

        T key;

        size_t height = 1;
        size_t count = 1;

        Node() : left(nullptr), right(nullptr) {}

        explicit Node(const T &key) : key(key), left(nullptr), right(nullptr) {}
    };

    void nodeFixCount(Node *node);
    size_t nodeCount(Node *node) const;

    int solderPosition(Node *node, const T &key) const;
    Node* nodeKStatistics(Node *node, size_t position) const;
    T nodeStatistics(Node *node, size_t countNode) const;

    void nodeVisitAndDelete(Node* node);

    Node* nodeInsert(Node* node, const T &key);
    Node* nodeErase(Node* node, const T &key);

    int nodeBalanceFactor(Node* node) const;
    void nodeFixHeight(Node* node);
    size_t nodeHeight(Node* node) const;

    Node* nodeRotateRight(Node* node);
    Node* nodeRotateLeft(Node* node);

    Node* nodeBalance(Node* node);

    Node* root = nullptr;
    Compare cmp;
};

template<class T, class Compare>
int AVLTree<T, Compare>::solderPosition(Node *node, const T &key) const {
    if (!node) {
        return 0;
    } else if (cmp(key, node->key)) {
        return solderPosition(node->left, key);
    }

    return solderPosition(node->right, key) + nodeCount(node->left) + 1;
}

template<class T, class Compare>
size_t AVLTree<T, Compare>::nodeCount(Node *node) const {
    if (node) {
        return node->count;
    } else {
        return 0;
    }
}

template<class T, class Compare>
T AVLTree<T, Compare>::nodeStatistics(Node *node, size_t countNode) const {
    if (countNode == nodeCount(node->left)) {
        return node->key;
    } else if (cmp(nodeCount(node->left), countNode)) {
        return nodeStatistics(node->left, countNode);
    }

    return nodeStatistics(node->right, countNode - (nodeCount(node->left) + 1));
}

template<class T, class Compare>
void AVLTree<T, Compare>::nodeVisitAndDelete(Node *node) {
    if (!node) {
        return;
    }

    nodeVisitAndDelete(node->left);
    nodeVisitAndDelete(node->right);
    delete node;
}

template<class T, class Compare>
void AVLTree<T, Compare>::nodeFixCount(Node *node) {
    size_t countRight = nodeCount(node->right);
    size_t countLeft = nodeCount(node->left);

    node->count = countRight + countLeft + 1;
}


template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeInsert(Node *node, const T &key) {
    if (!node) {
        return new Node(key);
    }

    if (cmp(key, node->key)) {
        node->left = nodeInsert(node->left, key);
    } else if (cmp(node->key, key)) {
        node->right = nodeInsert(node->right, key);
    }

    return nodeBalance(node);
}

template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeErase(Node *node, const T &key) {
    if (!node) {
        return nullptr;
    } else if (cmp(key, node->key)) {
        node->left = nodeErase(node->left, key);
        Node* balancedNode = nodeBalance(node);
        return balancedNode;
    } else if (cmp(node->key, key)) {
        node->right = nodeErase(node->right, key);
        Node* balancedNode = nodeBalance(node);
        return balancedNode;
    } else {
        Node* left = node->left;
        Node* right = node->right;
        delete node;

        if (!right) {
            return left;
        } else {
            Node *nodeMin = right;
            std::vector<Node*> nodesPeeked;
            nodesPeeked.push_back(right);

            while (nodeMin->left) {
                nodeMin = nodeMin->left;
                nodesPeeked.push_back(nodeMin);
            }
            Node *balancedNode = nodeMin->right;

            nodesPeeked.pop_back();
            while (!nodesPeeked.empty()) {
                nodesPeeked.back()->left = balancedNode;
                balancedNode = nodeBalance(nodesPeeked.back());
                nodesPeeked.pop_back();
            }

            nodeMin->right = balancedNode;
            nodeMin->left = left;

            return nodeBalance(nodeMin);
        }
    }
}

template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeKStatistics(Node *node, size_t position) const {
    if (!node) {
        return nullptr;
    }
    size_t nodeCountLeft = nodeCount(node->left);
    if (nodeCountLeft == position) {
        return node;
    } else if (cmp(nodeCountLeft, position)) {
        return nodeKStatistics(node->left, position);
    }

    return nodeKStatistics(node->right, position - (nodeCountLeft + 1));
}

template<class T, class Compare>
int AVLTree<T, Compare>::nodeBalanceFactor(Node *node) const {
    return nodeHeight(node->right) - nodeHeight(node->left);
}

template<class T, class Compare>
void AVLTree<T, Compare>::nodeFixHeight(Node *node) {
    node->height = std::max(nodeHeight(node->left), nodeHeight(node->right)) + 1;
}

template<class T, class Compare>
size_t AVLTree<T, Compare>::nodeHeight(Node *node) const {
    if (node) {
        return node->height;
    } else {
        return 0;
    }
}

template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeRotateRight(Node *node) {
    Node* nodeLeft = node->left;
    node->left = nodeLeft->right;
    nodeLeft->right = node;

    nodeFixHeight(node);
    nodeFixCount(node);

    nodeFixHeight(nodeLeft);
    nodeFixCount(nodeLeft);
    return nodeLeft;
}


template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeRotateLeft(Node *node) {
    Node* nodeRight = node->right;
    node->right = nodeRight->left;
    nodeRight->left = node;

    nodeFixHeight(node);
    nodeFixCount(node);

    nodeFixHeight(nodeRight);
    nodeFixCount(nodeRight);
    return nodeRight;
}

template<class T, class Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::nodeBalance(Node *node) {
    nodeFixHeight(node);
    nodeFixCount(node);

    int nodeBFactor = nodeBalanceFactor(node);
    if (nodeBFactor == 2) {
        if (nodeBalanceFactor(node->right) < 0) {
            node->right = nodeRotateRight(node->right);
            return nodeRotateLeft(node);
        } else if (nodeBalanceFactor(node->right) >= 0) {
            return nodeRotateLeft(node);
        }
    } else if (nodeBFactor == -2) {
        if (nodeBalanceFactor(node->left) > 0) {
            node->left = nodeRotateLeft(node->left);
            return nodeRotateRight(node);
        } else if (nodeBalanceFactor(node->left) <= 0) {
            return nodeRotateRight(node);
        }
    }
    return node;
}

int main() {
    AVLTree<int, DefaultComporator> tree;

    size_t n;
    std::cin >> n;
    for (size_t i = n; i > 0; --i) {
        int command, element;
        std::cin >> command >> element;
        if (command == 1) {
            std::cout << tree.Position(element) << std::endl;
            tree.insert(element);
        } else {
            tree.erase(tree.kStatistics(element));
        }
    }

    return 0;
}
