#include <iostream>


void readArrayFromUser(int * numbersFromUser, int lenOfArray);
int exponetialSearch(int * A, int k, int n);
int binarySearch(int * A, int k, int leftPart, int rightPart, int n);
void getAnswer(int * resultOfSearch, int * A, int * B, int n, int m);
void printAnswer(int * resultOfSearch, int m);

int main() {
    int n, m;
    std::cin >> n >> m;

    int * A = new int [n];
    readArrayFromUser(A, n);

    int * B = new int [m];
    readArrayFromUser(B, m);

    int * resultOfSearch = new int [m];
    getAnswer(resultOfSearch, A, B, n, m);
    printAnswer(resultOfSearch, m);

    delete [] A;
    delete [] B;
    delete [] resultOfSearch;

    return 0;
}

void readArrayFromUser(int * numbersFromUser, int lenOfArray) {
    for (int i = 0; i < lenOfArray; i++) {
        std::cin >> numbersFromUser[i];
    }
}

int exponetialSearch(int * A, int k , int n) {
    if (A[0] == k) {
        return 0;
    }
    int currentDegree = 1;
    while (currentDegree < n && A[currentDegree] < k) {
        currentDegree = currentDegree * 2;
    }
    if (currentDegree > n) {
        currentDegree = n;
    }
    return binarySearch(A, k, currentDegree/2, currentDegree, n);
}

int binarySearch(int * A, int k, int leftPart, int rightPart, int n) {
    while (leftPart < rightPart) {
        int midPart = (leftPart + rightPart) / 2;
        if (A[midPart] < k) {
            leftPart = midPart + 1;
        } else {
            rightPart = midPart;
        }
    }
    return (leftPart == n || A[leftPart] < k) ? n : leftPart;
}

void getAnswer(int * resultOfSearch, int * A, int * B, int n, int m) {
    for (size_t i = 0; i < m; ++i) {
        resultOfSearch[i] = exponetialSearch(A, B[i], n);
    }
}

void printAnswer(int * resultOfSearch, int m) {
    for (size_t i = 0; i < m; ++i) {
        std::cout << resultOfSearch[i] << " ";
    }
    std::cout << std::endl;
}
