//
// Created by Andrei on 03.11.22.
//

#include <iostream>

using namespace std;

template<typename T>
class E {
public:
    explicit E(T *data = nullptr) : data(data), ownsData(false) {}

    explicit E(T &&data) : data(new T(std::move(data))), ownsData(true) {}

    explicit E(T &data) = delete;

    E(E const &other) : data(other.data), ownsData(false) {}

    E(E &&other) noexcept: data(other.data), ownsData(other.ownsData) {
        other.ownsData = false;
        other.reset();
    }

    E &operator=(E const &other) {
        if (&other != this) {
            this->discardData();
            this->data = other.data;
            this->ownsData = false;
        }
        return *this;
    }

    E &operator=(E &&other) noexcept {
        if (&other != this) {
            this->discardData();
            this->data = other.data;
            this->ownsData = other.ownsData;
            other.ownsData = false;
            other.reset();
        }
        return *this;
    }

    virtual ~E() {
        this->reset();
    }

    void reset() {
        this->discardData();
        this->ownsData = false;
        this->data = nullptr;
    }

    void discardData() {
        if (this->ownsData) {
            cout << "Call delete!" << endl;
            delete this->data;
            this->data = nullptr;
            this->ownsData = false;
        }
    }

    T *data;
    bool ownsData;
};

template<typename T>
void printRValueData(E<T> &&v) {
    cout << "v.data = " << v.data << "; owns it = " << v.ownsData;
    if (v.data != nullptr) {
        cout << "; with value = " << *(v.data);
    }
    cout << endl;
}

template<typename T>
void printRValueData(E<T> &v) = delete;

template<typename T>
void allowOnlyRValues(T &&v) {
    printRValueData(std::forward<T>(v));
    printRValueData(std::forward<T>(v));
}

template<typename T>
void allowOnlyRValues(T &v) = delete;

void testMoveSemantics() {
    E<int> a(42);
    printRValueData(std::move(a));
    cout << endl;
    E<int> b = std::move(a);  // move assignment
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    cout << endl;
    E<int> c(std::move(b));  // move constructor
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    cout << endl;
    E<int> d = c;  // copy assignment
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    cout << endl;
    E<int> e(c);  // copy constructor from c
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    printRValueData(std::move(e));
    cout << endl;
    E<int> f(d);  // copy constructor from d
    printRValueData(std::move(a));
    printRValueData(std::move(b));
    printRValueData(std::move(c));
    printRValueData(std::move(d));
    printRValueData(std::move(e));
    printRValueData(std::move(f));
    cout << endl;
    allowOnlyRValues(E<int>(69));
}

int main() {
    cout << "Hello World!" << endl;

    testMoveSemantics();

    return 0;
}