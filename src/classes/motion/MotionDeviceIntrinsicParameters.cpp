//
// Created by Andrei on 27-Feb-22.
//

#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <utility>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

MotionDeviceIntrinsicParameters::MotionDeviceIntrinsicParameters() : data(), biasVariances(), noiseVariances() {}

MotionDeviceIntrinsicParameters::MotionDeviceIntrinsicParameters(
        MatrixXd data, VectorXd biasVariances, VectorXd noiseVariances) :
        data(move(data)), biasVariances(move(biasVariances)), noiseVariances(move(noiseVariances)) {}

MotionDeviceIntrinsicParameters::~MotionDeviceIntrinsicParameters() = default;

void MotionDeviceIntrinsicParameters::setMotionDeviceIntrinsicParameters(
        const MatrixXd &_data, const VectorXd &_biasVariances, const VectorXd &_noiseVariances) {
    this->data = _data;
    this->biasVariances = _biasVariances;
    this->noiseVariances = _noiseVariances;
}
