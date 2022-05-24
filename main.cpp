//
// Created by andrei on 05.10.21.
//

#include <AndreiUtils/classes/CrossBilateralFilter.hpp>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/classes/TypeCreator.hpp>
#include <AndreiUtils/classes/UnionFind.hpp>
#include <AndreiUtils/json.hpp>
#include <AndreiUtils/traits/Container2DEigen.hpp>
#include <AndreiUtils/traits/get_vector_type_for_convolution_eigen.hpp>
#include <AndreiUtils/traits/is_hashable.hpp>
#include <AndreiUtils/traits/median_computer_eigen.hpp>
#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsEigenOpenCV.h>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsTime.h>
#include <iostream>
#include <librealsense2/rs.hpp>
#include <random>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

class Test {
public:
    explicit Test(int &data) : a(&data) {}

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

void timeTesting() {
    SystemTimePoint t;
    cout << convertChronoToStringWithSubseconds(t) << endl;
    cout << endl << endl << endl;

    auto now = std::chrono::system_clock::now();
    string time;
    time = AndreiUtils::convertChronoToStringWithSubseconds(now, "%Y-%m-%d-%H-%M-%S", "%us-%pns", ":");
    cout << "Initial time = " << time << endl;
    time = AndreiUtils::convertChronoToStringWithSubseconds(now, "%Y-%m-%d-%H-%M-%S", "%ns", ":");
    cout << "Initial time = " << time << endl;
    time = AndreiUtils::convertChronoToStringWithSubseconds(now, "%Y-%m-%d-%H-%M-%S", "%ms-%pus-%pns", ":");
    cout << "Initial time = " << time << endl;

    auto x = AndreiUtils::convertStringToChronoWithSubseconds(time, "%Y-%m-%d-%H-%M-%S", "%ms-%pus-%pns", ":");

    auto d = x.time_since_epoch();
    // UTC: +1:00
    using Days = std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type>;
    Days days = std::chrono::duration_cast<Days>(d);
    d -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(d);
    d -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(d);
    d -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
    d -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    d -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(d);
    d -= microseconds;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(d);
    cout << hours.count() << ":" << minutes.count() << ":" << seconds.count() << ":" << milliseconds.count() << ":"
         << microseconds.count() << ":" << nanoseconds.count() << endl;

    cout << AndreiUtils::convertChronoToStringWithSubseconds(x, "%Y-%m-%d-%H-%M-%S", "%ns", ":") << endl;
    cout << AndreiUtils::convertChronoToStringWithSubseconds(x, "%Y-%m-%d-%H-%M-%S", "%pns", ":") << endl;
    cout << AndreiUtils::convertChronoToStringWithSubseconds(x, "%Y-%m-%d-%H-%M-%S", "%us", ":") << endl;
    cout << AndreiUtils::convertChronoToStringWithSubseconds(x, "%Y-%m-%d-%H-%M-%S", "%pus", ":") << endl;
    cout << AndreiUtils::convertChronoToStringWithSubseconds(x, "%Y-%m-%d-%H-%M-%S", "%ms", ":") << endl;
}

void timeAddingTesting() {
    SystemTimePoint now = SystemClock::now();
    auto d = now.time_since_epoch();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(d);
    d -= nanoseconds;

    SystemTimePoint t(d);

    SystemTimePoint stD(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, ratio<86400, 1>>(1.5)));
    SystemTimePoint stH(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, ratio<3600, 1>>(1.5)));
    SystemTimePoint stM(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, ratio<60, 1>>(1.5)));
    SystemTimePoint stS(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double>(1.5)));
    SystemTimePoint stMS(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, milli>(1.5)));
    SystemTimePoint stUS(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, micro>(1.5)));
    SystemTimePoint stNS(d + chrono::duration_cast<chrono::nanoseconds>(chrono::duration<double, nano>(1.5)));
    cout << convertChronoToStringWithSubseconds(stD) << endl;
    cout << convertChronoToStringWithSubseconds(stH) << endl;
    cout << convertChronoToStringWithSubseconds(stM) << endl;
    cout << convertChronoToStringWithSubseconds(stS) << endl;
    cout << convertChronoToStringWithSubseconds(stMS) << endl;
    cout << convertChronoToStringWithSubseconds(stUS) << endl;
    cout << convertChronoToStringWithSubseconds(stNS) << endl;

    cout << convertChronoToStringWithSubseconds(now) << endl;
    cout << convertChronoToStringWithSubseconds(t) << endl;
    cout << "d  : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "d")) << endl;
    cout << "h  : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "h")) << endl;
    cout << "min: " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "min")) << endl;
    cout << "s  : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "s")) << endl;
    cout << "ms : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "ms")) << endl;
    cout << "us : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "us")) << endl;
    cout << "ns : " << convertChronoToStringWithSubseconds(addDeltaTime(t, 1.5, "ns")) << endl;
}

void testMapKeys() {
    map<string, int> x{
            {"72", 1},
            {"60", 2},
            {"48", 3},
            {"36", 4},
            {"24", 5},
            {"12", 6},
    };
    printVector(getMapKeys(x));
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

void testDualQuaternions() {
    Vector3d t(1, 2, 3);
    Vector3d p(4, 2, 3);
    vector<double> angles = {M_PI_2, 0, M_PI};
    Quaterniond r = qFromEulerAngles<double>(angles, "zyx");
    DualQuaternion<double> q(r, t);
    cout << q << endl;
    cout << printVectorToString(angles) << endl;
    cout << q.getTranslation().transpose() << endl;
    cout << q.transform(p).transpose() << endl;
    cout << q.getTransformationMatrix() << endl;
    cout << eulerAnglesFromQ(q.getRotation(), "zyx").transpose() << endl;
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

void testSortMultipleVectorsBasedOnOneCriterion() {
    vector<double> x(10);
    std::iota(x.begin(), x.end(), 10);
    vector<int> y(10);
    std::iota(y.begin(), y.end(), 4);
    shuffle(y.begin(), y.end(), std::mt19937(std::random_device()()));
    vector<string> z(10);
    for (int i = 0; i < z.size(); i++) {
        if (i == 0) {
            z[i] = "1";
        } else {
            z[i] = z[i - 1] + "1";
        }
    }
    auto permutation = getSortedIndicesOfVector(y, [](const int &a, const int &b) {
        return a < b;
    });
    printVector(x);
    printVector(y);
    printVector(z);
    printVector(permutation);
    sortMultipleVectorsBasedOnPermutation(permutation, x, y, z);
    printVector(x);
    printVector(y);
    printVector(z);
}

void testAccessTimeInMapVsVector() {
    int64_t N = 1e8 + 1;
    vector<int64_t> v(N);
    map<int64_t, bool> m;
    Timer t;
    double time;
    t.start();
    for (int64_t i = 0; i < N; i++) {
        v[i] = i;
        m[i] = true;
    }
    time = t.measure(TimeUnit::SECOND);
    cout << "Initialization took " << time << endl;

    vector<int64_t> queries = {static_cast<int64_t>(1e0), static_cast<int64_t>(1e1), static_cast<int64_t>(1e2),
                               static_cast<int64_t>(1e3), static_cast<int64_t>(1e4), static_cast<int64_t>(1e5),
                               static_cast<int64_t>(1e6), static_cast<int64_t>(1e7), static_cast<int64_t>(1e8),
                               static_cast<int64_t>(1e9), static_cast<int64_t>(1e10), static_cast<int64_t>(1e11)};
    bool res;
    for (const auto &q: queries) {
        t.start();
        res = vectorContains(v, q);
        time = t.measure(TimeUnit::SECOND);
        cout << "Checking if " << q << " is in vector took " << time << ": res = " << res << endl;

        t.start();
        auto iter = find(v.begin(), v.end(), q);
        time = t.measure(TimeUnit::SECOND);
        cout << "Checking if " << q << " is in vector took " << time << ": res = " << (iter != v.end()) << endl;

        t.start();
        res = mapContains(m, q);
        time = t.measure(TimeUnit::SECOND);
        cout << "Checking if " << q << " is in map took " << time << ": res = " << res << endl;
    }
}

class A {
public:
    virtual ~A() = default;
};

class B : virtual public A {
public:
    bool operator<(const B &other) const {
        return true;
    }
};

class C : virtual public A {
};

class D : public B, C {
};

void testTypeCreator() {
    TypeCreator<A> creator;
    creator.registerTypeCreator("A", []() { return new A(); });
    creator.registerTypeCreator("B", []() { return new B(); });
    creator.registerTypeCreator("C", []() { return new C(); });
    creator.registerTypeCreator("D", []() { return new D(); });
    auto c = creator.createType("C");
    auto a = dynamic_cast<A *>(c);
    auto b = dynamic_cast<B *>(c);
    auto d = dynamic_cast<D *>(c);
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    delete c;
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
    struct hash<B> {
        size_t operator()(const B &b) const noexcept {
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
    cout << std::is_default_constructible<std::hash<B>>::value << endl;
    cout << std::is_default_constructible<std::hash<C>>::value << endl;

    cout << is_hashable<int>::value << endl;
    cout << is_hashable<float>::value << endl;
    cout << is_hashable<double>::value << endl;
    cout << is_hashable<B>::value << endl;
    cout << is_hashable<C>::value << endl;

    auto b = std::hash<B>();
    // auto c = std::hash<C>();  // Compiler error!

    map<B, int> mB;
    map<C, int> mC;
    B bObj1, bObj2, bObj3;
    C cObj1, cObj2, cObj3;

    // is_detected<std::less<int>::operator()>;
    // is_detected<std::less<int>::operator(), int>;
    // is_detected<std::less<C>::operator(), C, C>;

    cout << is_hashable<map<B, int>::key_compare>::value << endl;

    mB[bObj1] = 1;
    mB[bObj2] = 2;
    mB[bObj3] = 3;
    // mC[cObj1] = 1;  // Compiler error
    // mC[cObj2] = 2;  // Compiler error
    // mC[cObj3] = 3;  // Compiler error
}

int main() {
    cout << "Hello World!" << endl;
    // eigenTesting();
    // fileTesting();
    // realsenseDistortionString();
    // timeTesting();
    // timeAddingTesting();
    // testPointerReference();
    // testMapKeys();
    // testJsonNull();
    // testLambdaCaptureScope();
    // testDualQuaternions();
    // testStringAllocation();
    // testFloatSlidingWindow();
    // testCrossBilateralFilter();
    // testSortMultipleVectorsBasedOnOneCriterion();
    // testAccessTimeInMapVsVector();
    // testTypeCreator();
    // testIntegralAndUnsignedTypes();
    testUnionFind();
    // testHashable();
    return 0;
}
