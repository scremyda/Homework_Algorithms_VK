#include <iostream>


template <class T>
class IsLessNotStrict {
 public:
    bool operator()(const T& left, const T& right) {
        return left <= right;
    }
};

template <class Compare>
int medianOfThree(int* arr, int left, int mid, int right, Compare cmp);

template <class Compare>
int partition(int* arr, int left, int right, Compare cmp);

template <class Compare>
int findKStat(int* arr, int n, int k, Compare cmp);

int main() {
    int n;
    std::cin >> n;
    
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    IsLessNotStrict<int> cmp;
    std::cout << findKStat(arr, n, n / 10, cmp) << std::endl;
    std::cout << findKStat(arr, n, n / 2, cmp) << std::endl;
    std::cout << findKStat(arr, n, n * 9 / 10, cmp) << std::endl;

    delete[] arr;
    return 0;
}

template <class Compare>
int medianOfThree(int* arr, int left, int mid, int right, Compare cmp) {
    if ((cmp(arr[left], arr[mid])) && (cmp(arr[mid], arr[right])) ||
        (cmp(arr[right], arr[mid]) && cmp(arr[mid], arr[left]))) {
        return mid;
    } else if ((cmp(arr[mid], arr[right]) && cmp(arr[right], arr[left])) ||
               (cmp(arr[left], arr[right]), cmp(arr[right], arr[mid]))) {
        return right;
    } else {
        return left;
    }
}

template <class Compare>
int partition(int* arr, int left, int right, Compare cmp) {
    int pivotPosition = medianOfThree(arr, left, (right + left)/ 2, right, cmp);

    std::swap(arr[pivotPosition], arr[left]);

    int i = right;
    int j = right;
    while (j > 0) {
        if (!(cmp(arr[j], arr[left]))) {
            std::swap(arr[j], arr[i--]);
        }
        j--;
    }
    std::swap(arr[i], arr[left]);

    return i;
}

template <class Compare>
int findKStat(int* arr, int n, int k, Compare cmp) {
    int left = 0;
    int right = n - 1;
    int pivotPos = partition(arr, left, right, cmp);

    while (pivotPos != k) {
        if (k < pivotPos) {
            right = pivotPos - 1;
        } else {
            left = pivotPos + 1;
        }
        pivotPos = partition(arr, left, right, cmp);
    }

    return arr[pivotPos];
}
