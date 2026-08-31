// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 20.01.23.
//

#include <AndreiUtils/utilsEigenSVD.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

Eigen::JacobiSVD<Eigen::MatrixXd> AndreiUtils::svdJacobi(Eigen::MatrixXd const &a, int computeOptions) {
    return a.jacobiSvd(computeOptions);
}

Eigen::BDCSVD<Eigen::MatrixXd> AndreiUtils::svdBDC(Eigen::MatrixXd const &a, int computeOptions) {
    return a.bdcSvd(computeOptions);
}
