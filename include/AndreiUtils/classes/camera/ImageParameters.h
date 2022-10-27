//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_IMAGEPARAMETERS_H
#define ANDREIUTILS_IMAGEPARAMETERS_H

namespace AndreiUtils {
    class ImageParameters {
    public:
        ImageParameters();

        ImageParameters(int height, int width);

        virtual ~ImageParameters();

        void setImageParameters(int _h, int _w);

        int h, w;
    };
}

#endif //ANDREIUTILS_IMAGEPARAMETERS_H
