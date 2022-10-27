//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H
#define ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H

#include <AndreiUtils/classes/camera/ImageCaptureParameters.h>
#include <AndreiUtils/classes/camera/ImageParameters.h>
#include <AndreiUtils/classes/camera/CameraIntrinsicParameters.h>

namespace AndreiUtils {
    class ImageCaptureParametersWithIntrinsics : public ImageCaptureParameters {
    public:
        ImageCaptureParametersWithIntrinsics();

        ImageCaptureParametersWithIntrinsics(
                double fps, ImageParameters const &size, CameraIntrinsicParameters const &intrinsics);

        ~ImageCaptureParametersWithIntrinsics() override;

        CameraIntrinsicParameters intrinsics;
    };
}

#endif //ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H
