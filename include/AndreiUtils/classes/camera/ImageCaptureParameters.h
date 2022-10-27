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

        ImageCaptureParameters(double fps, ImageParameters const &size);

        virtual ~ImageCaptureParameters();

        double fps;
        ImageParameters size;
    };
}

#endif //ANDREIUTILS_IMAGECAPTUREPARAMETERS_H
