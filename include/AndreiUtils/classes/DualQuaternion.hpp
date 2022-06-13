//
// Created by andrei on 09.02.22.
//

#ifndef ANDREIUTILS_DUALQUATERNION_HPP
#define ANDREIUTILS_DUALQUATERNION_HPP

#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsEigen.hpp>
#include <Eigen/Dense>

namespace AndreiUtils {
    template<class T>
    class DualQuaternion {
    public:
        static DualQuaternion identity() {
            DualQuaternion q;
            q.r.setIdentity();
            qSetZero(q.d);
            return q;
        }

        DualQuaternion() : r(qZero<double>()), d(qZero<double>()) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Quaternion<T> d) : r(r), d(d) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Matrix<T, 3, 1> t) : r(r) {
            this->d = qMulScalar(vToQ(t) * r, 0.5);
        }

        explicit DualQuaternion(Eigen::Matrix<T, 4, 4> t) :
                DualQuaternion(qFromRotationMatrix(Eigen::Matrix<double, 3, 3>(t.template block<3, 3>(0, 0))),
                                                   t.template block<3, 1>(0, 3)) {}

        virtual ~DualQuaternion() = default;

        double coefficientNorm() const {
            Eigen::Matrix<double, 8, 1> coefficients;
            coefficients.topRows(4) = this->r.coeffs().template cast<double>();
            coefficients.bottomRows(4) = this->d.coeffs().template cast<double>();
            return coefficients.norm();
        }

        double coefficientSquareSum() const {
            Eigen::Matrix<double, 8, 1> coefficients;
            coefficients.topRows(4) = this->r.coeffs().template cast<double>();
            coefficients.bottomRows(4) = this->d.coeffs().template cast<double>();
            return coefficients.squaredNorm();
        }

        void normalize() {
            T norm = this->r.norm();
            if (equal(norm, T(0))) {
                return;
            }
            (*this) /= norm;
        }

        DualQuaternion normalized() const {
            DualQuaternion res = *this;
            T norm = this->r.norm();
            if (equal(norm, T(0))) {
                return res;
            }
            res.r = qDivScalar(res.r, norm);
            res.d = qDivScalar(res.d, norm);
            return res;
        }

        DualQuaternion conjugate() const {
            return DualQuaternion(this->r.conjugate(), this->d.conjugate());
        }

        DualQuaternion quaternionConjugate() const {
            return this->conjugate();
        }

        DualQuaternion dualConjugate() const {
            return DualQuaternion(this->r, qNeg(this->d));
        }

        DualQuaternion quaternionDualConjugate() const {
            return DualQuaternion(this->r.conjugate(), this->d.conjugate()).dualConjugate();
        }

        DualQuaternion dualQuaternionInverse() const {
            DualQuaternion inv;
            inv.r = this->r.conjugate();
            inv.d = qNeg(inv.r * this->d * inv.r);
            return inv;
        }

        DualQuaternion operator*(const T &s) const {
            return DualQuaternion(qMulScalar(this->r, s), qMulScalar(this->d, s));
        }

        DualQuaternion &operator*=(const T &s) {
            *this = *this * s;
            return *this;
        }

        DualQuaternion operator/(const T &s) const {
            return DualQuaternion(qDivScalar(this->r, s), qDivScalar(this->d, s));
        }

        DualQuaternion &operator/=(const T &s) {
            *this = *this / s;
            return *this;
        }

        // q_T = a * b corresponds to T_a_b = T_a_i * T_i_b; *this * other
        DualQuaternion operator*(const DualQuaternion &other) const {
            /*
            Eigen::Quaternion<T> real = this->r * other.r;
            Eigen::Quaternion<T> dual((this->d * other.r).coeffs() + (this->r * other.d).coeffs());
            return DualQuaternion(real, dual);
            /*/
            return DualQuaternion(this->r * other.r, qAdd(this->d * other.r, this->r * other.d));
            //*/
        }

        DualQuaternion &operator*=(const DualQuaternion &other) {
            (*this) = (*this) * other;
            return *this;
        }

        DualQuaternion operator+(const DualQuaternion &other) const {
            return DualQuaternion(qAdd(this->r, other.r), qAdd(this->d, other.d));
        }

        DualQuaternion &operator+=(const DualQuaternion &other) {
            (*this) = (*this) + other;
            return *this;
        }

        DualQuaternion operator-() const {
            return DualQuaternion(qNeg(this->r), qNeg(this->d));
        }

        DualQuaternion operator-(const DualQuaternion &other) const {
            return (*this) + (-other);
        }

        DualQuaternion &operator-=(const DualQuaternion &other) {
            (*this) = (*this) - other;
            return (*this);
        }

        Eigen::Matrix<T, 3, 1> transform(const Eigen::Matrix<T, 3, 1> &p) const {
            return ((*this) * DualQuaternion(Eigen::Quaternion<T>::Identity(), vToQ(p)) *
                    this->quaternionDualConjugate()).getDual().vec();
        }

        Eigen::Matrix<T, 3, 1> rotate(const Eigen::Matrix<T, 3, 1> &v) const {
            return ((*this) * DualQuaternion(vToQ(v), qZero<double>()) *
                    this->quaternionDualConjugate()).getRotation().vec();
        }

        Eigen::Matrix<T, 3, 1> translate(const Eigen::Matrix<T, 3, 1> &t) const {
            return ((*this) * DualQuaternion(qZero<double>(), vToQ(t)) *
                    this->quaternionDualConjugate()).getTranslation();
        }

        DualQuaternion addRotation(const Eigen::Quaternion<T> &q) const {
            return DualQuaternion(q * this->r, this->getTranslation());
        }

        DualQuaternion addTranslation(const Eigen::Matrix<T, 3, 1> &t) const {
            return DualQuaternion(this->r, t + this->getTranslation());
        }

        Eigen::Quaternion<T> getRotation() const {
            return this->r;
        }

        Eigen::Matrix<T, 3, 3> getRotationAsMatrix() const {
            return this->r.toRotationMatrix();
        }

        Eigen::Quaternion<T> getDual() const {
            return this->d;
        }

        Eigen::Matrix<T, 3, 1> getTranslation() const {
            return qMulScalar(this->d * this->r.conjugate(), T(2)).vec();
        }

        Eigen::Matrix<T, 4, 4> getTransformationMatrix() const {
            auto q = this->normalized();
            Eigen::Matrix<T, 4, 4> M = Eigen::Matrix<T, 4, 4>::Identity();
            M.template block<3, 3>(0, 0) = q.getRotationAsMatrix();  // Extract rotational information
            M.col(3).template topRows<3>() = q.getTranslation();  // Extract translation information
            return M;
        }

        friend std::ostream &operator<<(std::ostream &os, const DualQuaternion &q) {
            os << q.r << " " << q.d;
            return os;
        }

        friend std::istream &operator>>(std::istream &is, DualQuaternion &q) {
            is >> q.r >> q.d;
            return is;
        }

    protected:
        Eigen::Quaternion<T> r;
        Eigen::Quaternion<T> d;
    };

    using Posef = DualQuaternion<float>;
    using Posed = DualQuaternion<double>;
    using Poseld = DualQuaternion<long double>;

    using Pose = Posed;
}

#endif //ANDREIUTILS_DUALQUATERNION_HPP
