//
// Created by Andrei on 06.09.21.
//

#ifndef ANDREIUTILS_UTILSEIGEN_HPP
#define ANDREIUTILS_UTILSEIGEN_HPP

#include <AndreiUtils/utils.hpp>
#include <Eigen/Dense>
#include <sstream>
#include <string>
#include <vector>

namespace AndreiUtils {
    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::string eigenToString(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &eigenData) {
        std::stringstream ss;
        ss << eigenData;
        return ss.str();
    }

    template<typename S1, int R1, int C1, int O1, int MR1, int MC1, typename S2, int R2, int C2, int O2, int MR2, int MC2>
    bool checkSameSize(const Eigen::Matrix<S1, R1, C1, O1, MR1, MC1> &a,
                       const Eigen::Matrix<S2, R2, C2, O2, MR2, MC2> &b) {
        return a.rows() == b.rows() && a.cols() == b.cols();
    }

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::vector<Eigen::Matrix<Scalar, Cols, 1, Options, MaxCols, 1>> getMatrixRowsAsVector(
            const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m) {
        std::vector<Eigen::Matrix<Scalar, Cols, 1, Options, MaxCols, 1>> res(m.rows());
        for (int i = 0; i < m.rows(); i++) {
            res[i] = m.row(i);
        }
        return res;
    }

    template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    std::vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> getMatrixColsAsVector(
            const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &m) {
        std::vector<Eigen::Matrix<Scalar, Rows, 1, Options, MaxRows, 1>> res(m.cols());
        for (int i = 0; i < m.cols(); i++) {
            res[i] = m.col(i);
        }
        return res;
    }

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
    Eigen::Matrix<T, 3, 1> xAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitX();
        return a;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> yAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitY();
        return a;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> zAxis3d() {
        const static Eigen::Matrix<T, 3, 1> a = Eigen::Matrix<T, 3, 1>::UnitZ();
        return a;
    }

    template<class T>
    void qSetZero(Eigen::Quaternion<T> &q) {
        q.w() = 0;
        q.x() = 0;
        q.y() = 0;
        q.z() = 0;
    }

    template<class T>
    Eigen::Quaternion<T> qZero() {
        Eigen::Quaternion<T> q;
        qSetZero(q);
        return q;
    }

    template<class T>
    Eigen::Quaternion<T> qIdentity() {
        return Eigen::Quaternion<T>::Identity();
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

    // q1 == q2
    template<class T>
    bool qEqual(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2, T const &tol = T(1e-9)) {
        return equal(q1.w(), q2.w(), tol) && equal(q1.x(), q2.x(), tol) && equal(q1.y(), q2.y(), tol) &&
               equal(q1.z(), q2.z(), tol);
    }

    // q1 != q2
    template<class T>
    bool qNotEqual(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2, T const &tol = T(1e-9)) {
        return !qEqual(q1, q2, tol);
    }

    // q1 + q2
    template<class T>
    Eigen::Quaternion<T> qAdd(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        return Eigen::Quaternion<T>(q1.coeffs() + q2.coeffs());
    }

    // q1 += q2
    template<class T>
    void qIncrement(Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        q1.coeffs() += q2.coeffs();
    }

    // q1 - q2
    template<class T>
    Eigen::Quaternion<T> qDiff(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        return Eigen::Quaternion<T>(q1.coeffs() - q2.coeffs());
    }

    // q1 - q2: alias for qDiff
    template<class T>
    Eigen::Quaternion<T> qSub(const Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        return qDiff(q1, q2);
    }

    // q1 -= q2
    template<class T>
    void qDecrement(Eigen::Quaternion<T> &q1, const Eigen::Quaternion<T> &q2) {
        q1.coeffs() -= q2.coeffs();
    }

    template<class T>
    double qCoefficientSquareSum(Eigen::Quaternion<T> const &q) {
        return q.coeffs().squaredNorm();
    }

    // -q
    template<class T>
    Eigen::Quaternion<T> qNeg(const Eigen::Quaternion<T> &q) {
        return Eigen::Quaternion<T>(-q.coeffs());
    }

    // log(q)
    template<class T>
    Eigen::Quaternion<T> qLog(const Eigen::Quaternion<T> &q) {
        T qNorm = q.norm();
        Eigen::Matrix<T, 3, 1> x = acos(q.w() / qNorm) * q.vec().normalized();
        return Eigen::Quaternion<T>(log(qNorm), x.x(), x.y(), x.z());
    }

    // exp(q)
    template<class T>
    Eigen::Quaternion<T> qExp(const Eigen::Quaternion<T> &q) {
        Eigen::Matrix<T, 3, 1> x = q.vec();
        T xNorm = x.norm();
        return qMulScalar(Eigen::Quaternion<T>(Eigen::AngleAxis<T>(xNorm, x.normalized())), exp(q.w()));
    }

    template<class T>
    Eigen::Quaternion<T> vToQ(const Eigen::Matrix<T, 3, 1> &v) {
        Eigen::Quaternion<T> q;
        q.w() = 0;
        q.vec() = v;
        return q;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> qRotate(const Eigen::Quaternion<T> &q, const Eigen::Matrix<T, 3, 1> &v) {
        return (q * vToQ(v) * q.inverse()).vec();
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    //*/
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(const std::vector<T> &angles, const std::string &format = "xyz") {
        Eigen::Quaternion<T> q = qIdentity<T>();
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
        swapData(e.x(), e.z());
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

    template<class T>
    Eigen::Matrix<T, 3, 1> qToRPY(const Eigen::Quaternion<T> &q) {
        // first rotate around X, then round Y and finally around Z => R = Rx * Ry * Rz
        // x points left, y points front, z point up
        Eigen::Matrix<T, 3, 1> euler;

        double sqw = q.w() * q.w(), sqx = q.x() * q.x(), sqy = q.y() * q.y(), sqz = q.z() * q.z();

        // If quaternion is normalised the unit is one, otherwise it is the correction factor
        double unit = sqx + sqy + sqz + sqw, test = q.x() * q.z() + q.y() * q.w();

        if (test > 0.4999 * unit) {
            // 0.4999f OR 0.5f - EPSILON: Singularity at north pole
            euler.x() = 0;                                                                         // Roll
            euler.y() = M_PI * 0.5f;                                                      // Pitch
            euler.z() = atan2(2 * q.w() * q.z() + 2 * q.x() * q.y(), sqw - sqx + sqy - sqz);  // Yaw
        } else if (test < -0.4999f * unit) {
            // -0.4999f OR -0.5f + EPSILON: Singularity at south pole
            euler.x() = 0;                                                                         // Roll
            euler.y() = -M_PI * 0.5f;                                                     // Pitch
            euler.z() = atan2(2 * q.w() * q.z() + 2 * q.x() * q.y(), sqw - sqx + sqy - sqz);  // Yaw
        } else {
            euler.x() = atan2(2 * q.x() * q.w() - 2 * q.y() * q.z(), sqw - sqx - sqy + sqz);  // Roll
            euler.y() = asin(2 * test / unit);                                         // Pitch
            euler.z() = atan2(2 * q.z() * q.w() - 2 * q.x() * q.y(), sqw + sqx - sqy - sqz);  // Yaw
        }

        // return (float)(180 / M_PI) * euler;  // convert to degrees
        return euler;
    }
}

namespace std {
    template<class T>
    ostream &operator<<(ostream &os, const Eigen::Quaternion<T> &q) {
        os << q.w() << " " << q.x() << " " << q.y() << " " << q.z();
        return os;
    }

    template<class T>
    istream &operator>>(istream &is, Eigen::Quaternion<T> &q) {
        is >> q.w() >> q.x() >> q.y() >> q.z();
        return is;
    }
}

#endif //ANDREIUTILS_UTILSEIGEN_HPP
