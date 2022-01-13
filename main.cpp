//
// Created by andrei on 05.10.21.
//

#include <AndreiUtils/utilsEigenOpenCV.h>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsTime.h>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

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

void timeTesting() {
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
    /* UTC: +1:00 */
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

int main() {
    cout << "Hello World!" << endl;
    // eigenTesting();
    fileTesting();
    // timeTesting();
    return 0;
}
