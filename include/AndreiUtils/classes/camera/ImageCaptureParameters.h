//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_IMAGECAPTUREPARAMETERS_H
#define ANDREIUTILS_IMAGECAPTUREPARAMETERS_H

#include <AndreiUtils/classes/camera/ImageParameters.h>

namespace AndreiUtils {
    class ImageCaptureParameters {
    public:
        ImageCaptureParameters();

        ImageCaptureParameters(double fps, const ImageParameters &size);

        virtual ~ImageCaptureParameters();

        virtual void toJson(nlohmann::json &j) const;

        virtual void fromJson(const nlohmann::json &j);

        double fps;
        ImageParameters size;
    };
}

namespace nlohmann {
    template<> struct adl_serializer<AndreiUtils::ImageCaptureParameters> {
        static void to_json(nlohmann::json& j, const AndreiUtils::ImageCaptureParameters& p) {
            p.toJson(j);
        }

        static void from_json(const nlohmann::json& j, AndreiUtils::ImageCaptureParameters& p) {
            p.fromJson(j);
        }
    };
}

#endif //ANDREIUTILS_IMAGECAPTUREPARAMETERS_H
