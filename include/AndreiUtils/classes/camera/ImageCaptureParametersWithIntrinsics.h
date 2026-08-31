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
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H
#define ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H

#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>
#include <AndreiUtils/classes/camera/ImageCaptureParameters.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>

namespace AndreiUtils {
    class ImageCaptureParametersWithIntrinsics : public ImageCaptureParameters {
    public:
        ImageCaptureParametersWithIntrinsics();

        ImageCaptureParametersWithIntrinsics(double fps, ImageParameters const &size,
                                             CameraIntrinsicParameters const &intrinsics);

        ~ImageCaptureParametersWithIntrinsics() override;

        void setFromOther(ImageCaptureParametersWithIntrinsics const &other);

        void setFromOther(ImageCaptureParametersWithIntrinsics &&other);

        CameraIntrinsicParameters intrinsics;
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H
