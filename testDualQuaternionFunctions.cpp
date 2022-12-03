//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/QuaternionLowPassFilter.hpp>
#include <AndreiUtils/utilsEigenGeometry.h>
#include <AndreiUtils/utilsVector.hpp>
#include <iomanip>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace std;

void testDualQuaternions() {
    cout << setprecision(16);
    cout << DualQuaternion<double>::zero << endl;
    cout << DualQuaternion<double>::one << endl;
    cout << DualQuaternion<double>::i << endl;
    cout << DualQuaternion<double>::j << endl;
    cout << DualQuaternion<double>::k << endl;
    cout << DualQuaternion<double>::e << endl;
    cout << DualQuaternion<double>::ei << endl;
    cout << DualQuaternion<double>::ej << endl;
    cout << DualQuaternion<double>::ek << endl;

    Vector3d t(1, 2, 3);
    Vector3d p(4, 2, 3);
    vector<double> angles;
    // angles = {M_PI_2, 0, M_PI};
    // angles = {0, 0, M_PI};
    angles = {M_PI_2, 0, 0};
    Quaterniond r = qFromEulerAngles<double>(angles, "zyx");
    DualQuaternion<double> q(r, t), qCopy;

    cout << "q = " << q << endl;
    cout << "Log of q = " << q.log() << endl;
    cout << "Exp of Log of q = " << q.log().exp() << endl;
    cout << "Exp of Log of q == q?: " << q.log().exp().equal(q) << endl;

    qCopy = q;
    cout << "qCopy == q: " << (qCopy == q) << endl;
    qCopy = qCopy.addTranslation({0, 0, 1});
    cout << "qCopy != q: " << (qCopy != q) << endl;

    cout << "qCopy = " << qCopy << endl;
    cout << "Log of qCopy = " << qCopy.log() << endl;
    cout << "Exp of Log of qCopy = " << qCopy.log().exp() << endl;
    cout << "Exp of Log of qCopy == qCopy?: " << qCopy.log().exp().equal(qCopy) << endl;

    auto qNorm = q.norm();
    auto qCopyNorm = qCopy.norm();
    cout << qNorm << endl;
    cout << qCopyNorm << endl;
    cout << qCopyNorm.coefficientsAsEigen() << endl;

    cout << "q = " << q << endl;
    cout << printVectorToString(angles) << endl;
    cout << q.getTranslation().transpose() << endl;
    cout << q.transform(p).transpose() << endl;
    cout << q.getTransformationMatrix() << endl;
    cout << eulerAnglesFromQ(q.getRotation(), "zyx").transpose() << endl;
    cout << endl;

    cout << "Inverse:" << endl;
    DualQuaternion<double> qInv = q.dualQuaternionInverse();
    cout << "qInv = " << qInv << endl;

    cout << "qInv = " << qInv << endl;
    cout << "Log of qInv = " << qInv.log() << endl;
    cout << "Exp of Log of qInv = " << qInv.log().exp() << endl;
    cout << "Exp of Log of qInv == qInv?: " << qInv.log().exp().equal(qInv) << endl;
    cout << endl;

    cout << "q * qInv = " << q * qInv << endl;
    cout << "qInv * q = " << qInv * q << endl;
    cout << endl;

    Eigen::Matrix<double, 8, 1> coeffs;
    coeffs.topRows(4) = q.getRotation().coeffs().cast<double>();
    coeffs.bottomRows(4) = q.getDual().coeffs().cast<double>();

    cout << q.coefficientNorm() << endl;
    cout << qInv.coefficientNorm() << endl;
    q.normalize();
    qInv.normalize();
    cout << q.coefficientNorm() << endl;
    cout << qInv.coefficientNorm() << endl;
    cout << "q - qInv = " << q - qInv << endl;
    cout << (qInv - q).coefficientNorm() << endl;
    cout << (q - qInv).coefficientNorm() << endl;
    cout << "q    = " << q << endl;
    cout << "qInv = " << qInv << endl;
    cout << average(vector<DualQuaternion<double>>({q, qInv})) << endl;

    cout << endl << endl;

    cout << q.getTranslation().transpose() << ", " << q << endl;
    p = Vector3d(2, -31, 1);
    auto q1 = q.addTranslation(p);
    cout << q1.getTranslation().transpose() << ", " << q1 << endl;
    cout << endl;

    cout << eulerAnglesFromQ(q.getRotation(), "zyx").transpose() << ", " << q.getTranslation().transpose() << ", " << q
         << endl;
    // angles = {M_PI / 4, M_PI / 3, -M_PI / 6};
    angles = {-M_PI_2, M_PI, 0};
    r = qFromEulerAngles<double>(angles, "zyx");
    auto q2 = q.addRotation(r);
    cout << eulerAnglesFromQ(q2.getRotation(), "zyx").transpose() << ", " << q2.getTranslation().transpose() << ", "
         << q2 << endl;
    cout << endl;

    cout << q.powScrew(0.5) << endl;
}

void testTheSameOperations() {
    Vector3d p{-0.473938, 0.11523, 1.18481};
    for (int i = 0; i < 10; i++) {
        Quaterniond rot = sampleOrientation();
        Posed q{rot, p};
        cout << q.getTranslation().transpose() << endl;
        cout << q << endl;
        cout << Posed(qIdentity<double>(), p) * Posed(rot, Vector3d::Zero()) << endl;

        cout << endl;
    }
}

void testLowPassFilterQuaternion() {
    QuaternionLowPassFilter<double> qFilter(0.3, 0.3);
    qFilter.resetFilterValue(qIdentity<double>());
    qFilter.filter({0, 0, 1, 0});
    cout << qFilter.getFilterValue() << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // testDualQuaternions();
    // testTheSameOperations();
    testLowPassFilterQuaternion();

    return 0;
}