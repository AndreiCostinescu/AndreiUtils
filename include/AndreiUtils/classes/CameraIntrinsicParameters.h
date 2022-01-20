//
// Created by andrei on 14.01.22.
//

#ifndef ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
#define ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP

#include <AndreiUtils/enums/ImageDistortionModel.h>
#include <AndreiUtils/utilsOpenMP.hpp>
#include <vector>

namespace AndreiUtils {
    class CameraIntrinsicParameters {
    public:
        CameraIntrinsicParameters();

        CameraIntrinsicParameters(int h, int w, double fx, double fy, double ppx, double ppy,
                                  ImageDistortionModel distortionModel, const float *distortionCoefficients);

        ~CameraIntrinsicParameters();

        std::string toString() const;

        void setImageParameters(int _h, int _w, double _fx, double _fy, double _ppx, double _ppy);

        void setDistortionParameters(ImageDistortionModel _distortionModel, const float *_distortionCoefficients);

        void setDistortionParameters(ImageDistortionModel _distortionModel,
                                     const std::vector<float> &_distortionCoefficients);

        std::vector<float> getDistortionCoefficientsAsVector() const;

        int h, w, nrDistortionCoefficients;
        double fx, fy, ppx, ppy;
        float *distortionCoefficients;
        ImageDistortionModel distortionModel;
    };
}

#endif //ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
