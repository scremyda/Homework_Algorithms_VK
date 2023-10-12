#include <iostream>


template <class T>
class CompareByValue {
 public:
    bool operator()(const T& left, const T& right) {
        return left < right;
    }
};

template <class R>
class MergeArrays {
 public:
    void Merge(R** arrayOfArrays, R* arraysLength, R arraysCount, R* result);
};

template <class T, class Compare>
class Heap {
 public:
	explicit Heap(Compare cmp);
    Heap(int size, Compare cmp);
    ~Heap();
	void Add(const T& item);
	T ExtractMin();
    Heap(const Heap&) = delete;
    T& operator=(const T&) = delete;

 private:
    void siftDown(size_t i);
    void siftUp(size_t i);
    Compare cmp_;
	T* buffer_;
	size_t bufferSize_;
	size_t size_;
};

struct Element {
	int value_;
	size_t valueIndex_;
	size_t arrayIndex_;
    Element(int value, size_t valueIndex, size_t arrayIndex);
    Element(): value_(0), valueIndex_(0), arrayIndex_(0) {}
};


Element::Element(int value, size_t valueIndex, size_t arrayIndex) {
    value_ = value;
    valueIndex_ = valueIndex;
    arrayIndex_ = arrayIndex;
}

bool operator<(const Element& lhs, const Element& rhs) {
    return lhs.value_ < rhs.value_;
}

template <class T, class Compare>
Heap<T, Compare>::Heap(int size, Compare cmp) {
    cmp_ = cmp;
    bufferSize_ = size;
    size_ = 0;
    buffer_ = new T[bufferSize_];
}

template <class T, class Compare>
Heap<T, Compare>::~Heap() {
    delete[] buffer_;
}

template <class T, class Compare>
Heap<T, Compare>::Heap(Compare cmp) {
    cmp_ = cmp;
}

template <class T, class Compare>
void Heap<T, Compare>::Add(const T& item) {
    if (bufferSize_ < size_) {
        bufferSize_ = bufferSize_ * 2;
        T* tmpBuffer = new T[bufferSize_];
        std::copy(buffer_, buffer_ + size_, tmpBuffer);
        delete[] buffer_;
        buffer_ = tmpBuffer;
    }

    buffer_[size_] = item;
    siftUp(size_);
    size_++;
}

template <class T, class Compare>
void Heap<T, Compare>::siftUp(size_t index) {
    int parent = (index - 1) / 2;
    while (0 < index && cmp_(buffer_[index], buffer_[parent])) {
        std::swap(buffer_[index], buffer_[parent]);

        index = parent;
        parent = (index - 1) / 2;
    }
}

template <class T, class Compare>
void Heap<T, Compare>::siftDown(size_t index) {
    int parent = index;
    int child = 2 * index + 1;
    while (child < size_) {
        if ((child + 1 < size_) && cmp_(buffer_[child + 1], buffer_[child])) {
            child++;
        }

        if (cmp_(buffer_[child], buffer_[parent])) {
            std::swap(buffer_[parent], buffer_[child]);
            parent = child;
            child = 2 * parent + 1;
        } else {
            break;
        }
    }
}

template <class T, class Compare>
T Heap<T, Compare>::ExtractMin() {
    std::swap(buffer_[0], buffer_[size_ - 1]);
    size_--;
    T minElement = buffer_[size_];
    siftDown(0);
    return minElement;
}

template <class R>
void MergeArrays<R>::Merge(R** arrayOfArrays, R* arraysLength, R arraysCount, R* result) {
    size_t countOfElements = 0;
    CompareByValue<Element> cmp;
    Heap<Element, CompareByValue<Element>> heap(arraysCount, cmp);
    for (size_t i = 0; i < arraysCount; ++i) {
        countOfElements += arraysLength[i];
        if (arraysLength[i] > 0) {
            heap.Add(Element(arrayOfArrays[i][0], 0, i));
        }
    }

    for (size_t i = 0; i < countOfElements; ++i) {
        Element smallestElem = heap.ExtractMin();
        result[i] = smallestElem.value_;
        if (smallestElem.valueIndex_ + 1 < arraysLength[smallestElem.arrayIndex_]) {
            heap.Add(Element(arrayOfArrays[smallestElem.arrayIndex_][smallestElem.valueIndex_ + 1],
                             smallestElem.valueIndex_ + 1, smallestElem.arrayIndex_));
        }
    }
}

int main() {
    int arraysCount;
    std::cin >> arraysCount;

    int sumOfElements = 0;
    int* arraysLength = new int[arraysCount];
    int** arrayOfArrays = new int* [arraysCount];
    for (size_t i = 0; i < arraysCount; ++i) {
        std::cin >> arraysLength[i];
        arrayOfArrays[i] = new int[arraysLength[i]];
        sumOfElements += arraysLength[i];
        for (size_t j = 0; j < arraysLength[i]; ++j) {
            std::cin >> arrayOfArrays[i][j];
        }
    }

    int* result = new int[sumOfElements];
    MergeArrays<int> merge;
    merge.Merge(arrayOfArrays, arraysLength, arraysCount, result);

    for (size_t i = 0; i < sumOfElements; ++i) {
        std::cout << result[i] << " ";
    }

    for (size_t i = 0; i < arraysCount; ++i) {
        delete[] arrayOfArrays[i];
    }
    delete[] arrayOfArrays;
    delete[] arraysLength;
    delete[] result;
    return 0;
}

