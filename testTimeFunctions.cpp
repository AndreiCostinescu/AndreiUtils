//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/Timer.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsTime.h>
#include <AndreiUtils/utilsVector.hpp>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;

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

void testSerializeDeserializeTimestamp() {
    #if __cplusplus >= 202002L

    cout << "With C++20 suppport!" << endl;
    auto t = now();
    auto s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    auto t1 = convertStringToChronoWithSubseconds(s);
    auto s1 = convertChronoToStringWithSubseconds(t1);
    cout << s1 << endl;
    auto t2 = convertStringToChronoWithSubseconds(s1);
    auto s2 = convertChronoToStringWithSubseconds(t2);
    cout << s2 << endl;

    #elif __cplusplus >= 201703L

    cout << "With C++17 support!" << endl;
    auto t = now();
    auto s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;
    t = convertStringToChronoWithSubseconds(s);
    s = convertChronoToStringWithSubseconds(t);
    cout << s << endl;

    #endif
}

TEST(TimeTesting, AccessTimeInMapVsVector) {
    int64_t N = static_cast<int64_t>(1e7);
    vector<int64_t> v(N);
    map<int64_t, bool> m;
    Timer t;
    double vector_time = 0.0;
    double map_time = 0.0;

    for (int64_t i = 0; i < N; i++) {
        v[i] = i;
        m[i] = true;
    }

    vector<int64_t> queries = {static_cast<int64_t>(1e0), static_cast<int64_t>(1e1), static_cast<int64_t>(1e2),
                                    static_cast<int64_t>(1e3), static_cast<int64_t>(1e4), static_cast<int64_t>(1e5),
                                    static_cast<int64_t>(1e6), static_cast<int64_t>(1e7)};
    bool res;

    for (const auto& q : queries) {

        t.start();
        res = vectorContains(v, q);
        vector_time += t.measure();
        if (q < N) {
            ASSERT_TRUE(res) << "Expected " << q << " to be found in vector";
        }
        t.start();
        res = mapContains(m, q);
        map_time += t.measure();
        if (q < N) {
            ASSERT_TRUE(res) << "Expected " << q << " to be found in map";
        }
    }

    vector_time /= queries.size();
    map_time /= queries.size();

    std::cout << "Average vector access time: " << vector_time << " seconds" << std::endl;
    std::cout << "Average map access time: " << map_time << " seconds" << std::endl;

    ASSERT_LT(map_time, vector_time) << "Expected map access to be faster than vector access";
}

TEST(TimeTesting, SerializeDeserialize) {
#if __cplusplus >= 202002L
    auto t = now();
    auto s = convertChronoToStringWithSubseconds(t);
    auto t1 = convertStringToChronoWithSubseconds(s);
    auto s1 = convertChronoToStringWithSubseconds(t1);
    EXPECT_EQ(s, s1);
    auto t2 = convertStringToChronoWithSubseconds(s1);
    auto s2 = convertChronoToStringWithSubseconds(t2);
    EXPECT_EQ(s1, s2);

#elif __cplusplus >= 201703L
    auto t = now();
    auto s = convertChronoToStringWithSubseconds(t);
    for (int i = 0; i < 5; ++i) {
        t = convertStringToChronoWithSubseconds(s);
        s = convertChronoToStringWithSubseconds(t);
    }
    EXPECT_EQ(s, convertChronoToStringWithSubseconds(convertStringToChronoWithSubseconds(s)));
#endif
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testAccessTimeInMapVsVector();
    // testSerializeDeserializeTimestamp();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}