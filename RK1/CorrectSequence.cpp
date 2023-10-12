#include <iostream>
#include <string>
#include <cassert>


class Stack {
public:
    explicit Stack(int bufferSize);
    ~Stack();
    void Push(char element);
    int Pop();
    bool IsEmpty() const;
    char PeekTop();

private:
    char* buffer;
    int bufferSize;
    int top;
};

Stack::Stack( int _bufferSize ) : bufferSize( _bufferSize ), top(-1 ) {
    buffer = new char[bufferSize];
}

Stack::~Stack() {
    delete[] buffer;
}

void Stack::Push(char element) {
    assert(top + 1 < bufferSize);
    buffer[++top] = element;
}

int Stack::Pop() {
    assert(top != -1);
    return buffer[top--];
}

bool Stack::IsEmpty() const {
    return top == -1;
}

char Stack::PeekTop() {
    return buffer[top];
}

int main() {
    std::string line;
    std::cin >> line;

    Stack stack(line.length());
    for (char c : line) {
        if (c == '('){
            stack.Push(c);
        } else if (!stack.IsEmpty() && stack.PeekTop() == '(') {
            stack.Pop();
        } else {
            std::cout << "NO";
            return 0;
        }
    }

    if(stack.IsEmpty()) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}