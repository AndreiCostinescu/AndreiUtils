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

#ifndef ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
#define ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H

#include <AndreiUtils/classes/motion/MotionDeviceIntrinsicParameters.h>
#include <Eigen/Dense>

namespace AndreiUtils {
    class MotionDeviceCaptureParameters {
    public:
        MotionDeviceCaptureParameters();

        MotionDeviceCaptureParameters(const double &fps, const MotionDeviceIntrinsicParameters &intrinsics);

        virtual ~MotionDeviceCaptureParameters();

        void setMotionDeviceCaptureParameters(const double &_fps, const MotionDeviceIntrinsicParameters &_intrinsics);

        double fps;
        MotionDeviceIntrinsicParameters intrinsics;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_MOTIONDEVICECAPTUREPARAMETERS_H
