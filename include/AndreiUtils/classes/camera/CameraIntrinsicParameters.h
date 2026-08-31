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
// Created by Andrei on 14.01.22.
//

#ifndef ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
#define ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP

#include <AndreiUtils/enums/ImageDistortionModel.h>
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

        int nrDistortionCoefficients;
        double fx, fy, ppx, ppy;
        float *distortionCoefficients;
        ImageDistortionModel distortionModel;

    protected:
        void copyDataFromOther(const CameraIntrinsicParameters &other);
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_CAMERAINTRINSICPARAMETERS_HPP
