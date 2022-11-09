//
// Created by Andrei on 21.10.22.
//

#ifndef ANDREIUTILS_UTILSQUATERNIONS_HPP
#define ANDREIUTILS_UTILSQUATERNIONS_HPP

#include <AndreiUtils/classes/TypeHelper.hpp>
#include <Eigen/Dense>
#include <vector>

namespace AndreiUtils {
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
    Eigen::Quaternion<T> qxRotation(CR<T> angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitX()));
    }

    template<class T>
    Eigen::Quaternion<T> qyRotation(CR<T> angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitY()));
    }

    template<class T>
    Eigen::Quaternion<T> qzRotation(CR<T> angle) {
        return Eigen::Quaternion<T>(Eigen::AngleAxis<T>(angle, Eigen::Matrix<T, 3, 1>::UnitZ()));
    }

    template<class T>
    Eigen::Quaternion<T> qMulScalar(CR<Eigen::Quaternion<T>> q, CR<T> scalar) {
        return Eigen::Quaternion<T>(q.coeffs() * scalar);
    }

    template<class T>
    Eigen::Quaternion<T> qDivScalar(CR<Eigen::Quaternion<T>> q, CR<T> scalar) {
        return Eigen::Quaternion<T>(q.coeffs() / scalar);
    }

    // q1 == q2
    template<class T>
    bool qEqual(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2, CR<T> tol = T(1e-9)) {
        return equal(q1.w(), q2.w(), tol) && equal(q1.x(), q2.x(), tol) && equal(q1.y(), q2.y(), tol) &&
               equal(q1.z(), q2.z(), tol);
    }

    // q1 != q2
    template<class T>
    bool qNotEqual(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2, CR<T> tol = T(1e-9)) {
        return !qEqual(q1, q2, tol);
    }

    // q1 + q2
    template<class T>
    Eigen::Quaternion<T> qAdd(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2) {
        return Eigen::Quaternion<T>(q1.coeffs() + q2.coeffs());
    }

    // q1 += q2
    template<class T>
    void qIncrement(Eigen::Quaternion<T> &q1, CR<Eigen::Quaternion<T>> q2) {
        q1.coeffs() += q2.coeffs();
    }

    // q1 - q2
    template<class T>
    Eigen::Quaternion<T> qDiff(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2) {
        return Eigen::Quaternion<T>(q1.coeffs() - q2.coeffs());
    }

    // q1 - q2: alias for qDiff
    template<class T>
    Eigen::Quaternion<T> qSub(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2) {
        return qDiff(q1, q2);
    }

    // q1 -= q2
    template<class T>
    void qDecrement(Eigen::Quaternion<T> &q1, CR<Eigen::Quaternion<T>> q2) {
        q1.coeffs() -= q2.coeffs();
    }

    template<class T>
    double qCoefficientSquareSum(CR<Eigen::Quaternion<T>> q) {
        return q.coeffs().squaredNorm();
    }

    // -q
    template<class T>
    Eigen::Quaternion<T> qNeg(CR<Eigen::Quaternion<T>> q) {
        return Eigen::Quaternion<T>(-q.coeffs());
    }

    // log(q)
    template<class T>
    Eigen::Quaternion<T> qLog(CR<Eigen::Quaternion<T>> q) {
        T qNorm = q.norm();
        Eigen::Matrix<T, 3, 1> x = acos(q.w() / qNorm) * q.vec().normalized();
        return Eigen::Quaternion<T>(log(qNorm), x.x(), x.y(), x.z());
    }

    // exp(q)
    template<class T>
    Eigen::Quaternion<T> qExp(CR<Eigen::Quaternion<T>> q) {
        Eigen::Matrix<T, 3, 1> x = q.vec();
        T xNorm = x.norm();
        return qMulScalar(Eigen::Quaternion<T>(Eigen::AngleAxis<T>(xNorm, x.normalized())), exp(q.w()));
    }

    // orientation: q1 == q2
    template<class T>
    bool qEqualOrientation(CR<Eigen::Quaternion<T>> q1, CR<Eigen::Quaternion<T>> q2, CR<T> tol = T(1e-9)) {
        return qEqual(q1, q2) || qEqual(q1, qNeg(q2));
    }

    template<class T>
    Eigen::Quaternion<T> vToQ(CR<Eigen::Matrix<T, 3, 1>> v) {
        Eigen::Quaternion<T> q;
        q.w() = 0;
        q.vec() = v;
        return q;
    }

    template<class T>
    Eigen::Matrix<T, 3, 1> qRotate(CR<Eigen::Quaternion<T>> q, CR<Eigen::Matrix<T, 3, 1>> v) {
        return (q * vToQ(v) * q.inverse()).vec();
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    //*/
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(CP<T> angles, CR<size_t> size, CR<std::string> format = "xyz") {
        Eigen::Quaternion<T> q = qIdentity<T>();
        if (size > format.size()) {
            throw std::runtime_error("Not all angles have the format specified!");
        }
        for (int i = 0; i < size; i++) {
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
    //*/
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(CR<std::vector<T>> angles, CR<std::string> format = "xyz") {
        return qFromEulerAngles(angles.data(), angles.size(), format);
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    */
    template<class T>
    Eigen::Quaternion<T> qFromEulerAngles(CR<Eigen::Matrix<T, 3, 1>> angles, CR<std::string> format = "xyz") {
        return qFromEulerAngles(angles.data(), 3, format);
    }

    template<class T>
    Eigen::Quaternion<T> qFromRotationMatrix(CR<Eigen::Matrix<T, 3, 3>> rot) {
        return Eigen::Quaternion<T>(rot);
    }

    /**
    Format specifies the following: xyz means R has the form R_z * R_y * R_x, when rotating a point p like this: R * p
    */
    template<class T>
    Eigen::Matrix<T, 3, 1> eulerAnglesFromQ(CR<Eigen::Quaternion<T>> q, CR<std::string> format = "xyz") {
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

    template<class T>
    Eigen::Matrix<T, 3, 1> qToRPY(CR<Eigen::Quaternion<T>> q) {
        // first rotate around X, then round Y and finally around Z => R = Rx * Ry * Rz
        // x points left, y points front, z point up
        Eigen::Matrix<T, 3, 1> euler;

        double sqw = q.w() * q.w(), sqx = q.x() * q.x(), sqy = q.y() * q.y(), sqz = q.z() * q.z();

        // If quaternion is normalised the unit is one, otherwise it is the correction factor
        double unit = sqx + sqy + sqz + sqw, test = q.x() * q.z() + q.y() * q.w();

        if (test > 0.4999 * unit) {
            // 0.4999f OR 0.5f - EPSILON: Singularity at North Pole
            euler.x() = 0;                                                                // Roll
            euler.y() = M_PI * 0.5f;                                                      // Pitch
            euler.z() = atan2(2 * q.w() * q.z() + 2 * q.x() * q.y(), sqw - sqx + sqy - sqz);  // Yaw
        } else if (test < -0.4999f * unit) {
            // -0.4999f OR -0.5f + EPSILON: Singularity at South Pole
            euler.x() = 0;                                                                // Roll
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
    ostream &operator<<(ostream &os, CR<Eigen::Quaternion<T>> q) {
        os << q.w() << " " << q.x() << " " << q.y() << " " << q.z();
        return os;
    }

    template<class T>
    istream &operator>>(istream &is, Eigen::Quaternion<T> &q) {
        is >> q.w() >> q.x() >> q.y() >> q.z();
        return is;
    }
}

#endif //ANDREIUTILS_UTILSQUATERNIONS_HPP
