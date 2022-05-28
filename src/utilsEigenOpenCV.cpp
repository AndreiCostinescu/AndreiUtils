//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/utilsEigenOpenCV.h>

using namespace AndreiUtils;
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

Eigen::Matrix4d AndreiUtils::recoverMatPoseFrom2dAnd3dPoints(
        const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d, double fx, double fy, double ppx,
        double ppy, float distortionCoefficients[5]) {
    cv::Mat tvec;
    cv::Matx33d r;
    recoverPoseFrom2dAnd3dPoints(tvec, r, points2d, points3d, fx, fy, ppx, ppy, distortionCoefficients);

    Eigen::Matrix3d wRo;
    wRo << r(0, 0), r(0, 1), r(0, 2), r(1, 0), r(1, 1), r(1, 2), r(2, 0), r(2, 1), r(2, 2);

    Eigen::Matrix4d T;
    T.topLeftCorner(3, 3) = wRo;
    T.col(3).head(3) << tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2);
    T.row(3) << 0, 0, 0, 1;

    return T;
}

Eigen::Matrix4d AndreiUtils::recoverMatPoseFrom2dAnd3dPoints(
        const vector<Vector2f> &eigenPoints2d, const vector<Vector3f> &eigenPoints3d, double fx, double fy, double ppx,
        double ppy, float distortionCoefficients[5]) {
    size_t n = eigenPoints3d.size();
    std::vector<cv::Point2f> points2d(n);
    std::vector<cv::Point3f> points3d(n);
    for (int i = 0; i < n; i++) {
        points2d[i].x = eigenPoints2d[i].x();
        points2d[i].y = eigenPoints2d[i].y();
        points3d[i].x = eigenPoints3d[i].x();
        points3d[i].y = eigenPoints3d[i].y();
        points3d[i].z = eigenPoints3d[i].z();
    }
    return recoverMatPoseFrom2dAnd3dPoints(points2d, points3d, fx, fy, ppx, ppy, distortionCoefficients);
}

Eigen::Matrix4d AndreiUtils::recoverMatPoseFrom2dAnd3dPoints(
        const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        const CameraIntrinsicParameters &intrinsics) {
    return recoverMatPoseFrom2dAnd3dPoints(points2d, points3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                           intrinsics.ppy, intrinsics.distortionCoefficients);
}

Eigen::Matrix4d AndreiUtils::recoverMatPoseFrom2dAnd3dPoints(
        const vector<Vector2f> &eigenPoints2d, const vector<Vector3f> &eigenPoints3d,
        const CameraIntrinsicParameters &intrinsics) {
    return recoverMatPoseFrom2dAnd3dPoints(eigenPoints2d, eigenPoints3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                           intrinsics.ppy, intrinsics.distortionCoefficients);
}

DualQuaternion<double> AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d, double fx, double fy, double ppx,
        double ppy, float distortionCoefficients[5]) {
    cv::Mat tvec;
    cv::Matx33d r;
    recoverPoseFrom2dAnd3dPoints(tvec, r, points2d, points3d, fx, fy, ppx, ppy, distortionCoefficients);

    Eigen::Matrix3d wRo;
    wRo << r(0, 0), r(0, 1), r(0, 2), r(1, 0), r(1, 1), r(1, 2), r(2, 0), r(2, 1), r(2, 2);

    return {qFromRotationMatrix(wRo), {tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2)}};
}

DualQuaternion<double> AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        const vector<Vector2f> &eigenPoints2d, const vector<Vector3f> &eigenPoints3d, double fx, double fy, double ppx,
        double ppy, float distortionCoefficients[5]) {
    size_t n = eigenPoints3d.size();
    std::vector<cv::Point2f> points2d(n);
    std::vector<cv::Point3f> points3d(n);
    for (int i = 0; i < n; i++) {
        points2d[i].x = eigenPoints2d[i].x();
        points2d[i].y = eigenPoints2d[i].y();
        points3d[i].x = eigenPoints3d[i].x();
        points3d[i].y = eigenPoints3d[i].y();
        points3d[i].z = eigenPoints3d[i].z();
    }
    return recoverPoseFrom2dAnd3dPoints(points2d, points3d, fx, fy, ppx, ppy, distortionCoefficients);
}

DualQuaternion<double> AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        const vector<cv::Point2f> &points2d, const vector<cv::Point3f> &points3d,
        const CameraIntrinsicParameters &intrinsics) {
    return recoverPoseFrom2dAnd3dPoints(points2d, points3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                        intrinsics.ppy, intrinsics.distortionCoefficients);
}

DualQuaternion<double> AndreiUtils::recoverPoseFrom2dAnd3dPoints(
        const vector<Vector2f> &eigenPoints2d, const vector<Vector3f> &eigenPoints3d,
        const CameraIntrinsicParameters &intrinsics) {
    return recoverPoseFrom2dAnd3dPoints(eigenPoints2d, eigenPoints3d, intrinsics.fx, intrinsics.fy, intrinsics.ppx,
                                        intrinsics.ppy, intrinsics.distortionCoefficients);
}
