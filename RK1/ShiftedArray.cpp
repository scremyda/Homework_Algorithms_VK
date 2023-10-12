#include <iostream>


int binarySearch(const int* numbers, int n);

int main() {
    int n;
    std::cin >> n;

    int* numbers = new int [n];
    for(size_t i = 0; i < n; ++i){
        std::cin >> numbers[i];
    }

    std::cout << binarySearch(numbers, n);

    delete[] numbers;
    return 0;
}

int binarySearch(const int* numbers, int n) {
    int first = 0;
    int last = n - 1;
    while (first < last) {
        int mid = (first + last) / 2;
        if (numbers[mid] > numbers[last]) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}