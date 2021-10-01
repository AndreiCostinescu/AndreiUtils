//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsEigen.h>
#include <fstream>

using namespace Eigen;
using namespace std;

string AndreiUtils::eigenToString(const MatrixXd &eigenData) {
    std::stringstream ss;
    ss << eigenData;
    return ss.str();
}

bool AndreiUtils::checkSameSize(const MatrixXd &a, const MatrixXd &b) {
    return a.rows() == b.rows() && a.cols() == b.cols();
}

MatrixXd AndreiUtils::readEigenData(const string &file) {
    ifstream fin(file);
    if (!fin.is_open()) {
        throw runtime_error("Can not open eigen-read file: " + file);
    }
    int n, d;
    fin >> n >> d;
    MatrixXd data = MatrixXd::Zero(n, d);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            fin >> data(i, j);
        }
    }
    return data;
}

void AndreiUtils::writeEigenData(const string &file, const MatrixXd &m) {
    ofstream fout(file);
    if (!fout.is_open()) {
        throw runtime_error("Can not open eigen-output file: " + file);
    }
    int n = m.rows(), d = m.cols();
    fout << n << " " << d << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            if (j > 0) {
                fout << " ";
            }
            fout << m(i, j);
        }
        fout << endl;
    }
}
