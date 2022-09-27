//
// Created by Andrei on 14.01.22.
//

#ifndef ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
#define ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP

#include <AndreiUtils/enums/ImageDistortionModel.h>
#include <AndreiUtils/json.hpp>
#include <AndreiUtils/utilsOpenMP.hpp>
#include <vector>

namespace AndreiUtils {
    class CameraIntrinsicParameters {
    public:
        CameraIntrinsicParameters();

        CameraIntrinsicParameters(double fx, double fy, double ppx, double ppy, ImageDistortionModel distortionModel,
                                  const float *distortionCoefficients);

        CameraIntrinsicParameters(const CameraIntrinsicParameters &other);

        ~CameraIntrinsicParameters();

        CameraIntrinsicParameters &operator=(const CameraIntrinsicParameters &other);

        std::string toString() const;

        void setImageParameters(double _fx, double _fy, double _ppx, double _ppy);

        void setDistortionParameters(ImageDistortionModel _distortionModel, const float *_distortionCoefficients);

        void setDistortionParameters(ImageDistortionModel _distortionModel,
                                     const std::vector<float> &_distortionCoefficients);

        std::vector<float> getDistortionCoefficientsAsVector() const;

        void toJson(nlohmann::json &j) const;

        void fromJson(const nlohmann::json &j);

        int nrDistortionCoefficients;
        double fx, fy, ppx, ppy;
        float *distortionCoefficients;
        ImageDistortionModel distortionModel;

    protected:
        void copyDataFromOther(const CameraIntrinsicParameters &other);
    };
}

namespace nlohmann {
    template<>
    struct adl_serializer<AndreiUtils::CameraIntrinsicParameters> {
        static void to_json(nlohmann::json &j, const AndreiUtils::CameraIntrinsicParameters &p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::CameraIntrinsicParameters &p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
