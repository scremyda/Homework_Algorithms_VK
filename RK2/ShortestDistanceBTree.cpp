#include <iostream>
#include <sstream>

template<class T, class Compare>
class BinaryTree {
public:
    BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) {}
    ~BinaryTree();
    BinaryTree(const BinaryTree& tree) = delete;
    BinaryTree& operator = (const BinaryTree& tree) = delete;

    void Add(const T& key);

    int ShortestDistance();

private:
    struct Node {
        Node* Left;
        Node* Right;
        T Key;
        Node(const T& key) : Left(nullptr), Right(nullptr), Key(key) {}
    };
    Node* root;
    Compare cmp;

    int shortestDistance(Node*& node);
    void deleteNode(Node* node);
    void add(Node*& node, const T& key);
    int min(const T& left, const T& right);
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    deleteNode(root);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(Node* node) {
    if(node == nullptr) {
        return;
    }
    deleteNode(node->Left);
    deleteNode(node->Right);
    delete node;
}

template<class T, class Compare>
int BinaryTree<T, Compare>::min(const T& left, const T& right) {
    return (left < right) ? left : right;

}

template<class T, class Compare>
int BinaryTree<T, Compare>::ShortestDistance() {
    return shortestDistance(root);
}

template<class T, class Compare>
int BinaryTree<T, Compare>::shortestDistance(Node*& node) {
    if (node == nullptr) {
        return 0;
    } else if (node->Left && node->Right) {
        return min(shortestDistance(node->Left), shortestDistance(node->Right)) + 1;
    } else if (node->Left) {
        return shortestDistance(node->Left) + 1;
    } else if (node->Right) {
        return shortestDistance(node->Right) + 1;
    } else  {
        return 1;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key) {
    add(root, key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(Node*& node, const T& key) {
    if (node == nullptr) {
        node = new Node(key);
    } else {
        Node* newNode = node;
        while(newNode != nullptr) {
            if (!cmp(newNode->Key, key)) {
                if(newNode->Left == nullptr) {
                    newNode->Left = new Node(key);
                    return;
                }
                newNode = newNode->Left;
            } else {
                if(newNode->Right == nullptr) {
                    newNode->Right = new Node(key);
                    return;
                }
                newNode = newNode->Right;
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

    std::string string;
    std::getline(std::cin, string);
    std::stringstream sstream(string);
    int element;
    while(sstream >> element) {
        tree.Add(element);
    }
    std::cout << tree.ShortestDistance();
    return 0;
}

