//
// Created by Andrei on 05.10.21.
//

#include <AndreiUtils/classes/CrossBilateralFilter.hpp>
#include <AndreiUtils/classes/LinearInterpolator.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/classes/PythonInterface.h>
#include <AndreiUtils/classes/RandomNumberGenerator.hpp>
#include <AndreiUtils/classes/SlerpInterpolator.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/classes/TypeCreator.hpp>
#include <AndreiUtils/classes/TypeHelper.hpp>
#include <AndreiUtils/classes/UnionFind.hpp>
#include <AndreiUtils/classes/graph/BFS.hpp>
#include <AndreiUtils/classes/graph/DFS.hpp>
#include <AndreiUtils/json.hpp>
#include <AndreiUtils/traits/Container2DEigen.hpp>
#include <AndreiUtils/traits/is_hashable.hpp>
#include <AndreiUtils/traits/median_computer_eigen.hpp>
#include <AndreiUtils/utilsEigenOpenCV.h>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsOpenCV.h>
#include <AndreiUtils/utilsOpenCV.hpp>
#include <AndreiUtils/utilsTime.h>
#include <iostream>
// #include <librealsense2/rs.hpp>

#ifdef WITH_OPENMP

#include <AndreiUtils/utilsOpenMP.hpp>

#endif

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

class Test {
public:
    explicit Test(R<int> data) : a(&data) {}

    int *a;
};

void eigenTesting() {
    Matrix2i m = Matrix2i::Identity();
    m(0, 1) = 2;
    nlohmann::json j = m;
    cout << "Json from matrix m " << endl << m << endl << j << endl;
    Matrix2i m2 = j.get<MatrixXi>();
    cout << "Reconstructed matrix:" << endl << m2 << endl;

    Vector3f v(5, 3.6, 21);
    j = v;
    cout << "Json from vector v " << endl << v << endl << j << endl;
    Vector3f v2 = j.get<Vector3f>();
    cout << "Reconstructed vector:" << endl << v2 << endl;

    cv::FileStorage wfs("tmp.xml", cv::FileStorage::WRITE);
    m(1, 0) = 3;
    wfs << "matrix" << m;
    wfs.release();
    cv::FileStorage rfs("tmp.xml", cv::FileStorage::READ);
    MatrixXi m3;
    rfs["matrix"] >> m3;
    rfs.release();
    cout << "Reconstructed matrix:" << endl << m3 << endl;
}

void fileTesting() {
    cout << getCurrentDirectory(true) << endl;
}

void realsenseDistortionString() {
    /*
    cout << rs2_distortion_to_string(RS2_DISTORTION_NONE) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_MODIFIED_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_INVERSE_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_FTHETA) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_BROWN_CONRADY) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_KANNALA_BRANDT4) << endl;
    cout << rs2_distortion_to_string(RS2_DISTORTION_COUNT) << endl;
    //*/
}

void realsenseToCameraIntrinsicParameters() {
    ;
}

void testPointerReference() {
    int b = 5;
    auto t = Test(b);
    cout << "b = " << b << "; t = " << *t.a << endl;
    b = 4;
    cout << "b = " << b << "; t = " << *t.a << endl;
}

void testJsonNull() {
    nlohmann::json j = nullptr;
    cout << "JSON CONTENT: " << j.dump() << endl;
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

    SlidingWindow<Eigen::Vector3d> swEigen(11);
    for (int i = 0; i < 20; i++) {
        swEigen.addData(Vector3d((double) i, (double) (i * i), (double) (i * i * i)));
        cout << "At i = " << i << ": median = " << swEigen.getMedian() << ", average = " << swEigen.getAverage()
             << endl;
    }
}

void testCrossBilateralFilter() {
    cout << ModifiableContainer2D<int>::isContainer2D << endl;
    cout << ModifiableContainer2D<double>::isContainer2D << endl;
    cout << ModifiableContainer2D<cv::Mat>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::Matrix3d>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::MatrixXf>::isContainer2D << endl;
    cout << ModifiableContainer2D<Eigen::Vector2d>::isContainer2D << endl;
    Eigen::MatrixXd m = Eigen::MatrixXd::Identity(21, 21);
    CrossBilateralFilter filter(5);
    float x, y;
    filter.filter(11, 11, m, x, y);
    cout << "x = " << x << ", y = " << y << endl;
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
    TypeCreator<A_> creator;
    creator.registerTypeCreator("A", []() { return new A_(); });
    creator.registerTypeCreator("B", []() { return new B_(); });
    creator.registerTypeCreator("C", []() { return new C_(); });
    creator.registerTypeCreator("D", []() { return new D_(); });
    auto c = creator.createType("C");
    auto a = dynamic_cast<A_ *>(c);
    auto b = dynamic_cast<B_ *>(c);
    auto d = dynamic_cast<D_ *>(c);
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    delete c;
}

void testJsonArraySerialization() {
    nlohmann::json j = readJsonFile("../testJsonOutput.json");
    j["data"] = {110, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    j["name"] = "dummy data 2";
    j["isDummy"] = false;
    writeJsonFile("../testJsonOutput.json", j);
}

void testIntegralAndUnsignedTypes() {
    cout << is_integral<bool>::value << endl;
    cout << is_unsigned<bool>::value << endl;
    cout << endl;
    cout << is_integral<long long>::value << endl;
    cout << is_integral<int64_t>::value << endl;
    cout << endl;
    cout << is_integral<unsigned long long>::value << endl;
    cout << is_integral<uint64_t>::value << endl;
    cout << endl;
    cout << is_unsigned<long long>::value << endl;
    cout << is_unsigned<int64_t>::value << endl;
    cout << endl;
    cout << is_unsigned<unsigned long long>::value << endl;
    cout << is_unsigned<uint64_t>::value << endl;
    cout << endl;
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

void testOpenCVMatrixAccessors() {
    cv::Mat m1(10, 10, CV_64F);
    m1.setTo(1);
    auto getElement = getMatrixElementAccessor<double, int>(m1, 10);
    cout << getElement(10, 10) << endl;
    cout << getElement(4, 10) << endl;
    cout << getElement(1, 1) << endl;
    cout << getElement(5, 5) << endl;
    cout << getElement(1, -1) << endl;
    cout << getElement(1, -1) << endl;
    cout << endl;

    cv::Mat *m2 = &m1;
    m1.setTo(4);
    cout << "m2 = " << m2 << endl;
    auto getElementPtr = getMatrixElementAccessor<double, int>(m2, 14);
    cout << getElementPtr(10, 10) << endl;
    cout << getElementPtr(4, 10) << endl;
    cout << getElementPtr(1, 1) << endl;
    cout << getElementPtr(5, 5) << endl;
    cout << getElementPtr(1, -1) << endl;
    cout << getElementPtr(1, -1) << endl;
    cout << endl;

    auto *m3 = new cv::Mat(10, 10, CV_64F);
    m3->setTo(64);
    cout << "m3 = " << m3 << endl;
    auto getElementPtr2 = getMatrixElementAccessor<double, int>(m3, 19);
    cout << getElementPtr2(10, 10) << endl;
    cout << getElementPtr2(4, 10) << endl;
    cout << getElementPtr2(1, 1) << endl;
    cout << getElementPtr2(5, 5) << endl;
    cout << getElementPtr2(1, -1) << endl;
    cout << getElementPtr2(1, -1) << endl;
    cout << endl;
    delete m3;

    const auto *m4 = new cv::Mat(10, 10, CV_64F);
    cout << "m4 = " << m4 << endl;
    auto getElementPtr3 = getMatrixElementAccessor<double, int>(m4, 69);
    cout << getElementPtr3(10, 10) << endl;
    cout << getElementPtr3(4, 10) << endl;
    cout << getElementPtr3(1, 1) << endl;
    cout << getElementPtr3(5, 5) << endl;
    cout << getElementPtr3(1, -1) << endl;
    cout << getElementPtr3(1, -1) << endl;
    cout << endl;

    auto const *const &m5 = m4;
    cout << "m5 = " << m5 << endl;
    auto getElementPtr4 = getMatrixElementAccessor<double, float>(m5, 2.4);
    cout << getElementPtr4(10, 10) << endl;
    cout << getElementPtr4(4, 10) << endl;
    cout << getElementPtr4(1, 1) << endl;
    cout << getElementPtr4(5, 5) << endl;
    cout << getElementPtr4(1, -1) << endl;
    cout << getElementPtr4(1, -1) << endl;
    cout << endl;
    delete m4;
}

#ifdef WITH_OPENMP

void testOMPUtils() {
    cout << "Default settings:" << endl;
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), maxNumberOfOMPThreads());
    #pragma omp parallel default(none)
    {
        #pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
    #pragma omp parallel default(none) num_threads(3)
    {
        #pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
    cout << endl;

    cout << "After setting #omp threads to 12:" << endl;
    setNumberOfOMPThreads(12);
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), maxNumberOfOMPThreads());
    #pragma omp parallel default(none)
    {
        #pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
    #pragma omp parallel default(none) num_threads(3)
    {
        #pragma omp single
        printf("num_threads = %d (out of %d)\n", getNumberOfActiveOMPThreads(), omp_get_max_threads());
    }
}

void testFastForLoop() {
    vector<int> v(4 * maxNumberOfOMPThreads());
    printVector(v);
    fastForLoop<int>(v, [](int threadID, vector<int> &_v, size_t index, size_t) {
        _v[index] = threadID;
    });
    printVector(v);
    setNumberOfOMPThreads(16);
    fastForLoop<int>(v, [](int threadID, vector<int> &_v, size_t index, size_t) {
        _v[index] = threadID;
    });
    printVector(v);
}

#endif

void testPrintingImagesOpenCV() {
    cv::Mat m = cv::Mat(300, 300, CV_8U);
    m.setTo(100);
    displayImage(m, "GrayScale");
    for (int i = 0; i < 4; i++) {
        for (int v = 0; v < 256; v++) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(2);
        }
        for (int v = 255; v >= 0; v--) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(2);
        }
    }
    cout << "Finished visualization!" << endl;
    cv::waitKey();
    cout << "END PROGRAM!" << endl;
}

void testOpenCVMatrixCropReference() {
    cv::Mat m = cv::Mat(300, 300, CV_8U);
    m.setTo(100);

    for (int i = 0; i < 4; i++) {
        for (int v = 0; v < 256; v++) {
            cv::Mat view = m(cv::Range(100, 200), cv::Range(100, 200));
            view.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(10);
        }
        for (int v = 255; v >= 0; v--) {
            m.setTo(v);
            displayImage(m, "GrayScale");
            cv::waitKey(10);
        }
    }
}

void testMixedDataContainer() {
    nlohmann::json x;
    x["24"] = 25;
    MixedDataContainer c;
    c.addData("json", &x);
    auto tmp = *(c.getData<nlohmann::json>("json"));
    cout << tmp.dump() << endl;
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
    SlerpInterpolator<double> sx;
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 101, true, true);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 100, true, false);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 100, false, true);
    printVector(sx.getResult());
    sx.compute(Quaterniond(1, 0, 0, 0), Quaterniond(0, 0, 0, 1), 99, false, false);
    printVector(sx.getResult());
}

void testPythonInterface() {
    PythonInterface p("hello_world", {"print_hello", "return_hello"});
    p.callFunction("print_hello");
    auto res = p.callFunction("return_hello");
    cout << res.cast<string>() << endl;
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

void testInstanceOf() {
    B_ b;
    int _tmp = 1;
    Test e(_tmp);
    A_ *a = &b;
    A_ c;

    cout << "A is polymorphic? " << is_polymorphic<A_>::value << endl;
    cout << "B is polymorphic? " << is_polymorphic<B_>::value << endl;
    cout << "Test is polymorphic? " << is_polymorphic<Test>::value << endl;
    cout << "A* is polymorphic? " << is_polymorphic<A_ *>::value << endl;
    cout << "B* is polymorphic? " << is_polymorphic<B_ *>::value << endl;
    cout << "Test* is polymorphic? " << is_polymorphic<Test *>::value << endl;

    cout << "a instance of A?: " << instanceOf<A_>(a) << endl;
    cout << "b instance of A?: " << instanceOf<A_>(b) << endl;
    cout << "c instance of A?: " << instanceOf<A_>(c) << endl;
    cout << "e instance of A?: " << instanceOf<A_>(e) << endl;
    cout << "a instance of B?: " << instanceOf<B_>(a) << endl;
    cout << "b instance of B?: " << instanceOf<B_>(b) << endl;
    cout << "c instance of B?: " << instanceOf<B_>(c) << endl;
    cout << "e instance of B?: " << instanceOf<B_>(e) << endl;
    cout << "a instance of Test?: " << instanceOf<Test>(a) << endl;
    cout << "b instance of Test?: " << instanceOf<Test>(b) << endl;
    cout << "c instance of Test?: " << instanceOf<Test>(c) << endl;
    cout << "e instance of Test?: " << instanceOf<Test>(e) << endl;
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
    CPU<int> x17 = std::move(aPtr);
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

int main() {
    cout << "Hello World!" << endl;

    // eigenTesting();
    // fileTesting();
    // realsenseDistortionString();
    // testPointerReference();
    // testJsonNull();
    // testLambdaCaptureScope();
    // testStringAllocation();
    // testFloatSlidingWindow();
    // testCrossBilateralFilter();
    // testTypeCreator();
    // testJsonArraySerialization();
    // testIntegralAndUnsignedTypes();
    // testUnionFind();
    // testHashable();
    // testDynamicCast();
    // testOpenCVMatrixAccessors();
    // testOMPUtils();
    // testPrintingImagesOpenCV();
    // testFastForLoop();
    // testOpenCVMatrixCropReference();
    // testMixedDataContainer();
    // testGraph();
    // testRandom();
    // testInterpolation();
    // testPythonInterface();
    // testStringFindFunctions();
    // testInstanceOf();
    testTypes();

    return 0;
}
