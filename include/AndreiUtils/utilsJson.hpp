//
// Created by Andrei on 27.10.22.
//

#ifndef ANDREIUTILS_UTILSJSON_HPP
#define ANDREIUTILS_UTILSJSON_HPP

#include <AndreiUtils/classes/camera/ImageCaptureParametersWithIntrinsics.h>

namespace nlohmann {
    template<class T>
    struct adl_serializer<AndreiUtils::ImageParameters> {
        using T = AndreiUtils::ImageParameters;

        static void to_json(nlohmann::json &j, T const &data) {
            j.clear();
            j["h"] = data.h;
            j["w"] = data.w;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.h = j.at("h").get<int>();
            data.w = j.at("w").get<int>();
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::CameraIntrinsicParameters> {
        using T = AndreiUtils::CameraIntrinsicParameters;

        static void to_json(nlohmann::json &j, T const &data) {
            j.clear();
            j["fx"] = data.fx;
            j["fy"] = data.fy;
            j["ppx"] = data.ppx;
            j["ppy"] = data.ppy;
            j["distortionModel"] = convertImageDistortionModelToString(data.distortionModel);
            j["distortionCoefficients"] = data.getDistortionCoefficientsAsVector();
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.setImageParameters(j.at("fx").get<double>(), j.at("fy").get<double>(), j.at("ppx").get<double>(),
                                     j.at("ppy").get<double>());
            data.setDistortionParameters(convertStringToImageDistortionModel(j.at("distortionModel").get<string>()),
                                          j.at("distortionCoefficients").get<vector<float>>());
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::ImageCaptureParameters> {
        using T = AndreiUtils::ImageCaptureParameters;

        static void to_json(nlohmann::json &j, T const &data) {
            j.clear();
            j["fps"] = data.fps;
            j["size"] = data.size;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.fps = j.at("fps").get<double>();
            data.size = j.at("size").get<ImageParameters>();
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::ImageCaptureParametersWithIntrinsics> {
        using T = AndreiUtils::ImageCaptureParametersWithIntrinsics;

        static void to_json(nlohmann::json &j, T const &data) {
            j = (AndreiUtils::AndreiUtils::ImageCaptureParameters) data;
            j["intrinsics"] = data.intrinsics;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data = j.get<AndreiUtils::ImageCaptureParameters>();
            data.intrinsics = j.at("intrinsics").get<CameraIntrinsicParameters>();
        }
    };
}

#endif //ANDREIUTILS_UTILSJSON_HPP
