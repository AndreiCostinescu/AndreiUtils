//
// Created by Andrei on 09.02.22.
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
            return DualQuaternion::one;
        }

        static DualQuaternion createFromCoefficients(std::vector<T> const &coefficients) {
            DualQuaternion<T> q;
            q.fromCoefficients(coefficients);
            return q;
        }

        explicit DualQuaternion(T const &q0 = T(0), T const &q1 = T(0), T const &q2 = T(0), T const &q3 = T(0),
                                T const &q4 = T(0), T const &q5 = T(0), T const &q6 = T(0), T const &q7 = T(0)) :
                r(q0, q1, q2, q3), d(q4, q5, q6, q7) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Quaternion<T> d) : r(r), d(d) {}

        DualQuaternion(Eigen::Quaternion<T> r, Eigen::Matrix<T, 3, 1> t) : r(r) {
            this->d = qMulScalar(vToQ(t) * r, T(0.5));
        }

        DualQuaternion(Eigen::Matrix<T, 3, 3> r, Eigen::Matrix<T, 3, 1> t) :
                DualQuaternion(qFromRotationMatrix(r), t) {}

        explicit DualQuaternion(Eigen::Matrix<T, 4, 4> t) :
                DualQuaternion(qFromRotationMatrix(Eigen::Matrix<T, 3, 3>(t.template block<3, 3>(0, 0))),
                               t.template block<3, 1>(0, 3)) {}

        virtual ~DualQuaternion() = default;

        std::vector<T> coefficients() const {
            return {this->r.w(), this->r.x(), this->r.y(), this->r.z(), this->d.w(), this->d.x(), this->d.y(),
                    this->d.z()};
        }

        Eigen::Matrix<T, 8, 1> coefficientsAsEigen() const {
            Eigen::Matrix<T, 8, 1> res;
            res << this->r.w(), this->r.x(), this->r.y(), this->r.z(), this->d.w(), this->d.x(), this->d.y(),
                    this->d.z();
            return res;
        }

        void fromCoefficients(std::vector<T> const &coefficients) {
            if (coefficients.size() != 8) {
                throw std::runtime_error(
                        "Coefficients' size is not 8 (is " + std::to_string(coefficients.size()) + ")!");
            }
            this->r.w() = coefficients[0];
            this->r.x() = coefficients[1];
            this->r.y() = coefficients[2];
            this->r.z() = coefficients[3];
            this->d.w() = coefficients[4];
            this->d.x() = coefficients[5];
            this->d.y() = coefficients[6];
            this->d.z() = coefficients[7];
        }

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

        template<class CastType>
        DualQuaternion<CastType> cast() const {
            return {this->r.template cast<CastType>(), this->d.template cast<CastType>()};
        }

        double rotationAngle() const {
            return 2 * acos(this->r.w());
        }

        // inspired by Riddhiman's function
        DualQuaternion powScrew(double const &a) const {
            double theta = this->rotationAngle(), n;  // Theta in radians (ALWAYS)
            DualQuaternion res;

            if (theta == 0) {
                n = theta;
            } else {
                n = 0.5 * theta;
            }

            double cos_n = cos(n);
            double cos_a_n = cos(a * n);
            double sin_n = sin(n);
            double sin_a_n = sin(a * n);

            double tmp = -(2 * this->d.w() / sin_n);
            double tmp_cos_2 = 0.5 * tmp * cos_n;

            Eigen::Vector3d u, v;
            u.x() = this->r.x();
            u.y() = this->r.y();
            u.z() = this->r.z();
            u /= sin_n;
            v.x() = this->d.x() - tmp_cos_2 * u.x();
            v.y() = this->d.y() - tmp_cos_2 * u.y();
            v.z() = this->d.z() - tmp_cos_2 * u.z();
            v /= sin_n;

            res.r.w() = cos_a_n;
            res.r.x() = u.x() * sin_a_n;
            res.r.y() = u.y() * sin_a_n;
            res.r.z() = u.z() * sin_a_n;

            double tmp_a_2 = 0.5 * tmp * a;

            res.d.w() = -tmp_a_2 * sin_a_n;
            res.d.x() = (v.x() * sin_a_n) + (tmp_a_2 * u.x() * cos_a_n);
            res.d.y() = (v.y() * sin_a_n) + (tmp_a_2 * u.y() * cos_a_n);
            res.d.z() = (v.z() * sin_a_n) + (tmp_a_2 * u.z() * cos_a_n);

            return res;
        }

        // inspired by DQ::log function
        DualQuaternion log() const {
            // Verify if the object caller is a unit DQ
            if (this->norm() != DualQuaternion::identity()) {
                throw (std::range_error("Bad log() call: Not a unit dual quaternion"));
            }

            // log calculation: https://www.researchgate.net/publication/362477656_Dynamic_Modeling_of_Robotic_Systems_A_Dual_Quaternion_Formulation
            Eigen::Quaterniond _r = (0.5 * this->rotationAngle()) * this->rotation_axis();  // primary
            Eigen::Quaterniond _d = 0.5 * this->translation();  // dual
            return {_r, _d};
        }

        // inspired by DQ::exp function
        DualQuaternion exp() const {
            double phi;
            DualQuaternion prim;
            DualQuaternion exp;

            if (this->r != qZero<double>()) {
                throw (std::range_error(
                        "Bad exp() call: Exponential operation is defined only for pure dual quaternions."));
            }

            auto pFct = this->P();
            auto dFct = this->D();

            prim = pFct;
            phi = prim.q.norm();

            if (phi != 0.0) {
                prim = cos(phi) + (sin(phi) / phi) * pFct;
            } else {
                prim = DualQuaternion::identity();
            }

            exp = (prim + DualQuaternion::e * dFct * prim);

            return exp;
        }

        // inspired by DQ::pow function
        DualQuaternion powGeom(double const &a) const {
            DualQuaternion res = a * this->log();
            return res.exp();
        }

        // inspired by DQ::norm function
        DualQuaternion norm() const {
            /*
            return DualQuaternion(this->r * this->r.conjugate(),
                                  qAdd(this->r * this->d.conjugate(), this->d * this->r.conjugate()));
            //*/

            DualQuaternion norm = this->conjugate() * (*this);
            norm.r.w() = sqrt(norm.r.w());
            norm.d.w() /= (2 * norm.r.w());  // why???
            return norm;
        }

        void normalize() {
            *this = (*this) * (this->norm().dualQuaternionInverse());
            /*
            T norm = this->r.norm();
            if (equal(norm, T(0))) {
                return;
            }
            this->r.normalize();
            //*/
        }

        DualQuaternion normalized() const {
            DualQuaternion res = *this;
            res.normalize();
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

        bool operator==(DualQuaternion const &other) const {
            return qEqual(this->r, other.r) && qEqual(this->d, other.d);
        }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

        bool operator!=(DualQuaternion const &other) const {
            return !(*this == other);
        }

#pragma clang diagnostic pop

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
            return ((*this) * DualQuaternion(qIdentity<T>(), vToQ(p)) *
                    this->quaternionDualConjugate()).getDual().vec();
        }

        Eigen::Matrix<T, 3, 1> rotate(const Eigen::Matrix<T, 3, 1> &v) const {
            return ((*this) * DualQuaternion(vToQ(v), qZero<T>()) *
                    this->quaternionDualConjugate()).getRotation().vec();
        }

        Eigen::Matrix<T, 3, 1> translate(const Eigen::Matrix<T, 3, 1> &t) const {
            return ((*this) * DualQuaternion(qZero<T>(), vToQ(t)) * this->quaternionDualConjugate()).getTranslation();
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

        static DualQuaternion<T> const zero;
        static DualQuaternion<T> const one;
        static DualQuaternion<T> const i;
        static DualQuaternion<T> const j;
        static DualQuaternion<T> const k;
        static DualQuaternion<T> const e;
        static DualQuaternion<T> const ei;
        static DualQuaternion<T> const ej;
        static DualQuaternion<T> const ek;

    protected:
        Eigen::Quaternion<T> r;
        Eigen::Quaternion<T> d;
    };

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::zero;

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::one = DualQuaternion<T>(T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::i = DualQuaternion<T>(T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::j = DualQuaternion<T>(T(0), T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::k = DualQuaternion<T>(T(0), T(0), T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::e = DualQuaternion<T>(T(0), T(0), T(0), T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::ei = DualQuaternion<T>(T(0), T(0), T(0), T(0), T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::ej = DualQuaternion<T>(T(0), T(0), T(0), T(0), T(0), T(0), T(1));

    template<class T>
    DualQuaternion<T> const DualQuaternion<T>::ek = DualQuaternion<T>(T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(1));

    using Posef = DualQuaternion<float>;
    using Posed = DualQuaternion<double>;
    using Poseld = DualQuaternion<long double>;

    using Pose = Posed;
}

#endif //ANDREIUTILS_DUALQUATERNION_HPP
