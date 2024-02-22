//
// Created by Andrei on 20-Oct-21.
//

#ifndef ANDREIUTILS_UTILSIMAGES_H
#define ANDREIUTILS_UTILSIMAGES_H

#include <AndreiUtils/enums/RotationType.h>
#include <AndreiUtils/enums/StandardTypes.h>
#include <cstdint>
#include <fstream>
#include <tuple>
#include <vector>

namespace AndreiUtils {
    void imageDataRotationInto(uint8_t *dst, uint8_t *data, RotationType rotation, StandardTypes imageType, int height,
                               int width, int channels = 1);

    uint8_t *imageDataRotation(uint8_t *data, RotationType rotation, StandardTypes imageType, int height, int width,
                               int channels = 1);

    uint8_t *imageDataRotationWithDesiredParameters(
            uint8_t *data, RotationType applyRotation, StandardTypes imageType, int desiredHeight, int desiredWidth,
            int channels = 1);

    bool readImageHeader(std::ifstream &in, int &height, int &width, StandardTypes &type, int &channels);

    bool skipImageData(std::ifstream &in, int nrBytes);

    bool readImageData(std::ifstream &in, uint8_t *image, int nrBytes);

    bool readImageBinary(std::ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type,
                         int &channels);

    bool readImageBinary(std::ifstream &in, uint8_t *image, int &height, int &width, StandardTypes &type,
                         int &channels, int nrBytes);

    bool readColorImageBinary(std::ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type);

    bool readColorImageBinary(std::ifstream &in, uint8_t *image, int &height, int &width, StandardTypes &type,
                              int nrBytes);

    bool readDepthImageBinary(std::ifstream &in, uint16_t *&depth, int &height, int &width);

    bool readDepthImageBinary(std::ifstream &in, uint16_t *depth, int &height, int &width, int nrBytes);

    bool readDepthImageBinary(std::ifstream &in, double *&depth, int &height, int &width);

    bool readDepthImageBinary(std::ifstream &in, double *depth, int &height, int &width, int nrBytes);

    bool readDepthImageBinaryConvert(std::ifstream &in, double *&depth, int &height, int &width,
                                     double conversionFactor = 1000);

    bool readDepthImageBinaryConvert(std::ifstream &in, double *depth, int &height, int &width, int nrBytes,
                                     double conversionFactor = 1000);

    void writeImageBinary(std::ofstream &out, const uint8_t *image, int height, int width, StandardTypes type,
                          int channels);

    void writeColorImageBinary(std::ofstream &out, uint8_t const *image, int height, int width, StandardTypes type);

    void writeDepthImageBinary(std::ofstream &out, uint16_t const *depth, int height, int width);

    void writeDepthImageBinary(std::ofstream &out, double const *depth, int height, int width);

    void writeDepthImageBinaryConvert(std::ofstream &out, double const *depth, int height, int width,
                                      double conversionFactor = 1000);

    void swapColorImageChannels(uint8_t *image, int nrElements, int channels,
                                const std::vector<std::pair<int, int>> &channelSwaps);
}

#endif //ANDREIUTILS_UTILSIMAGES_H
