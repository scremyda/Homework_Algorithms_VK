#include <iostream>
#include <vector>
#include <stack>

template<class T, class Compare>
class BinaryTree {
 public:
    explicit BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
    ~BinaryTree();
    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;

    void Add(const T& key);

    void DfsPostOrder(void (*visit)(const T& key)) const;

 private:
    struct Node {
        Node* Left;
        Node* Right;
        T Key;
        explicit Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
    };
    Node* root;
    Compare cmp;

    void deleteNode(Node* node);
    void add(Node*& node, const T& key);
    void dfsPostOrder(Node* node, void (*visit)(const T& key)) const;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    deleteNode(root);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    std::stack<Node*> stackNodes;
    Node* lastVisited = nullptr;
    Node* peekedNode = nullptr;

    while (node || !stackNodes.empty()){
        if (node) {
            stackNodes.push(node);
            node = node->Left;
        } else {
            peekedNode = stackNodes.top();
            if (peekedNode->Right && lastVisited != peekedNode->Right) {
                node = peekedNode->Right;
            } else {
                delete peekedNode;
                lastVisited = peekedNode;
                stackNodes.pop();
            }
        }
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key)
{
    add(root, key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(Node*& node, const T& key) {
    if (node == nullptr) {
        node = new Node(key);
    } else {
        Node* newNode = node;
        while (newNode != nullptr) {
            if (!cmp(newNode->Key, key)) {
                if (newNode->Left == nullptr) {
                    newNode->Left = new Node(key);
                    return;
                }
                newNode = newNode->Left;
            } else {
                if (newNode->Right == nullptr) {
                    newNode->Right = new Node(key);
                    return;
                }
                newNode = newNode->Right;
            }
        }
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsPostOrder(void (*visit)(const T& key)) const {
    dfsPostOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsPostOrder(Node* node, void (*visit)(const T& key)) const {
    if (node == nullptr) {
        return;
    }

    std::stack<Node*> stackNodes;
    Node* lastVisited = nullptr;
    Node* peekedNode = nullptr;

    while (node || !stackNodes.empty()){
        if (node) {
            stackNodes.push(node);
            node = node->Left;
        } else {
            peekedNode = stackNodes.top();
            if (peekedNode->Right && lastVisited != peekedNode->Right) {
                node = peekedNode->Right;
            } else {
                visit(peekedNode->Key);
                lastVisited = peekedNode;
                stackNodes.pop();
            }
        }
    }
}

struct IntCmp {
    bool operator()(int l, int r) { return l <= r; }
};

int main()
{
    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);
    size_t count = 0;
    std::cin >> count;
    int element;
    for (;count > 0; count--) {
        std::cin >> element;
        tree.Add(element);
    }
    tree.DfsPostOrder([](const int& key) { std::cout << key << " "; });
    return 0;
}