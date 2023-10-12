#include <iostream>
#include <cstring>
#include <cassert>

template <typename T>
class Deque {
public:
    Deque() : buffer_(nullptr), tail_(0), head_(0), bufferSize_(0) {}
    explicit Deque(T size);
    ~Deque();
    void pushBack(const T& element);
    void pushFront(const T& element);
    T popBack();
    T popFront();
    void resize();
    bool IsEmpty() const;

private:
    T * buffer_;
    int bufferSize_;
    int head_;
    int tail_;
};

template <typename T>
Deque<T>::Deque(T size) {
    buffer_ = new T[size];
    bufferSize_ = size;
    head_ = 0;
    tail_ = 0;
}

template <typename T>
Deque<T>::~Deque() {
    delete [] buffer_;
}

template<class T>
bool Deque<T>::IsEmpty() const {
    return head_ == tail_;
}

template <typename T>
void Deque<T>::pushBack(const T& element) {
    if (bufferSize_ == 0 || head_ == ((tail_ + 1) % bufferSize_)) {
        resize();
    }

    buffer_[tail_] = element;
    tail_ = (tail_ + 1) % bufferSize_;
}

template <typename T>
void Deque<T>::pushFront(const T& element) {
    if (bufferSize_ == 0 || ((head_ + bufferSize_ - 1) % bufferSize_) == tail_) {
        resize();
    }

    head_ = (head_ + bufferSize_ - 1) % bufferSize_;
    buffer_[head_] = element;
}

template <typename T>
T Deque<T>::popBack() {
    assert(!IsEmpty());

    tail_ = (tail_ - 1 + bufferSize_) % bufferSize_;
    T result = buffer_[tail_];
    return result;
}

template <typename T>
T Deque<T>::popFront() {
    assert(!IsEmpty());

    T result = buffer_[head_];
    head_ = (head_ + 1) % bufferSize_;
    return result;
}

template <typename T>
void Deque<T>::resize() {
    if (bufferSize_ == 0) {
        bufferSize_ = 2;
        buffer_ = new T[2];
    } else {
        int newBufferSize = bufferSize_ * 2;
        T * temporaryBuffer = new T[newBufferSize];
        if (head_ > tail_) {
            memcpy(temporaryBuffer + head_, buffer_ + head_,
                   (bufferSize_ - head_) * sizeof(T));

            memcpy(temporaryBuffer + bufferSize_, buffer_, tail_ * sizeof(T));

            tail_ += bufferSize_;
        } else {
            memcpy(temporaryBuffer, buffer_, bufferSize_ * sizeof(T));
        }

        delete [] buffer_;
        bufferSize_ = newBufferSize;
        buffer_ = temporaryBuffer;
    }
}

int main() {
    int n;
    std::cin >> n;

    int command;
    int data;
    bool result = true;
    Deque<int> deckDynamicBuffer;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> command;
        std::cin >> data;
        switch(command) {
            case 1:
                deckDynamicBuffer.pushFront(data);
                break;
            case 2:
                if(deckDynamicBuffer.IsEmpty()) {
                    result = result && data == -1;
                } else {
                    result = result && deckDynamicBuffer.popFront() == data;
                }
                break;
            case 3:
                deckDynamicBuffer.pushBack(data);
                break;
            case 4:
                if(deckDynamicBuffer.IsEmpty()) {
                    result = result && data == -1;
                } else {
                    result = result && deckDynamicBuffer.popBack() == data;
                }
                break;
        }
    }
    std::cout << (result ? "YES" : "NO");
    return 0;
}
