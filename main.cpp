//
// Created by Andrei on 05.10.21.
//

#include <AndreiUtils/classes/AnyType.h>
#include <AndreiUtils/classes/Interval.hpp>
#include <AndreiUtils/classes/LinearInterpolator.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/classes/PriorityQueue.hpp>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/classes/TypeCreator.hpp>
#include <AndreiUtils/classes/TypeHelper.hpp>
#include <AndreiUtils/classes/UnionFind.hpp>
#include <AndreiUtils/classes/UserInteraction.hpp>
#include <AndreiUtils/classes/graph/BFS.hpp>
#include <AndreiUtils/classes/graph/DFS.hpp>
#include <AndreiUtils/traits/is_hashable.hpp>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsMap.hpp>
#include <functional>
#include <iostream>
#include <queue>

using namespace AndreiUtils;
using namespace std;

class Test {
public:
    explicit Test(R<int> data) : a(&data) {}

    void f1() const {
        cout << "Called Test::f1" << endl;
    }

    void f2(int const &x) const {
        cout << "Called Test::f2" << endl;
    }

    void f3(int const &x, Test const &other) const {
        cout << "Called Test::f3" << endl;
    }

    [[nodiscard]] double f4(int const &x, Test const &other) const {
        cout << "Called Test::f4" << endl;
        return 0;
    }

    int *a;
};

void fileTesting() {
    cout << getCurrentDirectory(true) << endl;
}

void testPointerReference() {
    int b = 5;
    auto t = Test(b);
    cout << "b = " << b << "; t = " << *t.a << endl;
    b = 4;
    cout << "b = " << b << "; t = " << *t.a << endl;
}

function<void()> testLambdaCaptureScopeFunction() {
    int x = 42;
    return [x]() {
        cout << "x = " << x << endl;
    };
}

void testLambdaCaptureScope() {
    auto f = testLambdaCaptureScopeFunction();
    f();
}

void testStringAllocation() {
    char *data = new char[100];
    memcpy(data, "This is a house!", 17);
    string s(data);
    data[2] = 'a';
    data[3] = 't';
    delete[] data;
    cout << s << endl;
    s[15] = '?';
    cout << s << endl;
}

void testFloatSlidingWindow() {
    SlidingWindow<float> sw(10);
    for (int i = 0; i < 20; i++) {
        for (const auto &v: sw) {
            cout << v << " || ";
        }
        cout << endl;
        sw.addData((float) (i * i));
        const auto &swRef = sw;
        for (const auto &v: swRef) {
            cout << v << " || ";
        }
        cout << endl;
        cout << "At i = " << i << ": median = " << sw.getMedian() << ", average = " << sw.getAverage() << endl;
    }
}

class A_ {
public:
    virtual ~A_() = default;
};

class B_ : virtual public A_ {
public:
    bool operator<(CR<B_> other) const {
        return true;
    }
};

class C_ : virtual public A_ {
};

class D_ : public B_, public C_ {
};

void testTypeCreator() {
    SharedTypeCreator<A_> creator;
    creator.registerTypeCreator("A", []() { return make_shared<A_>(); });
    creator.registerTypeCreator("B", []() { return make_shared<B_>(); });
    creator.registerTypeCreator("C", []() { return make_shared<C_>(); });
    creator.registerTypeCreator("D", []() { return make_shared<D_>(); });
    auto c = creator.createType("C");
    auto a = dynamic_pointer_cast<A_>(c);
    auto b = dynamic_pointer_cast<B_>(c);
    auto d = dynamic_pointer_cast<D_>(c);
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    c.reset();
}

void testUnionFind() {
    UnionFind x;
    assert(x.numberOfDistinctComponents() == 0);
    x.add();
    assert(x.find(0) == 0);
    assert(x.find(0, 0));
    assert(x.numberOfDistinctComponents() == 1);
    x.add();
    assert(x.find(0) == 0);
    assert(x.find(1) == 1);
    assert(!x.find(0, 1));
    assert(x.find(1, 1));
    assert(x.numberOfDistinctComponents() == 2);
    assert(x.size() == 2);
    x.unite(1, 1);
    assert(x.find(0) == 0);
    assert(x.find(1) == 1);
    assert(x.numberOfDistinctComponents() == 2);
    assert(x.size() == 2);
    try {
        x.unite(1, 2);
        assert(false);
    } catch (exception &e) {
        if (string(e.what()) != "Index 2 out of bounds (2)!") {
            throw e;
        }
    }
    x.add();
    assert(x.numberOfDistinctComponents() == 3);
    assert(x.size() == 3);
    assert(x.find(0) == 0);
    assert(x.find(1) == 1);
    assert(x.find(2) == 2);
    assert(!x.find(0, 1));
    assert(!x.find(1, 2));
    assert(x.find(2, 2));
    x.unite(1, 2);
    assert(x.numberOfDistinctComponents() == 2);
    assert(x.size() == 3);
    assert(x.find(0) == 0);
    assert(x.find(1) == 1);
    assert(x.find(2) == 1);
    assert(!x.find(0, 1));
    assert(x.find(1, 2));
    assert(!x.find(0, 2));
    x.unite(2, 1);
    assert(x.numberOfDistinctComponents() == 2);
    assert(x.size() == 3);
    assert(x.find(0) == 0);
    assert(x.find(1) == 1);
    assert(x.find(2) == 1);
    assert(!x.find(0, 1));
    assert(x.find(1, 2));
    assert(!x.find(0, 2));
    x.unite(2, 0);
    assert(x.numberOfDistinctComponents() == 1);
    assert(x.size() == 3);
    assert(x.find(0) == 1);
    assert(x.find(1) == 1);
    assert(x.find(2) == 1);
    assert(x.find(0, 1));
    assert(x.find(1, 2));
    assert(x.find(0, 2));

    UnionFindWithValues<int> y;
    assert(y.numberOfDistinctComponents() == 0);
    y.addElem(0);
    assert(y.findByElem(0, 0));
    assert(y.numberOfDistinctComponents() == 1);
    y.addElem(1);
    assert(!y.findByElem(0, 1));
    assert(y.findByElem(1, 1));
    assert(y.numberOfDistinctComponents() == 2);
    assert(y.size() == 2);
    y.unite(1, 1);
    assert(y.numberOfDistinctComponents() == 2);
    assert(y.size() == 2);
    try {
        y.uniteByElem(1, 2);
        assert(false);
    } catch (exception &e) {
        if (string(e.what()) != "Element not found in map!") {
            throw e;
        }
    }
    y.addElem(2);
    assert(y.numberOfDistinctComponents() == 3);
    assert(y.size() == 3);
    assert(!y.findByElem(0, 1));
    assert(!y.findByElem(1, 2));
    assert(y.findByElem(2, 2));
    y.uniteByElem(1, 2);
    assert(y.numberOfDistinctComponents() == 2);
    assert(y.size() == 3);
    assert(!y.findByElem(0, 1));
    assert(y.findByElem(1, 2));
    assert(!y.findByElem(0, 2));
    y.uniteByElem(2, 1);
    assert(y.numberOfDistinctComponents() == 2);
    assert(y.size() == 3);
    assert(!y.findByElem(0, 1));
    assert(y.findByElem(1, 2));
    assert(!y.findByElem(0, 2));
    y.uniteByElem(2, 0);
    assert(y.numberOfDistinctComponents() == 1);
    assert(y.size() == 3);
    assert(y.findByElem(0, 1));
    assert(y.findByElem(1, 2));
    assert(y.findByElem(0, 2));
}

namespace std {
    template<>
    struct hash<B_> {
        size_t operator()(CR<B_> b) const noexcept {
            return 0;
        }
    };
}

void testHashable() {
    auto x = std::hash<int>();
    auto y = std::hash<float>();
    auto z = std::hash<double>();
    for (int i = 0; i < 10; i++) {
        cout << "Hash value of " << i << " is " << x(i) << endl;
    }
    for (int i = 0; i < 10; i++) {
        cout << "Hash value of " << i << " is " << x(i) << endl;
    }
    for (float i = 0; i < 10; i += 0.5) {
        cout << "Hash value of " << i << " is " << y(i) << endl;
    }
    for (double i = 0; i < 10; i += 0.5) {
        cout << "Hash value of " << i << " is " << z(i) << endl;
    }

    cout << std::is_default_constructible<std::hash<int>>::value << endl;
    cout << std::is_default_constructible<std::hash<float>>::value << endl;
    cout << std::is_default_constructible<std::hash<double>>::value << endl;
    cout << std::is_default_constructible<std::hash<B_>>::value << endl;
    cout << std::is_default_constructible<std::hash<C_>>::value << endl;

    cout << is_hashable<int>::value << endl;
    cout << is_hashable<float>::value << endl;
    cout << is_hashable<double>::value << endl;
    cout << is_hashable<B_>::value << endl;
    cout << is_hashable<C_>::value << endl;

    auto b = std::hash<B_>();
    // auto c = std::hash<C_>();  // Compiler error!

    map<B_, int> mB;
    map<C_, int> mC;
    B_ bObj1, bObj2, bObj3;
    C_ cObj1, cObj2, cObj3;

    // is_detected<std::less<int>::operator()>;
    // is_detected<std::less<int>::operator(), int>;
    // is_detected<std::less<C>::operator(), C, C>;

    cout << is_hashable<map<B_, int>::key_compare>::value << endl;

    mB[bObj1] = 1;
    mB[bObj2] = 2;
    mB[bObj3] = 3;
    // mC[cObj1] = 1;  // Compiler error
    // mC[cObj2] = 2;  // Compiler error
    // mC[cObj3] = 3;  // Compiler error
}

void testDynamicCast() {
    A_ objA;
    B_ objB;
    C_ objC;
    D_ objD;
    auto *a = &objA;
    auto *b = &objB;
    auto *c = &objC;
    auto *d = &objD;

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    cout << endl;

    A_ *tmp;
    tmp = dynamic_cast<A_ *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<A_ *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<A_ *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<A_ *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<B_ *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<B_ *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<B_ *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<B_ *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<C_ *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<C_ *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<C_ *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<C_ *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<D_ *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<D_ *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<D_ *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<D_ *>(d);
    cout << tmp << endl;
    cout << endl;

    // delete a;
    // delete b;
    // delete c;
    // delete d;
}

void testMixedDataContainer() {
    vector<string> x(5, "Hello World!");
    MixedDataContainer c;
    int data = 25;
    c.addData("24", &data);
    c.addData("json", &x);
    auto tmp = *(c.getData<vector<string>>("json"));
    cout << printVectorToString(tmp) << endl;
}

void testGraph() {
    auto edgeIdFromNodes = [](int const &n1, int const &n2) { return to_string(n1) + "->" + to_string(n2); };
    Graph<int, string> g;
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addNode(4);
    g.addNode(5);
    g.addNode(6);
    g.addNode(7);
    g.addNode(8);
    g.addNode(9);
    g.addNode(10);
    g.addEdge(2, 1, edgeIdFromNodes);
    g.addEdge(3, 2, edgeIdFromNodes);
    g.addEdge(4, 3, edgeIdFromNodes);
    g.addEdge(5, 4, edgeIdFromNodes);
    g.addEdge(6, 5, edgeIdFromNodes);
    g.addEdge(1, 3, edgeIdFromNodes);
    g.addEdge(7, 6, edgeIdFromNodes);
    g.addEdge(1, 8, edgeIdFromNodes);
    g.addEdge(8, 9, edgeIdFromNodes);
    g.addEdge(9, 10, edgeIdFromNodes);
    g.addEdge(1, 10, edgeIdFromNodes);

    Timer dfsRecTimer;
    DFS<int, string> dfs(g);
    auto t1 = dfsRecTimer.measure(TimeUnit::SECOND);
    Timer dfsIterTimer;
    DFS<int, string> dfsIter(g, false);
    auto t2 = dfsIterTimer.measure(TimeUnit::SECOND);
    cout << "DFS RecTime: " << t1 << " sec vs. IterTime: " << t2 << " sec" << endl;

    auto printDfsData = [](DFS<int, string> const &_dfs) {
        cout << "Roots: [" << printVectorToString(_dfs.getGraphRoots()) << "]" << endl;
        cout << "Tree edges: [" << printVectorToString(_dfs.getTreeEdges()) << "]" << endl;
        cout << "Forward edges: [" << printVectorToString(_dfs.getForwardEdges()) << "]" << endl;
        cout << "Backward edges: [" << printVectorToString(_dfs.getBackwardEdges()) << "]" << endl;
        cout << "Cross edges: [" << printVectorToString(_dfs.getCrossEdges()) << "]" << endl;
        vector<int> topoSort;
        bool isTopoSort = _dfs.getTopologicalSort(topoSort);
        cout << "TopoSort: ";
        if (isTopoSort) {
            cout << "[" << printVectorToString(topoSort) << "]" << endl;
        } else {
            cout << "None" << endl;
        }
    };
    printDfsData(dfs);
    printDfsData(dfsIter);

    BFS<int, string> bfs(g);
    Timer bfsRecTimer;
    auto res = bfs.bfs();
    auto t3 = bfsRecTimer.measure(TimeUnit::SECOND);
    cout << "BFS " << t3 << " sec" << endl;
    cout << "Roots: [" << printVectorToString(bfs.getGraphRoots()) << "]" << endl;
    cout << "BFS traversal: [" << printVectorToString(bfs.getTraversal()) << "]" << endl;
    for (auto const &rootData: res) {
        cout << "Distances starting from " << rootData.first << ": " << printMapToString(rootData.second) << endl;
    }

    int x = 35;
    cout << (x = 42) << endl;

    g.removeEdge(edgeIdFromNodes(2, 1));
    g.removeNode(1);
}

void testRandom() {
    RandomNumberGenerator<int> randomInt(1, 10);
    for (int i = 0; i < 20; i++) {
        cout << randomInt.sample() << endl;
    }
    cout << endl << endl;
    for (int i = 0; i < 20; i++) {
        cout << double01Sampler.sample() << endl;
    }
    cout << endl << endl;

    class A {
    };
    A a1, a2;
    // RandomNumberGenerator<A> randomA(a1, a2);  // produces compiler error as intended!
}

void testInterpolation() {
    LinearInterpolator<double> x;
    x.compute(0, 10, 101, true, true);
    printVector(x.getResult());
    x.compute(0, 10, 100, true, false);
    printVector(x.getResult());
    x.compute(0, 10, 100, false, true);
    printVector(x.getResult());
    x.compute(0, 10, 99, false, false);
    printVector(x.getResult());
}

void testStringFindFunctions() {
    string a = "abdabdad";
    string b = "a";
    string c = "abd";
    string d = "d";
    cout << a.rfind(b) << endl;
    cout << a.rfind(b, 0) << endl;
    cout << a.rfind(d) << endl;
    cout << a.rfind(d, 0) << endl;
    cout << a.rfind(d, 3) << endl;
    cout << a.rfind(c) << endl;
    cout << a.rfind(c, 0) << endl;
    cout << a.rfind(c, 3) << endl;

    a = "....";
    b = "..";
    cout << a.rfind(b, 4) << endl;
    cout << a.rfind(b, 3) << endl;
    cout << a.rfind(b, 2) << endl;
    cout << a.rfind(b, 1) << endl;
    cout << a.rfind(b, 0) << endl;
}

void testTypes() {
    int a = 24;
    int *aPtr = &a;
    int const *aCPtr = &a;
    int volatile *aVPtr = &a;
    int volatile const *aCVPtr = &a;

    if (std::is_same<CR<CP<V<int>>>, VCPCR<int>>::value) {
        cout << "Types are the same!" << endl;
    }

    C<int> x0 = a;
    P<int> x1 = aPtr;
    R<int> x2 = a;
    U<int> x3 = std::move(a);
    V<int> x4 = a;

    CP<int> x5 = aPtr;
    CR<int> x6 = a;
    CU<int> x7 = std::move(a);
    PC<int> x8 = aPtr;
    PR<int> x9 = aPtr;
    PU<int> x10 = std::move(aPtr);
    VC<int> x11 = a;
    VP<int> x12 = aPtr;
    VR<int> x13 = a;
    VU<int> x14 = std::move(a);

    CPC<int> x15 = aPtr;
    CPR<int> x16 = aCPtr;
    CPU<int> x17 = std::move(aCPtr);
    PCR<int> x18 = aPtr;
    PCU<int> x19 = std::move(aPtr);
    VCP<int> x20 = &a;
    VCR<int> x21 = a;
    VCU<int> x22 = std::move(a);
    VPC<int> x23 = &a;
    VPR<int> x24 = aVPtr;
    VPU<int> x25 = std::move(aVPtr);

    CPCR<int> x26 = aCPtr;
    CPCU<int> x27 = std::move(aCPtr);
    VCPC<int> x28 = &a;
    VCPR<int> x29 = aCVPtr;
    VCPU<int> x30 = std::move(aCVPtr);
    VPCR<int> x31 = aVPtr;
    VPCU<int> x32 = std::move(aVPtr);

    VCPCR<int> x33 = aCVPtr;
    VCPCU<int> x34 = std::move(aCVPtr);
}

void testAnyType() {
    AnyType x;
    x = 24;
    cout << (x.get<int>() == 25) << " " << x.get<int>() << endl;
    try {
        cout << (x.get<string>() == "Hello World!") << endl;
    } catch (runtime_error &e) {
        if (e.what() != string("Wrong type to cast to!")) {
            throw e;
        }
    }
    x = string("Hello World!");
    cout << x.get<string>() << endl;
}

template<typename T>
void testIntervalsPrivate() {
    Interval<T> i(0, 1);
    Interval<T> i2(std::move(Interval<T>::createFullRange()));
    Interval<T> i3(-std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    cout << i.createSampler().sample() << " || " << i2.createSampler().sample() << endl;
    cout << std::numeric_limits<T>::min() << " vs. " << std::numeric_limits<T>::max() << " vs. "
         << -std::numeric_limits<T>::max() << endl;
    cout << (std::numeric_limits<T>::max() + 1) << " vs. " << (-std::numeric_limits<T>::max() - 1) << " vs. "
         << (std::numeric_limits<T>::min() - 1) << endl;
    cout << std::numeric_limits<T>::max() - (-std::numeric_limits<T>::max()) << endl;
    cout << i.size() << " | " << i2.size() << " | " << i3.size() << endl;
    RandomNumberGenerator<T> sampler = i.createSampler();
    RandomNumberGenerator<T> sampler2 = i2.createSampler();
    RandomNumberGenerator<T> sampler3 = i3.createSampler();
    RandomNumberGenerator<T> sampler4(-std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    RandomNumberGenerator<T> sampler5(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    for (int iter = 0; iter < 10; ++iter) {
        cout << sampler.sample() << " || " << sampler2.sample() << " || " << sampler3.sample() << " || "
             << sampler4.sample() << " || " << sampler5.sample() << endl;
    }
}

void testIntervals() {
    testIntervalsPrivate<double>();
    cout << endl << endl;
    testIntervalsPrivate<float>();
    cout << endl << endl;
    testIntervalsPrivate<long long>();
    cout << endl << endl;
    testIntervalsPrivate<int>();
    cout << endl << endl;
}

void testUserInteraction() {
    UserInteraction user;
    user << "Hello World!";
    cout << "User responded with " << user.getBooleanResponse() << endl;

    UserInteraction user1;
    user1 << "Hello World!";
    cout << "User responded with " << user1.getBooleanResponse() << endl;

    UserInteraction user2;
    user2 << "Hello World!" << " and hello again!";
    cout << "User2 responded with " << user2.getBooleanResponse() << endl;
}

void testStoringFunctionsOfDifferentType() {
    std::map<std::string, AnyType> functions;
    int a = 5;
    Test t(a);
    mapEmplace<string>(functions, "f1", std::function<void()>([ObjectPtr = &t] { ObjectPtr->f1(); }));
    mapEmplace<string>(functions, "f2",
                       std::function<void(int const &)>([ObjectPtr = &t](int const &x) { ObjectPtr->f2(x); }));
    mapEmplace<string>(functions, "f3", std::function<void(int const &, Test const &)>(
            [ObjectPtr = &t](int const &x, Test const &ptr) { ObjectPtr->f3(x, ptr); }));
    mapEmplace<string>(functions, "f4", std::function<double(int const &, Test const &)>(
            [ObjectPtr = &t] (int const &x, Test const &ref) { return ObjectPtr->f4(x, ref); }));

    mapGet<string>(functions, "f1").get<std::function<void()>>()();
    mapGet<string>(functions, "f2").get<std::function<void(int const &)>>()(23);
    mapGet<string>(functions, "f3").get<std::function<void(int const &, Test const &)>>()(23, t);
    cout << mapGet<string>(functions, "f4").get<std::function<double(int const &, Test const &)>>()(23, t) << endl;
}

void testStdAlgorithms() {
    std::vector<bool> v1;
    std::vector<bool> v2 = {false, true};
    std::vector<bool> v3 = {true, true};
    std::vector<bool> v4 = {false, false};
    cout << "all of v1: " << std::all_of(v1.begin(), v1.end(), [](auto const &b) { return b; }) << endl;
    cout << "all of v2: " << std::all_of(v2.begin(), v2.end(), [](auto const &b) { return b; }) << endl;
    cout << "all of v3: " << std::all_of(v3.begin(), v3.end(), [](auto const &b) { return b; }) << endl;
    cout << "all of v4: " << std::all_of(v4.begin(), v4.end(), [](auto const &b) { return b; }) << endl;
    cout << "any of v1: " << std::any_of(v1.begin(), v1.end(), [](auto const &b) { return b; }) << endl;
    cout << "any of v2: " << std::any_of(v2.begin(), v2.end(), [](auto const &b) { return b; }) << endl;
    cout << "any of v3: " << std::any_of(v3.begin(), v3.end(), [](auto const &b) { return b; }) << endl;
    cout << "any of v4: " << std::any_of(v4.begin(), v4.end(), [](auto const &b) { return b; }) << endl;
}

void testFactorial() {
    cout << (int) AndreiUtils::factorial(char(5)) << endl;
    cout << AndreiUtils::factorial(short(5)) << endl;
    cout << AndreiUtils::factorial(int(5)) << endl;
    cout << AndreiUtils::factorial(long(5)) << endl;
    cout << AndreiUtils::factorial(long(5)) << endl;
    cout << (int) AndreiUtils::factorial(int8_t(5)) << endl;
    cout << AndreiUtils::factorial(int16_t(5)) << endl;
    cout << AndreiUtils::factorial(int32_t(5)) << endl;
    cout << AndreiUtils::factorial(int64_t(5)) << endl;
    cout << (int) AndreiUtils::factorial(uint8_t(5)) << endl;
    cout << AndreiUtils::factorial(uint16_t(5)) << endl;
    cout << AndreiUtils::factorial(uint32_t(5)) << endl;
    cout << AndreiUtils::factorial(uint64_t(5)) << endl;
}

struct Custom {
    int a, b;
};

struct CustomComparison {
    bool operator()(Custom const &a, Custom const &b) {
        return a.b > b.b;  // lowest priority first
    }
};

void testPriorityQueue() {
    PriorityQueue<int> queue;
    queue.add(5, 5);
    queue.add(4, 4);
    queue.add(3, 3);
    queue.add(1, 2);
    queue.add(2, 1);
    std::priority_queue<Custom, std::vector<Custom>, CustomComparison> q;
    q.emplace(5, 5);
    q.emplace(4, 4);
    q.emplace(3, 3);
    q.emplace(1, 2);
    q.emplace(2, 1);
    while (!queue.empty()) {
        auto const min = queue.extractMin();
        cout << min.first << ": " << min.second << endl;
    }
    cout << endl;
    while (!q.empty()) {
        auto const &min = q.top();
        cout << min.a << ": " << min.b << endl;
        q.pop();
    }
    cout << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // fileTesting();
    // testPointerReference();
    // testLambdaCaptureScope();
    // testStringAllocation();
    // testFloatSlidingWindow();
    // testTypeCreator();
    // testUnionFind();
    // testHashable();
    // testDynamicCast();
    // testMixedDataContainer();
    // testGraph();
    // testRandom();
    // testInterpolation();
    // testStringFindFunctions();
    // testTypes();
    // testAnyType();
    // testIntervals();
    // testUserInteraction();
    // testStoringFunctionsOfDifferentType();
    // testStdAlgorithms();
    // testFactorial();
    testPriorityQueue();

    return 0;
}
