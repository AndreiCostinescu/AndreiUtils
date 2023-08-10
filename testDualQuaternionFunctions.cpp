//
// Created by Andrei on 03.11.22.
//

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/QuaternionLowPassFilter.hpp>
#include <AndreiUtils/classes/PoseInterpolator.hpp>
#include <AndreiUtils/classes/PoseDecoupledInterpolator.hpp>
#include <AndreiUtils/utilsEigenGeometry.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsGeometry.h>
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

void testTransformationMatrixToDualQuaternion() {
    Eigen::Matrix4d m;
    m.setZero();
    m(0, 1) = 1;
    m(1, 2) = 1;
    m(2, 0) = -1;
    m(0, 3) = 10;
    m(1, 3) = 20;
    m(2, 3) = 30;
    m(3, 3) = 1;

    Matrix3d m1 = m.block<3, 3>(0, 0);
    Matrix3d m2;
    m2.setZero();
    m2(0, 2) = -1;
    m2(1, 0) = 1;
    m2(2, 1) = -1;

    cout << Eigen::Quaterniond(m1) << endl;
    cout << Eigen::Quaterniond(m2) << endl;

    auto q1 = Quaterniond(-0.5, -0.5, 0.5, 0.5);
    auto q2 = Quaterniond(0.5, -0.5, -0.5, 0.5);
    auto q3 = Quaterniond(-0.5, 0.5, 0.5, -0.5);
    auto q4 = Quaterniond(0.5, 0.5, 0.5, -0.5);
    cout << q1 << endl;
    cout << q1.toRotationMatrix() << endl;
    cout << q2 << endl;
    cout << q2.toRotationMatrix() << endl;
    cout << q3 << endl;
    cout << q3.toRotationMatrix() << endl;
    cout << q4 << endl;
    cout << q4.toRotationMatrix() << endl;

    cout << qFromRotationMatrix((Matrix3d) m2.transpose()) << endl;
}

void testRotationEquivalence() {
    Vector3d v(4, 2, 3);
    Posed q(sampleOrientation(), qZero<double>());
    cout << q.rotate(v).transpose() << endl;
    cout << (q.getRotationAsMatrix() * v).transpose() << endl;
}

void testNormalization() {
    Posed q(0.166411, -0.482188, 0.493274, -0.00571844, 4.46284e-60, 0.106648, -0.0645096, 6.95262e-310);
    cout << q << endl;
    cout << "Norm of q = " << q.norm() << endl;
    q.normalize();
    cout << "After normalize: " << q << endl;
    cout << "Norm of q = " << q.norm() << endl;
    q.normalize();
    cout << "After normalize 2: " << q << endl;
    cout << "Norm of q = " << q.norm() << endl;
}

void testTranslationDelta() {
    vector<double> tmpData = {-0.0009003758676818849, 0.04833426661985106, -0.9988211726304295, 0.0,
                              0.9997362675189353, 0.022541214719883684, 0.00018959817640538056, 0.0,
                              0.022524240269260587, -0.9985768060327067, -0.048342745617214196, 0.0,
                              0.3305606906144228, 0.12850723493527194, 0.2173939959466131, 1.0};
    Vector3d referenceT(0.3305606906144228, 0.12850723493527194, 0.2173939959466131);
    Matrix4d T;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            T(i, j) = tmpData[i + 4 * j];
        }
    }
    cout << T << endl;
    cout << qFromRotationMatrix((Matrix3d) T.block<3, 3>(0, 0)) << endl;
    Posed qRef(T);
    cout << qRef << "; " << qRef.getTranslation().transpose() << endl;
    cout << qRef.norm() << endl;
    cout << qRef.normalized() << "; " << qRef.normalized().getTranslation().transpose() << endl;

    Posed q(0.493275, 0.506182, 0.517635, -0.482187, -0.0645096, -0.00571899, 0.166411, 0.106648);
    cout << q << endl;
    cout << "Norm of q = " << q.norm() << endl;
    q.normalize();
    cout << q.getTranslation() << endl;
    Posed tmp(q.getRotation(), Vector3d{1, 1, 1});
    cout << tmp.getTranslation() << endl;
}

void testInterpolation() {
    Posed start(0.503886, 0.486674, 0.484643, -0.523792, -0.0852132, 0.00675892, 0.16508, 0.0770467);
    Posed goal(0.503886, 0.486674, 0.484643, -0.523792, -0.0243781, -0.0562269, 0.0996041, 0.0164655);
    PoseInterpolator<double> p;
    auto const &res = p.compute(start, goal, 10).getResult();
    for (auto const &pose: res) {
        cout << pose << " (t = " << pose.getTranslation().transpose() << ")" << endl;
    }
}

void testNormalizationInterpolation() {
    Posed dq1, dq2;
    Eigen::Quaterniond q1(0.01033028, 0.1294797, 0.0124287, 0.999145);
    Eigen::Quaterniond q2(0.996145, 0.0284797, 0.0214287, 0.00333028);
    cout << q1.norm() << endl;
    cout << q2.norm() << endl;
    PoseDecoupledInterpolator<double> i1;
    PoseInterpolator<double> i2;
    for (auto const &dq: i1.compute({q1, qZero<double>()}, {q2, qZero<double>()}, 1000).getResult()) {
        cout << dq.getRotation().norm() << ": " << dq.coefficientsAsEigen().transpose() << endl;
    }
    cout << endl << endl;
    for (auto const &dq: i2.compute({q1, qZero<double>()}, {q2, qZero<double>()}, 1000).getResult()) {
        cout << dq.getRotation().norm() << ": " << dq.coefficientsAsEigen().transpose() << endl;
    }
}

void testSameTranslationNegatedQuaternion() {
    Quaterniond q1(AngleAxisd(deg2Rad(180.), Vector3d(0, 1, -1).normalized()));
    Vector3d t1(-0.035, 0.017, 0.26);  // prev y = 0.025
    Posed p(q1, t1);
    cout << p.getTranslation().transpose() << endl;
    cout << (-p).getTranslation().transpose() << endl;
}

void playgroundGraspCupFromTop() {
    AngleAxisd tmpAA(deg2Rad(120.), Vector3d(1, 1, -1).normalized());
    AngleAxisd tmpAA2(deg2Rad(-90.), Vector3d(0, 0, 1).normalized());
    Quaterniond q1Prev(tmpAA);
    // tmpAA * tmpAA2 = q1
    Quaterniond tmp(AngleAxisd(deg2Rad(180.), Vector3d(0, 1, -1).normalized()));
    Quaterniond q1(tmp * qxRotation(-0.70)); // previously -0.65
    // Vector3d t1(-0.0225, 0.022, 0.26);  // prev y = 0.025
    Vector3d t1(-0.035, 0.017, 0.26);  // prev y = 0.025
    cout << "ApproachPose: " << printVectorToString(Posed(q1, t1).coefficients()) << endl;

    Quaterniond &q2 = q1;
    Vector3d t2(-0.035, 0.015, 0.17);  // previously x=-0.0225, y=0.028/0.024
    cout << "GraspPose: " << printVectorToString(Posed(q2, t2).coefficients()) << endl;

    Quaterniond q3 = q1 * qxRotation(0.08);
    Vector3d t3 = t2 - Vector3d{0, 0, 0.006};
    cout << "ReleasePoseRelativeToObjectGoal: " << printVectorToString(Posed(q3, t3).coefficients()) << endl;

    Quaterniond &q4 = q3;
    Vector3d t4 = t3 + Vector3d{0, 0, 0.1};
    cout << "PostReleasePoseRelativeToObjectGoal: " << printVectorToString(Posed(q4, t4).coefficients()) << endl;
    cout << endl;
}

void playgroundGraspCupFromSide() {
    AngleAxisd startRot(deg2Rad(120.), Vector3d(1, 1, -1).normalized());
    auto qX = qxRotation(1.32);
    auto qY = qyRotation(0.03);  // prev: -0.0375
    auto qZ = qzRotation(0.3);  // prev: -0.0025
    Quaterniond q1(startRot * qX * qY * qZ);
    Vector3d t1(0.03, 0.0475, 0.20);
    cout << "ApproachPose: " << printVectorToString(Posed(q1, t1).coefficients()) << endl;

    Quaterniond &q2 = q1;
    Vector3d t2(0.05, 0.0265, 0.060);  // prev: y = 0.0275, z = 0.052 // 0.062 works as well
    cout << "GraspPose: " << printVectorToString(Posed(q2, t2).coefficients()) << endl;

    Quaterniond &q3 = q1;
    Vector3d t3 = t2 - Vector3d{0, 0, 0.001};
    cout << "ReleasePoseRelativeToObjectGoal: " << printVectorToString(Posed(q3, t3).coefficients()) << endl;
}

void play() {
    auto q = Posed::createFromCoefficients(
            vector<double>{-0.43199914013303337,0.005861429276844252,-0.009443289551921301,-0.9018075871533447,-0.008872428920675416,0.05361516740646428,0.33881962340539384,0.0010507467228607378});
    cout << q.getTranslation().transpose() << endl;
    cout << q.getRotation() << endl;

    q = Posed::createFromCoefficients(
            vector<double>{0.52044, -0.493292, 0.468434, 0.516116, -0.00216926, -0.0154996, -0.0159906, 0.00188652});
    cout << q.getTranslation().transpose() << endl;

    Quaterniond r(-0.742227, 0.669617, 0.0221371, -0.0148873);
    Posed pose(r, q.getTranslation() - Vector3d(0, 0, -0.002));
    nlohmann::json j = pose;
    cout << j.dump(4) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // testDualQuaternions();
    // testTheSameOperations();
    // testLowPassFilterQuaternion();
    // testTransformationMatrixToDualQuaternion();
    // testRotationEquivalence();
    // testTranslationDelta();
    // testInterpolation();
    // testNormalizationInterpolation();
    // testSameTranslationNegatedQuaternion();
    // playgroundGraspCupFromTop();
    // playgroundGraspCupFromSide();
    play();

    return 0;
}