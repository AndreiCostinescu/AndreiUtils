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
    void imageDataRotation(uint8_t *data, RotationType rotation, StandardTypes imageType, int height, int width,
                           int channels = 1);

    void imageDataRotationWithDesiredParameters(
            uint8_t *data, RotationType applyRotation, StandardTypes imageType, int desiredHeight, int desiredWidth,
            int channels = 1);

    bool readImageHeader(std::ifstream *fin, int &height, int &width, StandardTypes &type, int &channels);

    bool readImageData(std::ifstream *fin, uint8_t *image, int nrBytes);

    bool readImageBinary(std::ifstream *fin, uint8_t *&image, int &height, int &width, StandardTypes &type,
                         int &channels);

    bool readImageBinary(std::ifstream *fin, uint8_t *image, int &height, int &width, StandardTypes &type,
                         int &channels, int nrBytes);

    bool readColorImageBinary(std::ifstream *fin, uint8_t *&image, int &height, int &width, StandardTypes &type);

    bool readColorImageBinary(std::ifstream *fin, uint8_t *image, int &height, int &width, StandardTypes &type,
                              int nrBytes);

    bool readDepthImageBinary(std::ifstream *fin, uint16_t *&depth, int &height, int &width);

    bool readDepthImageBinary(std::ifstream *fin, uint16_t *depth, int &height, int &width, int nrBytes);

    bool readDepthImageBinary(std::ifstream *fin, double *&depth, int &height, int &width);

    bool readDepthImageBinary(std::ifstream *fin, double *depth, int &height, int &width, int nrBytes);

    void writeImageBinary(std::ofstream *fout, const uint8_t *image, int height, int width, StandardTypes type,
                          int channels);

    void writeColorImageBinary(std::ofstream *fout, const uint8_t *image, int height, int width, StandardTypes type);

    void writeDepthImageBinary(std::ofstream *fout, const uint16_t *depth, int height, int width);

    void writeDepthImageBinary(std::ofstream *fout, const double *depth, int height, int width);

    void swapColorImageChannels(uint8_t *image, int nrElements, int channels,
                                const std::vector<std::pair<int, int>> &channelSwaps);
}

#endif //ANDREIUTILS_UTILSIMAGES_H
