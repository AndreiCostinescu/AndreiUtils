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
                double fps, const ImageParameters &size, const CameraIntrinsicParameters &intrinsics);

        ~ImageCaptureParametersWithIntrinsics() override;

        void toJson(nlohmann::json &j) const override;

        void fromJson(const nlohmann::json &j) override;

        CameraIntrinsicParameters intrinsics;
    };
}

namespace nlohmann {
    template<>
    struct adl_serializer<AndreiUtils::ImageCaptureParametersWithIntrinsics> {
        static void to_json(nlohmann::json &j, const AndreiUtils::ImageCaptureParametersWithIntrinsics &p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json &j, AndreiUtils::ImageCaptureParametersWithIntrinsics &p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_IMAGECAPTUREPARAMETERSWITHINTRINSICS_H
