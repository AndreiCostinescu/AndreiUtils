//
// Created by Andrei on 27-Feb-22.
//

#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
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

void MotionDeviceIntrinsicParameters::toJson(nlohmann::json &j) const {
    j.clear();
    j["data"] = this->data;
    j["biasVariances"] = this->biasVariances;
    j["noiseVariances"] = this->noiseVariances;
}

void MotionDeviceIntrinsicParameters::fromJson(const nlohmann::json &j) {
    this->data = j.at("data").get<Eigen::MatrixXd>();
    this->biasVariances = j.at("biasVariances").get<Eigen::VectorXd>();
    this->noiseVariances = j.at("noiseVariances").get<Eigen::VectorXd>();
}
