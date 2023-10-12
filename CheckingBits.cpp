#include <iostream>


bool checkingBits(size_t inputDigit);

int main() {
    size_t inputDigit;
    std::cin >> inputDigit;

    int answer = checkingBits(inputDigit);
    if (answer) {
        std::cout << "OK";
    } else {
        std::cout << "FAIL";
    }
    return 0;
}

bool checkingBits(size_t inputDigit) {
    int i = 0;
    while (inputDigit != 0) {
        if ((inputDigit & 1) == 1) {
            ++i;
        }
        inputDigit >>= 1;
    }

    if (i == 1) {
        return true;
    } else {
        return false;
    }
}