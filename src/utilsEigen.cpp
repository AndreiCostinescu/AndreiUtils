//
// Created by andrei on 27.08.21.
//

#include <utilsEigen.h>

using namespace Eigen;
using namespace std;

string eigenToString(const MatrixXd &eigenData) {
    std::stringstream ss;
    ss << eigenData;
    return ss.str();
}

bool checkSize(const MatrixXd &a, const MatrixXd &b) {
    return a.rows() == b.rows() && a.cols() == b.cols();
}
