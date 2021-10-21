//
// Created by Andrei on 20-Oct-21.
//

#include <AndreiUtils/utilsImages.h>
#include <AndreiUtils/enums/StandardTypes.h>

using namespace std;

bool AndreiUtils::readImageHeader(ifstream *fin, int &height, int &width, AndreiUtils::StandardTypes &type,
                                  int &channels) {
    if (fin->peek() == EOF) {
        return false;
    }
    fin->read((char *) &height, sizeof(int));         // rows
    if (fin->fail() || fin->peek() == EOF) {
        return false;
    }
    fin->read((char *) &width, sizeof(int));         // cols
    if (fin->fail() || fin->peek() == EOF) {
        return false;
    }
    fin->read((char *) &type, sizeof(int));         // type
    if (fin->fail() || fin->peek() == EOF) {
        return false;
    }
    fin->read((char *) &channels, sizeof(int));     // channels
    return !fin->fail();
}

bool AndreiUtils::readImageData(std::ifstream *fin, uint8_t *image, int nrBytes) {
    fin->read((char *) image, nrBytes);
    return !fin->fail();
}

bool AndreiUtils::readImageBinary(std::ifstream *fin, uint8_t *&image, int &height, int &width,
                                  StandardTypes &type, int &channels) {
    if (!readImageHeader(fin, height, width, type, channels) || fin->peek() == EOF) {
        return false;
    }

    delete[] image;
    int nrBytes = getStandardTypeByteAmount(type) * height * width * channels;
    image = new uint8_t[nrBytes];

    return readImageData(fin, image, nrBytes);
}

bool AndreiUtils::readImageBinary(std::ifstream *fin, uint8_t *image, int &height, int &width,
                                  StandardTypes &type, int &channels, const int nrBytes) {
    if (!readImageHeader(fin, height, width, type, channels) || fin->peek() == EOF) {
        return false;
    }

    int storedBytes = getStandardTypeByteAmount(type) * height * width * channels;
    if (nrBytes != storedBytes) {
        throw runtime_error("Image data container does not have the same size as the file-stored image: " +
                            to_string(nrBytes) + " vs. " + to_string(storedBytes));
    }

    return readImageData(fin, image, nrBytes);
}

bool AndreiUtils::readColorImageBinary(std::ifstream *fin, uint8_t *&image, int &height, int &width,
                                       StandardTypes &type) {
    int channels;
    bool readSuccess = readImageBinary(fin, image, height, width, type, channels);
    if (readSuccess && channels != 3) {
        throw runtime_error("Expected image to have 3 channels, has " + to_string(channels));
    }
    return readSuccess;
}

bool AndreiUtils::readColorImageBinary(std::ifstream *fin, uint8_t *image, int &height, int &width,
                                       StandardTypes &type, const int nrBytes) {
    int channels;
    bool readSuccess = readImageBinary(fin, image, height, width, type, channels, nrBytes);
    if (readSuccess && channels != 3) {
        throw runtime_error("Expected image to have 3 channels, has " + to_string(channels));
    }
    return readSuccess;
}

bool AndreiUtils::readDepthImageBinary(std::ifstream *fin, uint16_t *&depth, int &height, int &width) {
    int channels;
    StandardTypes type;
    bool readSuccess = readImageBinary(fin, (uint8_t *&) depth, height, width, type, channels);
    if (!readSuccess) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != StandardTypes::TYPE_UINT_16) {
        throw runtime_error("Expected depth to have type uint16, has " + to_string(type));
    }
    return true;
}

bool AndreiUtils::readDepthImageBinary(std::ifstream *fin, uint16_t *depth, int &height, int &width,
                                       const int nrBytes) {
    int channels;
    StandardTypes type;
    bool readSuccess = readImageBinary(fin, (uint8_t *&) depth, height, width, type, channels);
    if (!readSuccess) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != StandardTypes::TYPE_UINT_16) {
        throw runtime_error("Expected depth to have type uint16, has " + to_string(type));
    }
    return true;
}

bool AndreiUtils::readDepthImageBinary(std::ifstream *fin, double *&depth, int &height, int &width) {
    int channels;
    StandardTypes type;

    if (!readImageHeader(fin, height, width, type, channels) && fin->peek() == EOF) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != StandardTypes::TYPE_UINT_16) {
        throw runtime_error("Expected depth to have type uint16, has " + to_string(type));
    }

    int nrElements = height * width;
    auto *tmpData = new uint16_t[nrElements];
    int nrBytes = nrElements * (int) sizeof(uint16_t);
    bool readSuccess = readImageData(fin, (uint8_t *) tmpData, nrBytes);
    if (readSuccess) {
        // don't return now because tmpData will not be deleted
        delete[] depth;
        depth = new double[nrElements];
        for (int i = 0; i < nrElements; i++) {
            depth[i] = tmpData[i] / 1000.0;
        }
    }

    delete[] tmpData;
    return true;
}

bool AndreiUtils::readDepthImageBinary(std::ifstream *fin, double *depth, int &height, int &width, const int nrBytes) {
    int channels;
    StandardTypes type;

    if (!readImageHeader(fin, height, width, type, channels) && fin->peek() == EOF) {
        return false;
    }
    if (channels != 1) {
        throw runtime_error("Expected depth to have 1 channels, has " + to_string(channels));
    }
    if (type != StandardTypes::TYPE_UINT_16) {
        throw runtime_error("Expected depth to have type uint16, has " + to_string(type));
    }

    int nrElements = height * width;
    if (nrElements * sizeof(double) != nrBytes) {
        throw runtime_error("Image data container does not have the same size as the file-stored image: " +
                            to_string(nrBytes) + " vs. " + to_string(nrElements * sizeof(double)));
    }

    auto *tmpData = new uint16_t[nrElements];
    int tmpNrBytes = nrElements * (int) sizeof(uint16_t);
    bool readSuccess = readImageData(fin, (uint8_t *) tmpData, tmpNrBytes);
    if (readSuccess) {
        // don't return now because tmpData will not be deleted
        delete[] depth;
        depth = new double[nrElements];
        for (int i = 0; i < nrElements; i++) {
            depth[i] = tmpData[i] / 1000.0;
        }
    }

    delete[] tmpData;
    return true;
}

void AndreiUtils::writeImageBinary(std::ofstream *fout, const uint8_t *image, int height, int width, StandardTypes type,
                                   int channels) {
    fout->write((char *) &height, sizeof(int));
    fout->write((char *) &width, sizeof(int));
    fout->write((char *) &type, sizeof(int));
    fout->write((char *) &channels, sizeof(int));
    fout->write((char *) image, height * width * channels * getStandardTypeByteAmount(type));
}

void AndreiUtils::writeColorImageBinary(std::ofstream *fout, const uint8_t *image, int height, int width,
                                        StandardTypes type) {
    writeImageBinary(fout, image, height, width, type, 3);
}

void AndreiUtils::writeDepthImageBinary(std::ofstream *fout, const uint16_t *depth, int height, int width) {
    writeImageBinary(fout, (uint8_t *) depth, height, width, StandardTypes::TYPE_UINT_16, 1);
}

void AndreiUtils::writeDepthImageBinary(std::ofstream *fout, const double *depth, int height, int width) {
    size_t nrElements = height * width;
    auto *tmpData = new uint16_t[nrElements];
    for (size_t i = 0; i < nrElements; i++) {
        tmpData[i] = (uint16_t) (depth[i] * 1000);
    }
    writeImageBinary(fout, (uint8_t *) tmpData, height, width, StandardTypes::TYPE_UINT_16, 1);
    delete[] tmpData;
}