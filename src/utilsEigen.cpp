/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsEigen.h>
#include <fstream>

using namespace Eigen;
using namespace std;

MatrixXd AndreiUtils::readEigenData(string const &file) {
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

void AndreiUtils::writeEigenData(string const &file, MatrixXd const &m) {
    ofstream fout(file);
    if (!fout.is_open()) {
        throw runtime_error("Can not open eigen-output file: " + file);
    }
    int n = (int) m.rows(), d = (int) m.cols();
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

bool AndreiUtils::isOrthonormal(Matrix3d const &m) {
    for (unsigned i = 0; i < 3; i++) {
        if (fabs(sqrt(pow(m(0, i), 2) + pow(m(1, i), 2) + pow(m(2, i), 2)) - 1) > 1e-3) {
            return false;
        }
    }
    Eigen::Matrix<double, 3, 1> y_d = m.block<3, 1>(0, 2).cross(m.block<3, 1>(0, 0));
    for (unsigned i = 0; i < 3; i++) {
        if (fabs(y_d(i) - m(i, 1)) > 1e-3) {
            return false;
        }
    }
    return true;
}
