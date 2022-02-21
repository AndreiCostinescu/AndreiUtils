//
// Created by Andrei on 06.09.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_HPP
#define ANDREIUTILS_UTILSEIGEN_HPP

#include <Eigen/Dense>
#include <string>
#include <vector>

namespace AndreiUtils {
    // https://gist.github.com/javidcf/25066cf85e71105d57b6
    template<class MatT>
    Eigen::Matrix<typename MatT::Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> pseudoinverse(
            const MatT &mat, typename MatT::Scalar tolerance = typename MatT::Scalar{1e-4}) {
        typedef typename MatT::Scalar Scalar;
        auto svd = mat.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
        const auto &singularValues = svd.singularValues();
        Eigen::Matrix<Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> singularValuesInv(mat.cols(),
                                                                                                  mat.rows());
        singularValuesInv.setZero();
        for (unsigned int i = 0; i < singularValues.size(); ++i) {
            if (singularValues(i) > tolerance) {
                singularValuesInv(i, i) = Scalar{1} / singularValues(i);
            } else {
                singularValuesInv(i, i) = Scalar{0};
            }
        }
        return svd.matrixV() * singularValuesInv * svd.matrixU().adjoint();
    }

    template<class T>
    void qSetZero(Eigen::Quaternion<T> &q) {
        q.w() = 0;
        q.x() = 0;
        q.y() = 0;
        q.z() = 0;
    }

    template<class T>
    Eigen::Quaternion<T> qxRotation(const T &angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitX()));
    }

    template<class T>
    Eigen::Quaternion<T> qyRotation(const T &angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitY()));
    }

    template<class T>
    Eigen::Quaternion<T> qzRotation(const T &angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitZ()));
    }

    template<class T>
    Eigen::Quaternion<T> qMulScalar(const Eigen::Quaternion<T> &q, const T &scalar) {
        return Eigen::Quaternion<T>(q.coeffs() * scalar);
    }

    template<class T>
    Eigen::Quaternion<T> qDivScalar(const Eigen::Quaternion<T> &q, const T &scalar) {
        return Eigen::Quaternion<T>(q.coeffs() / scalar);
    }

    template<class T>
    Eigen::Quaternion<T> qAdd(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        return Eigen::Quaternion<T>(q1.coeffs() + q2.coeffs());
    }

    template<class T>
    void qIncrement(Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        q1.coeffs() += q2.coeffs();
    }

    template<class T>
    Eigen::Quaternion<T> qDiff(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        return Eigen::Quaternion<T>(q1.coeffs() - q2.coeffs());
    }

    template<class T>
    void qDecrement(Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        q1.coeffs() -= q2.coeffs();
    }

    template<class T>
    Eigen::Quaternion<T> qNeg(const Eigen::Quaternion<T> &q) {
        return Eigen::Quaternion<T>(-q.coeffs());
    }

    template<class T>
    Eigen::Quaternion<T> vToQ(const Eigen::Matrix<T, 3, 1> &v) {
        Eigen::Quaternion<T> q;
        q.w() = 0;
        q.vec() = v;
        return q;
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    */
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(const std::vector<T> &angles, const std::string &format = "xyz") {
        Eigen::Quaternion<T> q = Eigen::Quaternion<T>::Identity();
        if (angles.size() > format.size()) {
            throw std::runtime_error("Not all angles have the format specified!");
        }
        for (int i = 0; i < angles.size(); i++) {
            if (format[i] == 'x') {
                q = qxRotation(angles[i]) * q;
            } else if (format[i] == 'y') {
                q = qyRotation(angles[i]) * q;
            } else if (format[i] == 'z') {
                q = qzRotation(angles[i]) * q;
            } else {
                throw std::runtime_error(
                        std::string("Unknown axis format ") + format[i] + " for angle: " + std::to_string(i) + "!");
            }
        }
        return q;
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    */
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(const Eigen::Matrix<T, 3, 1> &angles, const std::string &format = "xyz") {
        Eigen::Quaternion<T> q = Eigen::Quaternion<T>::Identity();
        if (3 > format.size()) {
            throw std::runtime_error("Not all angles have the format specified!");
        }
        for (int i = 0; i < 3; i++) {
            if (format[i] == 'x') {
                q = qxRotation(angles.row(i)) * q;
            } else if (format[i] == 'y') {
                q = qyRotation(angles.row(i)) * q;
            } else if (format[i] == 'z') {
                q = qzRotation(angles.row(i)) * q;
            } else {
                throw std::runtime_error(
                        std::string("Unknown axis format ") + format[i] + " for angle: " + std::to_string(i) + "!");
            }
        }
        return q;
    }

    template<class T>
    Eigen::Quaternion<T> qFromRotationMatrix(const Eigen::Matrix<T, 3, 3> &rot) {
        return Eigen::Quaternion<T>(rot);
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    */
    template<class T>
    Eigen::Matrix<T, 3, 1> eulerAnglesFromQ(const Eigen::Quaternion<T> &q, const std::string &format = "xyz") {
        if (format.size() < 3) {
            throw std::runtime_error("Format " + format + " does not specify axis for each euler angle value");
        }
        int indices[3];
        for (int i = 2; i >= 0; i--) {
            if (format[i] == 'x') {
                indices[2 - i] = 0;
            } else if (format[i] == 'y') {
                indices[2 - i] = 1;
            } else if (format[i] == 'z') {
                indices[2 - i] = 2;
            } else {
                throw std::runtime_error(
                        std::string("Unknown axis format ") + format[i] + " for angle: " + std::to_string(i) + "!");
            }
        }
        auto e = q.toRotationMatrix().eulerAngles(indices[0], indices[1], indices[2]);
        std::swap(e.x(), e.z());
        return e;
    }

    template<class T, int N>
    Eigen::Matrix<T, -1, N> convertVectorsToMatrixRows(const std::vector<Eigen::Matrix<T, N, 1>> &rows) {
        Eigen::Matrix<T, -1, N> m = Eigen::Matrix<T, -1, N>::Zero(rows.size(), N);
        for (int i = 0; i < m.rows(); i++) {
            m.row(i) = rows[i];
        }
        return m;
    }

    template<class T>
    Eigen::Matrix<T, -1, 1> convertValuesToVector(const std::vector<T> &values) {
        Eigen::Matrix<T, -1, 1> v = Eigen::Matrix<T, -1, 1>::Zero(values.size(), 1);
        for (int i = 0; i < values.size(); i++) {
            v[i] = values[i];
        }
        return v;
    }

    template<class T>
    Eigen::Matrix<T, 1, -1> convertValuesToRowVector(const std::vector<T> &values) {
        Eigen::Matrix<T, 1, -1> v = Eigen::Matrix<T, 1, -1>::Zero(1, values.size());
        for (int i = 0; i < values.size(); i++) {
            v[i] = values[i];
        }
        return v;
    }
}

namespace std {
    template<class T>
    ostream &operator<<(ostream &os, const Eigen::Quaternion<T> &q) {
        os << q.w() << " " << q.x() << " " << q.y() << " " << q.z();
        return os;
    }
}

#endif //ANDREIUTILS_UTILSEIGEN_HPP
