#include <iostream>

bool pivot(const long long element, const int numBit);
void binaryMSD(long long* array, int left, int right, int numBit);


int main() {
    int n;
    std::cin >> n;

    long long* array = new long long[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }

    binaryMSD(array, 0, n, 63);

    for (int i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }

    delete [] array;

    return 0;
}

bool pivot(const long long element, const int numBit) {
    return (element >> numBit) & 1;
}

void binaryMSD(long long* array, int left, int right, int numBit) {
    if (numBit < 0 || right - left < 2) {
        return;
    }

    int i = left;
    int j = right;
    while (j > i) {
        for (; i < j && !(pivot(array[i], numBit)); ++i) {}
        for (; j > i && pivot(array[j-1], numBit); --j) {}
        if (i < j) {
            std::swap(array[i], array[j-1]);
            ++i;
            --j;
        }
    }

    binaryMSD(array, left, j, numBit - 1);
    binaryMSD(array, i, right, numBit - 1);
}
