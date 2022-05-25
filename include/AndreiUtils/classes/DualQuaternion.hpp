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
        DualQuaternion() : r(1, 0, 0, 0), d(0, 0, 0, 0) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Quaternion<T> d) : r(r.normalized()), d(d) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Matrix<T, 3, 1> t) : r(r.normalized()) {
            this->d = qMulScalar(vToQ(t) * r, 0.5);
        }

        virtual ~DualQuaternion() = default;

        void normalize() {
            T norm = this->r.norm();
            if (equal(norm, T(0))) {
                return;
            }
            this->r = qDivScalar(this->r, norm);
            this->d = qDivScalar(this->d, norm);
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

        DualQuaternion operator*(const T &s) {
            return DualQuaternion(qMulScalar(this->r, s), qMulScalar(this->d, s));
        }

        // q_T = a * b corresponds to T_a_b = T_a_i * T_i_b; *this * other
        DualQuaternion operator*(const DualQuaternion &other) {
            /*
            Eigen::Quaternion<T> real = this->r * other.r;
            Eigen::Quaternion<T> dual((this->d * other.r).coeffs() + (this->r * other.d).coeffs());
            return DualQuaternion(real, dual);
            /*/
            return DualQuaternion(this->r * other.r, qAdd(this->d * other.r, this->r * other.d));
            //*/
        }

        DualQuaternion operator+(const DualQuaternion &other) {
            return DualQuaternion(qAdd(this->r, other->r), qAdd(this->d, other.d));
        }

        DualQuaternion operator-() {
            return DualQuaternion(qNeg(this->r), qNeg(this->d));
        }

        DualQuaternion operator-(const DualQuaternion &other) {
            return qAdd((*this), qNeg(-other));
        }

        Eigen::Matrix<T, 3, 1> transform(const Eigen::Matrix<T, 3, 1> &p) {
            return ((*this) * DualQuaternion(Eigen::Quaternion<T>::Identity(), vToQ(p)) *
                    this->quaternionDualConjugate()).getDual().vec();
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

            // Extract rotational information
            M.template block<3, 3>(0, 0) = q.getRotationAsMatrix();

            // Extract translation information
            M.col(3).template topRows<3>() = q.getTranslation();
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
}

#endif //ANDREIUTILS_DUALQUATERNION_HPP
