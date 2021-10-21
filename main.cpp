//
// Created by andrei on 05.10.21.
//

#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsEigenOpenCV.h>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

int main() {
    cout << "Hello World!" << endl;
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
    return 0;
}