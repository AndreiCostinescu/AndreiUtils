//
// Created by Andrei on 20-Oct-21.
//

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsImages.h>
#include <AndreiUtils/utilsOpenMP.hpp>
#include <cstring>
#include <functional>

using namespace std;

void AndreiUtils::imageDataRotation(uint8_t *data, RotationType rotation, StandardTypes imageType, int height,
                                    int width, int channels) {
    if (rotation == RotationType::NO_ROTATION) {
        return;
    }

    int nrBytesPerElement = getStandardTypeByteAmount(imageType);
    int rowIncrement = width * channels;
    int nrElements = height * rowIncrement * nrBytesPerElement;
    auto *copy = new uint8_t[nrElements];
    fastMemCopy(copy, data, nrElements);

    int newHeight, newWidth;
    function<int(int, int, int)> getCopyIndex;
    switch (rotation) {
        case LEFT_90: {
            newHeight = width;
            newWidth = height;
            getCopyIndex = [width, channels, rowIncrement](int i, int j, int k) {
                return j * rowIncrement + (width - 1 - i) * channels + k;
            };
            break;
        }
        case LEFT_180: {
            newHeight = height;
            newWidth = width;
            getCopyIndex = [height, width, channels, rowIncrement](int i, int j, int k) {
                return (height - 1 - i) * rowIncrement + (width - 1 - j) * channels + k;
            };
            break;
        }
        case LEFT_270: {
            newHeight = width;
            newWidth = height;
            getCopyIndex = [height, channels, rowIncrement](int i, int j, int k) {
                return (height - 1 - j) * rowIncrement + i * channels + k;
            };
            break;
        }
        default : {
            throw runtime_error("Unknown rotation type " + to_string(rotation));
            break;
        }
    }

    int dataIndex, copyIndex, newRowIncrement = newWidth * channels;
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            for (int k = 0; k < channels; k++) {
                dataIndex = i * newRowIncrement + j * channels + k;
                copyIndex = getCopyIndex(i, j, k);
                memcpy(data + dataIndex, copy + copyIndex, nrBytesPerElement);
            }
        }
    }
}

void AndreiUtils::imageDataRotationWithDesiredParameters(
        uint8_t *data, RotationType applyRotation, StandardTypes imageType, int desiredHeight, int desiredWidth,
        int channels) {
    return imageDataRotation(data, applyRotation, imageType,
                             (applyRotation == LEFT_90 || applyRotation == RIGHT_90) ? desiredWidth : desiredHeight,
                             (applyRotation == LEFT_90 || applyRotation == RIGHT_90) ? desiredHeight : desiredWidth,
                             channels);
}

bool AndreiUtils::readImageHeader(ifstream &in, int &height, int &width, AndreiUtils::StandardTypes &type,
                                  int &channels) {
    try {
        deserialize(in, height);
        deserialize(in, width);
        deserialize<int>(in, type);
        deserialize(in, channels);
        return true;
    } catch (exception &e) {
        cout << "Caught exception while reading the image header: " << e.what() << endl;
        return false;
    }
}

bool AndreiUtils::readImageData(ifstream &in, uint8_t *image, int nrBytes) {
    try {
        deserialize(in, image, nrBytes);
        return true;
    } catch (exception &e) {
        cout << "Caught exception while reading the image data: " << e.what() << endl;
        return false;
    }
}

bool readImageBinaryPrivate(ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type,
                            int &channels, int const *nrBytes = nullptr) {
    if (!readImageHeader(in, height, width, type, channels) || reachedTheEndOfTheFile(in)) {
        return false;
    }

    int _nrBytes = getStandardTypeByteAmount(type) * height * width * channels;
    if (nrBytes == nullptr) {
        delete[] image;
        image = new uint8_t[_nrBytes];
    } else if (*nrBytes != _nrBytes) {
        throw runtime_error("Image data container does not have the same size as the file-stored image: " +
                            to_string(*nrBytes) + " vs. " + to_string(_nrBytes));
    }

    return readImageData(in, image, _nrBytes);
}

bool AndreiUtils::readImageBinary(ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type,
                                  int &channels) {
    return readImageBinaryPrivate(in, image, height, width, type, channels);
}

bool AndreiUtils::readImageBinary(ifstream &in, uint8_t *image, int &height, int &width, StandardTypes &type,
                                  int &channels, int const nrBytes) {
    return readImageBinaryPrivate(in, image, height, width, type, channels, &nrBytes);
}

bool readColorImageBinaryPrivate(ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type,
                                 int const *nrBytes = nullptr) {
    int channels;
    bool readSuccess = readImageBinaryPrivate(in, image, height, width, type, channels, nrBytes);
    if (readSuccess && channels != 3) {
        throw runtime_error("Expected image to have 3 channels, has " + to_string(channels));
    }
    return readSuccess;
}

bool AndreiUtils::readColorImageBinary(ifstream &in, uint8_t *&image, int &height, int &width, StandardTypes &type) {
    return readColorImageBinaryPrivate(in, image, height, width, type);
}

bool AndreiUtils::readColorImageBinary(ifstream &in, uint8_t *image, int &height, int &width, StandardTypes &type,
                                       int const nrBytes) {
    return readColorImageBinaryPrivate(in, image, height, width, type, &nrBytes);
}

template<typename T>
bool readDepthImageBinaryPrivate(ifstream &in, T *&depth, int &height, int &width, StandardTypes expectedType,
                                 int const *nrBytes = nullptr) {
    int channels;
    StandardTypes type;
    if (!readImageBinaryPrivate(in, (uint8_t *&) depth, height, width, type, channels, nrBytes)) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != expectedType) {
        throw runtime_error("Expected depth to have type " + to_string(expectedType) + ", has " + to_string(type));
    }
    return true;
}

bool AndreiUtils::readDepthImageBinary(ifstream &in, uint16_t *&depth, int &height, int &width) {
    return readDepthImageBinaryPrivate(in, depth, height, width, StandardTypes::TYPE_UINT_16);
}

bool AndreiUtils::readDepthImageBinary(ifstream &in, uint16_t *depth, int &height, int &width, int const nrBytes) {
    return readDepthImageBinaryPrivate(in, depth, height, width, StandardTypes::TYPE_UINT_16, &nrBytes);
}

bool AndreiUtils::readDepthImageBinary(ifstream &in, double *&depth, int &height, int &width) {
    return readDepthImageBinaryPrivate(in, depth, height, width, StandardTypes::TYPE_FLOAT_64);
}

bool AndreiUtils::readDepthImageBinary(ifstream &in, double *depth, int &height, int &width, const int nrBytes) {
    return readDepthImageBinaryPrivate(in, depth, height, width, StandardTypes::TYPE_UINT_16, &nrBytes);
}

bool readDepthImageBinaryConvertPrivate(ifstream &in, double *&depth, int &height, int &width,
                                        double conversionFactor, int const *nrBytes = nullptr) {
    int channels;
    StandardTypes type;

    if (!readImageHeader(in, height, width, type, channels) && reachedTheEndOfTheFile(in)) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != StandardTypes::TYPE_UINT_16) {
        throw runtime_error("Expected depth to have type uint16, has " + to_string(type));
    }

    int nrElements = height * width;
    if (nrBytes != nullptr && nrElements * sizeof(double) != *nrBytes) {
        throw runtime_error("Image data container does not have the same size as the file-stored image: " +
                            to_string(*nrBytes) + " vs. " + to_string(nrElements * sizeof(double)));
    }

    auto *tmpData = new uint16_t[nrElements];
    int _nrBytes = nrElements * (int) sizeof(uint16_t);
    bool readSuccess = readImageData(in, (uint8_t *) tmpData, _nrBytes);
    if (readSuccess) {
        // don't return now because tmpData will not be deleted
        delete[] depth;
        depth = new double[nrElements];
        #ifdef WITH_OPENMP
        fastSrcOp<uint16_t, double>(depth, tmpData, nrElements, [&conversionFactor](uint16_t const &x) {
            return double(x) / conversionFactor;
        });
        #else
        for (int i = 0; i < nrElements; i++) {
            depth[i] = tmpData[i] / conversionFactor;
        }
        #endif
    }

    delete[] tmpData;
    return readSuccess;
}

bool AndreiUtils::readDepthImageBinaryConvert(ifstream &in, double *&depth, int &height, int &width,
                                              double conversionFactor) {
    return readDepthImageBinaryConvertPrivate(in, depth, height, width, conversionFactor, nullptr);
}

bool AndreiUtils::readDepthImageBinaryConvert(ifstream &in, double *depth, int &height, int &width, const int nrBytes,
                                              double conversionFactor) {
    return readDepthImageBinaryConvertPrivate(in, depth, height, width, conversionFactor, &nrBytes);
}

void AndreiUtils::writeImageBinary(ofstream &out, uint8_t const *image, int height, int width, StandardTypes type,
                                   int channels) {
    serialize(out, height);
    serialize(out, width);
    serialize(out, type);
    serialize(out, channels);
    serialize(out, image, height * width * channels * getStandardTypeByteAmount(type));
}

void AndreiUtils::writeColorImageBinary(ofstream &out, uint8_t const *image, int height, int width,
                                        StandardTypes type) {
    writeImageBinary(out, image, height, width, type, 3);
}

void AndreiUtils::writeDepthImageBinary(ofstream &out, uint16_t const *depth, int height, int width) {
    writeImageBinary(out, (uint8_t *) depth, height, width, StandardTypes::TYPE_UINT_16, 1);
}

void AndreiUtils::writeDepthImageBinary(ofstream &out, double const *depth, int height, int width) {
    writeImageBinary(out, (uint8_t *) depth, height, width, StandardTypes::TYPE_FLOAT_64, 1);
}

void AndreiUtils::writeDepthImageBinaryConvert(ofstream &out, const double *depth, int height, int width,
                                               double conversionFactor) {
    size_t nrElements = height * width;
    auto *tmpData = new uint16_t[nrElements];
    #ifdef WITH_OPENMP
    fastSrcOp<double, uint16_t>(tmpData, depth, nrElements,
                                [conversionFactor](const double &x) { return (uint16_t) (x * conversionFactor); });
    #else
    for (size_t i = 0; i < nrElements; i++) {
        tmpData[i] = uint16_t(depth[i] * conversionFactor);
    }
    #endif
    writeImageBinary(out, (uint8_t *) tmpData, height, width, StandardTypes::TYPE_UINT_16, 1);
    delete[] tmpData;
}

void AndreiUtils::swapColorImageChannels(uint8_t *image, int nrElements, int channels,
                                         const std::vector<std::pair<int, int>> &channelSwaps) {
    for (const auto &channelSwap: channelSwaps) {
        if (channelSwap.first >= channels || channelSwap.second >= channels || channelSwap.first < 0 ||
            channelSwap.second < 0) {
            continue;
        }
        #ifdef WITH_OPENMP
        fastForLoop<uint8_t>(image, nrElements, [&channelSwap](uint8_t *const image, size_t i, size_t) {
            swapData(image[i + channelSwap.first], image[i + channelSwap.second]);
        }, channels);
        #else
        for (size_t i = 0; i < nrElements; i += channels) {
            swapData(image[i + channelSwap.first], image[i + channelSwap.second]);
        }
        #endif
    }
}
