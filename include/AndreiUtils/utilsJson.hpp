//
// Created by Andrei on 27.10.22.
//

#pragma once

#include <AndreiUtils/classes/camera/ImageCaptureParametersWithIntrinsics.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>
#include <AndreiUtils/classes/Interval.hpp>
#include <AndreiUtils/json.hpp>
#include <complex>

namespace nlohmann {
    template<class T>
    struct adl_serializer<std::complex<T>> {
        static void to_json(nlohmann::json &j, std::complex<T> const &v) {
            j["r"] = v.real();
            j["i"] = v.imag();
        }

        static void from_json(nlohmann::json const &j, std::complex<T> &v) {
            v.real(j["r"].get<T>());
            v.imag(j["i"].get<T>());
        }
    };

    template<>
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
            j["distortionModel"] = AndreiUtils::convertImageDistortionModelToString(data.distortionModel);
            j["distortionCoefficients"] = data.getDistortionCoefficientsAsVector();
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.setImageParameters(j.at("fx").get<double>(), j.at("fy").get<double>(), j.at("ppx").get<double>(),
                                     j.at("ppy").get<double>());
            data.setDistortionParameters(AndreiUtils::convertStringToImageDistortionModel(j.at("distortionModel").get<std::string>()),
                                          j.at("distortionCoefficients").get<std::vector<float>>());
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
            data.size = j.at("size").get<AndreiUtils::ImageParameters>();
        }
    };

    template<>
    struct adl_serializer<AndreiUtils::ImageCaptureParametersWithIntrinsics> {
        using T = AndreiUtils::ImageCaptureParametersWithIntrinsics;

        static void to_json(nlohmann::json &j, T const &data) {
            j = (AndreiUtils::ImageCaptureParameters) data;
            j["intrinsics"] = data.intrinsics;
        }

        static void from_json(nlohmann::json const &j, T &data) {
            data.ImageCaptureParameters::setFromOther(j.get<AndreiUtils::ImageCaptureParameters>());
            data.intrinsics = j.at("intrinsics").get<AndreiUtils::CameraIntrinsicParameters>();
        }
    };

    template<typename T>
    struct adl_serializer<AndreiUtils::Interval<T>> {
        using Type = AndreiUtils::Interval<T>;

        static void to_json(nlohmann::json &j, Type const &data) {
            std::vector<nlohmann::json> interval;
            if (!data.isEmpty()) {
                if (data.hasInfLowerBound()) {
                    interval.emplace_back("-inf");
                } else {
                    interval.emplace_back(data.getMin());
                }
                if (data.hasInfUpperBound()) {
                    interval.emplace_back("inf");
                } else {
                    interval.emplace_back(data.getMax());
                }
            }
            j = interval;
        }

        static void from_json(nlohmann::json const &j, Type &data) {
            auto interval = j.get<std::vector<nlohmann::json>>();
            if (interval.empty()) {
                data = Type::createEmpty();
                return;
            }
            assert(interval.size() == 2);
            bool infLower = false, infUpper = false;
            T minValue, maxValue;
            if (interval[0].is_string()) {
                assert(interval[0].get<std::string>() == "-inf");
                infLower = true;
            } else {
                minValue = interval[0].get<T>();
            }
            if (interval[1].is_string()) {
                assert(interval[1].get<std::string>() == "inf");
                infUpper = true;
            } else {
                maxValue = interval[1].get<T>();
            }
            if (infLower && infUpper) {
                data = Type::createFullRange();
            } else if (infLower) {
                data = Type::createOnlyUpperBound(std::move(maxValue));
            } else if (infUpper) {
                data = Type::createOnlyLowerBound(std::move(minValue));
            } else {
                data = Type(std::move(minValue), std::move(maxValue));
            }
        }
    };
}

namespace AndreiUtils {
    template<typename T>
    bool canConvertJsonTo(nlohmann::json const &j, T *res) {
        try {
            auto _res = j.get<T>();
            if (res != nullptr) {
                *res = _res;
            }
            return true;
        } catch (nlohmann::json::exception const &e) {
            return false;
        } catch (std::exception const &e) {
            return false;
        }
    }

    template<typename T>
    bool canConvertJsonTo(nlohmann::json const &j) {
        return canConvertJsonTo<T>(j, nullptr);
    }

    template<typename T>
    bool canConvertJsonTo(nlohmann::json const &j, T &res) {
        return canConvertJsonTo<T>(j, &res);
    }
}
