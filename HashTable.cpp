#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

struct StringHasher {
    unsigned int operator()(const string& key) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < key.size(); ++i) {
            hash = hash * 37 + key[i];
        }
        return hash;
    }
};

template<class T, class H>
class HashTable {
 public:
    HashTable(int initialSize, const H& hasher);
    ~HashTable() = default;
    HashTable& operator=(const HashTable& table) = delete;

    bool Has(const T& key) const;

    bool Add(const T& key);

    bool Delete(const T& key);

 private:
    H hasher_;

    struct HashTableNode {
        T key_;
        enum State { Empty, Key, Deleted } state;
        HashTableNode() : state(Empty) {}
        explicit HashTableNode(const T& key) : key_(key), state(Key) {}
    };

    size_t size_;
    size_t deletedSize_;
    vector<HashTableNode> table_;

    void growTable();
    void clearDeleted();
};

template<class T, class H>
HashTable<T, H>::HashTable(int initialSize, const H& _hasher) :
        hasher_(_hasher),
        table_(initialSize),
        size_(0),
        deletedSize_(0)
{}

template<class T, class H>
bool HashTable<T, H>::Has(const T& key) const {
    unsigned int hash = hasher_(key) % table_.size();
    for (size_t i = 0; table_[hash].state != HashTableNode::Deleted && i < table_.size(); ++i) {
        if (table_[hash].key_ == key && table_[hash].state == HashTableNode::Key) {
            return true;
        }
        hash = (hash + i + 1) % table_.size();
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T& key) {
    if (size_ >= table_.size() * 0.75) {
        growTable();
    }

    unsigned int hash = hasher_(key) % table_.size();
    size_t firstDeleted = -1;
    for (size_t i = 0; table_[hash].state != HashTableNode::Empty; ++i) {
        if (table_[hash].key_ == key && table_[hash].state == HashTableNode::Key) {
            return false;
        } else if (firstDeleted == -1 && table_[hash].state == HashTableNode::Deleted) {
            firstDeleted = hash;
        }
        hash = (hash + i + 1) % table_.size();
    }

    if (firstDeleted == -1) {
        table_[hash] = HashTableNode(key);
    } else {
        table_[firstDeleted] = HashTableNode(key);
        deletedSize_ --;
    }
    size_++;

    return true;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T& key) {
    if (deletedSize_ >= table_.size() * 0.25) {
        clearDeleted();
    }
    unsigned int hash = hasher_(key) % table_.size();
    for (size_t i = 0; table_[hash].state != HashTableNode::Empty; ++i) {
        if (table_[hash].key_ == key && table_[hash].state != HashTableNode::Deleted) {
            table_[hash].state = HashTableNode::Deleted;
            size_ --;
            deletedSize_ ++;
            return true;
        }
        hash = (hash + i + 1) % table_.size();
    }

    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable() {
    vector<HashTableNode> newTable(table_.size() * 2);
    for (const auto& n : table_) {
        if (n.state == HashTableNode::Key) {
            unsigned int hash = hasher_(n.key_) % newTable.size();
            for (size_t i = 0; newTable[hash].state != HashTableNode::Empty; ++i) {
                hash = (hash + i + 1) % newTable.size();
            }
            newTable[hash] = n;
        }
    }
    table_ = std::move(newTable);
    deletedSize_ = 0;
}

template<class T, class H>
void HashTable<T, H>::clearDeleted() {
    vector<HashTableNode> newTable(table_.size());
    for (const auto& n : table_) {
        if (n.state == HashTableNode::Key) {
            unsigned int hash = hasher_(n.key_) % newTable.size();
            for (size_t i = 0; newTable[hash].state != HashTableNode::Empty; ++i) {
                hash = (hash + i + 1) % newTable.size();
            }
            newTable[hash] = n;
        }
    }
    table_ = std::move(newTable);
    deletedSize_ = 0;
}

int main() {
    StringHasher h;
    HashTable<string, StringHasher> table(8, h);
    char operation = 0;
    string word;
    while (std::cin >> operation >> word) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}
