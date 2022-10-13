//
// Created by Andrei on 13.10.22.
//

#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsGeometry.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

SymmetryJsonSerializer::SymmetryJsonSerializer(Symmetry const &s) : sConst(&s), s(nullptr) {}

SymmetryJsonSerializer::SymmetryJsonSerializer(Symmetry &s) : sConst(nullptr), s(&s) {}

void SymmetryJsonSerializer::toJson(nlohmann::json &j) const {
    j["type"] = this->sConst->type;
    vector<double> v(3);
    for (int i = 0; i < 3; i++) {
        v[i] = this->sConst->axis[i];
    }
    j["axis"] = v;
    for (int i = 0; i < 3; i++) {
        v[i] = this->sConst->axisDisplacementFromOrigin[i];
    }
    j["axisDisplacementFromOrigin"] = v;
    if (this->sConst->type == "rotation") {
        pair<double, double> degRange(rad2Deg(this->sConst->range.first), rad2Deg(this->sConst->range.second));
        j["range"] = degRange;
    } else {
        j["range"] = this->sConst->range;
    }
}

void SymmetryJsonSerializer::fromJson(const nlohmann::json &j) {
    this->s->type = j.at("type").get<string>();
    vector<double> v = j.at("axis").get<vector<double>>();
    for (int i = 0; i < 3; i++) {
        this->s->axis[i] = v[i];
    }
    if (j.contains("axisDisplacementFromOrigin")) {
        v = j.at("axisDisplacementFromOrigin").get<vector<double>>();
        for (int i = 0; i < 3; i++) {
            this->s->axisDisplacementFromOrigin[i] = v[i];
        }
    }
    this->s->range = j.at("range").get<pair<double, double>>();
    if (this->s->type == "rotation") {
        this->s->range.first = deg2Rad(this->s->range.first);
        this->s->range.second = deg2Rad(this->s->range.second);
    }
}

MotionDeviceIntrinsicParametersJsonSerializer::MotionDeviceIntrinsicParametersJsonSerializer(
        MotionDeviceIntrinsicParameters const &m) : mConst(&m), m(nullptr) {}

MotionDeviceIntrinsicParametersJsonSerializer::MotionDeviceIntrinsicParametersJsonSerializer(
        MotionDeviceIntrinsicParameters &m) : mConst(nullptr), m(&m) {}

void MotionDeviceIntrinsicParametersJsonSerializer::toJson(nlohmann::json &j) const {
    j.clear();
    j["data"] = this->m->data;
    j["biasVariances"] = this->m->biasVariances;
    j["noiseVariances"] = this->m->noiseVariances;
}

void MotionDeviceIntrinsicParametersJsonSerializer::fromJson(const nlohmann::json &j) {
    this->m->data = j.at("data").get<Eigen::MatrixXd>();
    this->m->biasVariances = j.at("biasVariances").get<Eigen::VectorXd>();
    this->m->noiseVariances = j.at("noiseVariances").get<Eigen::VectorXd>();
}

MotionDeviceCaptureParametersJsonSerializer::MotionDeviceCaptureParametersJsonSerializer(
        MotionDeviceCaptureParameters const &m) : mConst(&m), m(nullptr) {}

MotionDeviceCaptureParametersJsonSerializer::MotionDeviceCaptureParametersJsonSerializer(
        MotionDeviceCaptureParameters &m) : mConst(nullptr), m(&m) {}

void MotionDeviceCaptureParametersJsonSerializer::toJson(nlohmann::json &j) const {
    j.clear();
    j["fps"] = this->m->fps;
    j["intrinsics"] = this->m->intrinsics;
}

void MotionDeviceCaptureParametersJsonSerializer::fromJson(const nlohmann::json &j) {
    this->m->fps = j.at("fps").get<double>();
    this->m->intrinsics = j.at("intrinsics").get<MotionDeviceIntrinsicParameters>();
}
