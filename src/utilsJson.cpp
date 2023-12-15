//
// Created by Andrei on 13.08.2021.
//

#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsJson.hpp>
#include <AndreiUtils/utilsFiles.h>
#include <fstream>
#include <iostream>

using namespace AndreiUtils;
using namespace nlohmann;
using namespace std;

json AndreiUtils::readJsonFile(string const &path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    json content;
    fin >> content;
    return content;
}

void AndreiUtils::writeJsonFile(string const &path, const json &content) {
    createNestedDirectory(path, true);
    ofstream fout(path);
    if (!fout.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    fout << content.dump(4) << endl;
    fout.close();
}

void adl_serializer<AndreiUtils::ImageParameters>::to_json(json &j, AndreiUtils::ImageParameters const &data) {
    j.clear();
    j["h"] = data.h;
    j["w"] = data.w;
}

void adl_serializer<AndreiUtils::ImageParameters>::from_json(json const &j, AndreiUtils::ImageParameters &data) {
    data.h = j.at("h").get<int>();
    data.w = j.at("w").get<int>();
}

void adl_serializer<AndreiUtils::CameraIntrinsicParameters>::to_json(json &j, CameraIntrinsicParameters const &data) {
    j.clear();
    j["fx"] = data.fx;
    j["fy"] = data.fy;
    j["ppx"] = data.ppx;
    j["ppy"] = data.ppy;
    j["distortionModel"] = AndreiUtils::convertImageDistortionModelToString(data.distortionModel);
    j["distortionCoefficients"] = data.getDistortionCoefficientsAsVector();
}

void adl_serializer<AndreiUtils::CameraIntrinsicParameters>::from_json(json const &j, CameraIntrinsicParameters &data) {
    data.setImageParameters(j.at("fx").get<double>(), j.at("fy").get<double>(), j.at("ppx").get<double>(),
                            j.at("ppy").get<double>());
    data.setDistortionParameters(
            AndreiUtils::convertStringToImageDistortionModel(j.at("distortionModel").get<std::string>()),
            j.at("distortionCoefficients").get<std::vector<float>>());
}

void adl_serializer<AndreiUtils::ImageCaptureParameters>::to_json(json &j, ImageCaptureParameters const &data) {
    j.clear();
    j["fps"] = data.fps;
    j["size"] = data.size;
}

void adl_serializer<AndreiUtils::ImageCaptureParameters>::from_json(json const &j, ImageCaptureParameters &data) {
    data.fps = j.at("fps").get<double>();
    data.size = j.at("size").get<AndreiUtils::ImageParameters>();
}

void adl_serializer<ImageCaptureParametersWithIntrinsics>::to_json(
        json &j, ImageCaptureParametersWithIntrinsics const &data) {
    j = (AndreiUtils::ImageCaptureParameters) data;
    j["intrinsics"] = data.intrinsics;
}

void adl_serializer<ImageCaptureParametersWithIntrinsics>::from_json(
        json const &j, ImageCaptureParametersWithIntrinsics &data) {
    data.ImageCaptureParameters::setFromOther(j.get<AndreiUtils::ImageCaptureParameters>());
    data.intrinsics = j.at("intrinsics").get<AndreiUtils::CameraIntrinsicParameters>();
}
