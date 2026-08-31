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
// Created by Andrei on 27-Feb-22.
//

#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <utility>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

MotionDeviceIntrinsicParameters::MotionDeviceIntrinsicParameters() : data(), biasVariances(), noiseVariances() {}

MotionDeviceIntrinsicParameters::MotionDeviceIntrinsicParameters(MatrixXd data, VectorXd biasVariances,
                                                                 VectorXd noiseVariances) :
    data(move(data)), biasVariances(move(biasVariances)), noiseVariances(move(noiseVariances)) {}

MotionDeviceIntrinsicParameters::~MotionDeviceIntrinsicParameters() = default;

void MotionDeviceIntrinsicParameters::setMotionDeviceIntrinsicParameters(MatrixXd const &_data,
                                                                         VectorXd const &_biasVariances,
                                                                         VectorXd const &_noiseVariances) {
    this->data = _data;
    this->biasVariances = _biasVariances;
    this->noiseVariances = _noiseVariances;
}

void MotionDeviceIntrinsicParameters::setFromOther(MotionDeviceIntrinsicParameters const &other) {
    if (this == &other) {
        return;
    }
    this->data = other.data;
    this->biasVariances = other.biasVariances;
    this->noiseVariances = other.noiseVariances;
}

void MotionDeviceIntrinsicParameters::setFromOther(MotionDeviceIntrinsicParameters &&other) {
    if (this == &other) {
        return;
    }
    this->data = std::move(other.data);
    this->biasVariances = std::move(other.biasVariances);
    this->noiseVariances = std::move(other.noiseVariances);
}
