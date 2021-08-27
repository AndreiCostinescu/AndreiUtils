//
// Created by andrei on 27.08.21.
//

#include <andrei_utils/utilsEigenOpenCV.h>

using namespace cv;
using namespace std;

Eigen::Array3f cvPointToEigenArray(const Point3f &point) {
    Eigen::Array3f x;
    x.x() = point.x;
    x.y() = point.y;
    x.z() = point.z;
    return x;
}

cv::Point3f eigenArrayToCVPoint(const Eigen::Array3f &array) {
    return {array.x(), array.y(), array.z()};
}

void writeEigenArray(FileStorage &fs, const Eigen::ArrayXf &x) {
    fs << "{";
    int size = x.size();
    fs << "size" << size;
    for (int i = 0; i < size; i++) {
        fs << "elem_" + std::to_string(i) << x[i];
    }
    fs << "}";
}

void readEigenArray(const FileNode &node, Eigen::ArrayXf &x) {
    int size = (int) node["size"];
    x = Eigen::ArrayXf::Zero(size);
    for (int i = 0; i < size; i++) {
        x[i] = (float) node["elem_" + std::to_string(i)];
    }
}

void write(FileStorage &fs, const string &name, const Eigen::ArrayXf &x) {
    // internal::WriteStructContext ws(fs, name, FileNode::SEQ + FileNode::FLOW);
    writeEigenArray(fs, x);
}

void read(const FileNode &node, Eigen::ArrayXf &x, const Eigen::ArrayXf &default_value) {
    if (node.empty()) {
        x = default_value;
    } else {
        readEigenArray(node, x);
    }
}

void write(FileStorage &fs, const string &name, const Eigen::Array<float, 3, 1> &x) {
    write(fs, name, (const Eigen::ArrayXf &) x);
}

void read(const FileNode &node, Eigen::Array<float, 3, 1> &x, const Eigen::Array<float, 3, 1> &default_value) {
    read(node, (Eigen::ArrayXf &) x, default_value);
}
