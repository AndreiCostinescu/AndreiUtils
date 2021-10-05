//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsEigenOpenCV.h>

using namespace Eigen;
using namespace std;

Array3f AndreiUtils::cvPointToEigenArray(const cv::Point3f &point) {
    Array3f x;
    x.x() = point.x;
    x.y() = point.y;
    x.z() = point.z;
    return x;
}

cv::Point3f AndreiUtils::eigenArrayToCVPoint(const Array3f &array) {
    return {array.x(), array.y(), array.z()};
}

Vector3f AndreiUtils::cvPointToEigenVector(const cv::Point3f &point) {
    Vector3f x;
    x.x() = point.x;
    x.y() = point.y;
    x.z() = point.z;
    return x;
}

cv::Point3f AndreiUtils::eigenVectorToCVPoint(const Vector3f &vector) {
    return {vector.x(), vector.y(), vector.z()};
}

void AndreiUtils::writeEigenArray(cv::FileStorage &fs, const ArrayXf &x) {
    fs << "{";
    int size = x.size();
    fs << "size" << size;
    for (int i = 0; i < size; i++) {
        fs << "elem_" + std::to_string(i) << x[i];
    }
    fs << "}";
}

void AndreiUtils::readEigenArray(const cv::FileNode &node, ArrayXf &x) {
    int size = (int) node["size"];
    x = ArrayXf::Zero(size);
    for (int i = 0; i < size; i++) {
        x[i] = (float) node["elem_" + std::to_string(i)];
    }
}

void AndreiUtils::write(cv::FileStorage &fs, const string &name, const ArrayXf &x) {
    // internal::WriteStructContext ws(fs, name, FileNode::SEQ + FileNode::FLOW);
    writeEigenArray(fs, x);
}

void AndreiUtils::read(const cv::FileNode &node, ArrayXf &x, const ArrayXf &default_value) {
    if (node.empty()) {
        x = default_value;
    } else {
        readEigenArray(node, x);
    }
}

void AndreiUtils::write(cv::FileStorage &fs, const string &name, const Array<float, 3, 1> &x) {
    write(fs, name, (const ArrayXf &) x);
}

void AndreiUtils::read(const cv::FileNode &node, Array<float, 3, 1> &x, const Array<float, 3, 1> &default_value) {
    read(node, (ArrayXf &) x, default_value);
}
