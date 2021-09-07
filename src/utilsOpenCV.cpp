//
// Created by andrei on 26.11.20.
//

#include <AndreiUtils/utilsOpenCV.h>

using namespace cv;
using namespace std;

void AndreiUtils::imageRotation(Mat *image, RotationType rotation) {
    switch (rotation) {
        case LEFT_90: {
            cv::rotate(*image, *image, cv::ROTATE_90_COUNTERCLOCKWISE);
            break;
        }
        case LEFT_180: {
            cv::rotate(*image, *image, cv::ROTATE_180);
            break;
        }
        case LEFT_270: {
            cv::rotate(*image, *image, cv::ROTATE_90_CLOCKWISE);
            break;
        }
        default: {
            break;
        }
    }
}

void AndreiUtils::matWriteBinary(ofstream *fs, const Mat &mat) {
    // Header
    int type = mat.type();
    int channels = mat.channels();
    fs->write((char *) &mat.rows, sizeof(int));    // rows
    fs->write((char *) &mat.cols, sizeof(int));    // cols
    fs->write((char *) &type, sizeof(int));        // type
    fs->write((char *) &channels, sizeof(int));    // channels

    // Data
    if (mat.isContinuous()) {
        fs->write(mat.ptr<char>(0), (mat.dataend - mat.datastart));
    } else {
        int rowSize = CV_ELEM_SIZE(type) * mat.cols;
        for (int r = 0; r < mat.rows; ++r) {
            fs->write(mat.ptr<char>(r), rowSize);
        }
    }
}

bool AndreiUtils::matReadBinary(ifstream *fs, Mat *result) {
    // Header
    int rows, cols, type, channels;
    if (fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &rows, sizeof(int));         // rows
    if (fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &cols, sizeof(int));         // cols
    if (fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &type, sizeof(int));         // type
    if (fs->peek() == EOF) {
        return false;
    }
    fs->read((char *) &channels, sizeof(int));     // channels
    if (fs->peek() == EOF) {
        return false;
    }

    // Data
    Mat mat(rows, cols, type);
    fs->read((char *) mat.data, CV_ELEM_SIZE(type) * rows * cols);

    *result = mat;
    return true;
}

void AndreiUtils::printImage(Mat *image, const char *title, bool verbose) {
    if (verbose) {
        cout << "Printing " << title << "... " << image << endl;
    }
    if (image != nullptr && !image->empty()) {
        imshow(title, *image);
    } else if (image != nullptr) {
        assert(image->empty());
        cerr << title << " is empty!" << endl;
    } else {
        assert(image == nullptr);
        cerr << title << " is null!" << endl;
    }
}

void AndreiUtils::printImages(const vector<Mat *> &images, const vector<string> &titles) {
    assert (images.size() >= titles.size());
    for (int i = 0; i < images.size(); i++) {
        printImage(images[i], titles[i].c_str());
    }
    cout << "Printed!" << endl;
}

void AndreiUtils::convertDepthToMillimetersUInt16(const Mat *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMillimetersUInt16(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_16U, 1000);
}

void AndreiUtils::convertDepthToMetersDouble64(const Mat *depthMat, Mat &output) {
    depthMat->convertTo(output, CV_64F, 0.001);
}

void AndreiUtils::convertDepthToMetersDouble64(Mat *depthMat) {
    depthMat->convertTo(*depthMat, CV_64F, 0.001);
}
