//
// Created by andrei on 05.10.21.
//

#include <AndreiUtils/classes/CrossBilateralFilter.hpp>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/classes/SlidingWindow.hpp>
#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/classes/TypeCreator.hpp>
#include <AndreiUtils/classes/UnionFind.hpp>
#include <AndreiUtils/classes/graph/BFS.hpp>
#include <AndreiUtils/classes/graph/DFS.hpp>
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
#include <AndreiUtils/utilsOpenCV.h>
#include <AndreiUtils/utilsOpenCV.hpp>
#include <AndreiUtils/utilsOpenMP.h>
#include <AndreiUtils/utilsTime.h>
#include <iostream>
// #include <librealsense2/rs.hpp>
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
    vector<double> angles;
    // angles = {M_PI_2, 0, M_PI};
    // angles = {0, 0, M_PI};
    angles = {M_PI_2, 0, 0};
    Quaterniond r = qFromEulerAngles<double>(angles, "zyx");
    DualQuaternion<double> q(r, t);
    cout << "q = " << q << endl;
    cout << printVectorToString(angles) << endl;
    cout << q.getTranslation().transpose() << endl;
    cout << q.transform(p).transpose() << endl;
    cout << q.getTransformationMatrix() << endl;
    cout << eulerAnglesFromQ(q.getRotation(), "zyx").transpose() << endl;
    cout << endl;

    cout << "Inverse:" << endl;
    DualQuaternion<double> qInv = q.dualQuaternionInverse();
    cout << "qInv = " << qInv << endl;
    cout << "q * qInv = " << q * qInv << endl;
    cout << "qInv * q = " << qInv * q << endl;
    cout << endl;

    Eigen::Matrix<double, 8, 1> coeffs;
    coeffs.topRows(4) = q.getRotation().coeffs().cast<double>();
    coeffs.bottomRows(4) = q.getDual().coeffs().cast<double>();

    cout << q.coefficientNorm() << endl;
    cout << qInv.coefficientNorm() << endl;
    q.normalize();
    qInv.normalize();
    cout << q.coefficientNorm() << endl;
    cout << qInv.coefficientNorm() << endl;
    cout << "q - qInv = " << q - qInv << endl;
    cout << (qInv - q).coefficientNorm() << endl;
    cout << (q - qInv).coefficientNorm() << endl;
    cout << "q    = " << q << endl;
    cout << "qInv = " << qInv << endl;
    cout << average(vector<DualQuaternion<double>>({q, qInv})) << endl;

    cout << endl << endl;

    cout << q.getTranslation().transpose() << ", " << q << endl;
    p = Vector3d(2, -31, 1);
    auto q1 = q.addTranslation(p);
    cout << q1.getTranslation().transpose() << ", " << q1 << endl;
    cout << endl;

    cout << eulerAnglesFromQ(q.getRotation(), "zyx").transpose() << ", " << q.getTranslation().transpose() << ", " << q
         << endl;
    // angles = {M_PI / 4, M_PI / 3, -M_PI / 6};
    angles = {-M_PI_2, M_PI, 0};
    r = qFromEulerAngles<double>(angles, "zyx");
    auto q2 = q.addRotation(r);
    cout << eulerAnglesFromQ(q2.getRotation(), "zyx").transpose() << ", " << q2.getTranslation().transpose() << ", "
         << q2 << endl;
    cout << endl;
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

class D : public B, public C {
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

void testMapFiltering() {
    map<int, int> x;
    for (int i = 0; i < 20; i++) {
        x[i] = i - 5;
    }
    auto y = getFilteredMapBasedOnPredicate(x, (std::function<bool(int const &, int const &)>) [](const int &key,
                                                                                                  const int &value) {
        // return false;
        // return true;
        // return key % 3;
        return value % 4 != 0;
    });
    cout << "Printing map:" << endl;
    printMap(x);
    printMap(y);
    cout << "Done!" << endl;
}

void testDynamicCast() {
    A objA;
    B objB;
    C objC;
    D objD;
    auto *a = &objA;
    auto *b = &objB;
    auto *c = &objC;
    auto *d = &objD;

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << d << endl;
    cout << endl;

    A *tmp;
    tmp = dynamic_cast<A *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<A *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<A *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<A *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<B *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<B *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<B *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<B *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<C *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<C *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<C *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<C *>(d);
    cout << tmp << endl;
    cout << endl;

    tmp = dynamic_cast<D *>(a);
    cout << tmp << endl;
    tmp = dynamic_cast<D *>(b);
    cout << tmp << endl;
    tmp = dynamic_cast<D *>(c);
    cout << tmp << endl;
    tmp = dynamic_cast<D *>(d);
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

void testMapRefAccessing() {
    map<int, int> x;
    x[0] = 0;
    x[1] = 1;
    x[2] = 2;
    x[3] = 3;
    x[4] = 4;
    int value = 42;
    int *valuePtr = nullptr;
    int const *valueConstPtr = nullptr;

    // ------------------ CHECK MAP_GET_IF_CONTAINS FUNCTIONS ------------------

    cout << mapGetIfContains(x, -1, value) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, -1, valueConstPtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 0, value) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    cout << mapGetIfContains(x, 2, valueConstPtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << printMapToString(x) << endl;
    cout << endl;

    value = 42;

    cout << mapGetIfContains(x, 1, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << "Update valuePtr..." << endl;
    *valuePtr = 42;
    cout << printMapToString(x) << endl;
    cout << endl;
    int &valueRef = *valuePtr;

    cout << mapGetIfContains(x, 2, valuePtr) << endl;
    cout << value << ", " << (valuePtr == nullptr ? "null" : to_string(*valuePtr)) << ", "
         << (valueConstPtr == nullptr ? "null" : to_string(*valueConstPtr)) << endl;
    cout << "Update valueRef..." << endl;
    valueRef = 69;
    *valuePtr = -42;
    cout << printMapToString(x) << endl;
    cout << endl;

    // ------------------ NOW CHECK MAP_GET FUNCTIONS ------------------

    try {
        mapGet(x, -1);
    } catch (exception &e) {
        if (strcmp(e.what(), "Element not found in map!") != 0) {
            throw e;
        }
    }

    value = mapGet(x, 0);
    cout << "value = " << value << endl;
    value = 5;
    cout << printMapToString(x) << endl;
    cout << endl;

    int &valueRef2 = mapGet(x, 4);
    cout << "valueRef2 = " << valueRef2 << endl;
    valueRef2 = 5;
    cout << printMapToString(x) << endl;
    cout << endl;

    int &valueRef3 = mapGet(x, 1);
    valueRef3 = -14;
    cout << "valueRef3 = " << valueRef3 << endl;
    cout << "valueRef  = " << valueRef << endl;
    cout << printMapToString(x) << endl;
    cout << endl;
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

    //*
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
    for (auto const &rootData : res) {
        cout << "Distances starting from " << rootData.first << ": " << printMapToString(rootData.second) << endl;
    }
    //*/

    int x = 35;
    cout << (x = 42) << endl;

    g.removeEdge(edgeIdFromNodes(2, 1));
    g.removeNode(1);
}

void testMapCopy() {
    map<int, int> x, y;
    x[0] = 0;
    x[1] = 1;
    x[2] = 1;
    y = x;
    y[0] = 42;

    printMap(x);
    cout << endl << endl;
    printMap(y);
}

void testVectorAppendFunctions() {
    Eigen::Vector3d x(1, 2, 3);
    vector<double> y{1, 2, 3, 4, 5, 6, 7};
    printVector(x.data(), 3);
    printVector(y);
    vector<double> res, res2;
    vectorAppendInPlace(res, y);
    printVector(res);
    vectorAppendInPlace(res, x.data(), 3);
    printVector(res);
    res2 = vectorAppend(y, x.data(), 3);
    printVector(res2);
    res2 = vectorAppend(res, x.data(), 3);
    printVector(res2);
    res2 = vectorAppend(res, y);
    printVector(res2);
    cout << "Done" << endl;
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
    // testJsonArraySerialization();
    // testIntegralAndUnsignedTypes();
    // testUnionFind();
    // testHashable();
    // testMapFiltering();
    // testDynamicCast();
    // testOpenCVMatrixAccessors();
    // testOMPUtils();
    // testPrintingImagesOpenCV();
    // testFastForLoop();
    // testOpenCVMatrixCropReference();
    // testMapRefAccessing();
    // testMixedDataContainer();
    // testGraph();
    // testMapCopy();
    testVectorAppendFunctions();
    return 0;
}
