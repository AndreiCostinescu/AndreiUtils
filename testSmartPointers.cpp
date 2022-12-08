//
// Created by Andrei on 03.11.22.
//

#include <iostream>
#include <memory>

using namespace std;

class A {
public:
    explicit A(string name) : name(std::move(name)) {}

    ~A() {
        cout << "In A's destructor for " << this->name << "!" << endl;
    }

    string name;
};

void print(A *a) {
    cout << a->name << endl;
}

int main() {
    cout << "Hello World!" << endl;

    std::unique_ptr<A>(new A("1"));
    cout << "After temporary object..." << endl;
    std::unique_ptr<A> a(new A("2"));
    cout << "After a object..." << endl;
    a.reset(new A("3"));
    cout << "After a reset..." << endl;

    print(make_shared<A>("213").get());
    A b("4");
    print(&b);

    auto c = make_shared<A>("1234");
    cout << c.use_count() << endl;
    auto d = c;
    cout << c.use_count() << endl;
    cout << d.use_count() << endl;

    d.reset();
    cout << c.use_count() << endl;
    cout << d.use_count() << endl;

    return 0;
}