//
// Created by Andrei on 21.10.22.
//

#ifndef ANDREIUTILS_UTILSDUALQUATERNIONS_HPP
#define ANDREIUTILS_UTILSDUALQUATERNIONS_HPP

#include <AndreiUtils/classes/PoseInterpolator.hpp>
#include <AndreiUtils/utilsQuaternions.hpp>
#include <AndreiUtils/utilsVector.hpp>

namespace AndreiUtils {
    template<typename T>
    std::vector<DualQuaternion<T>> sclerp(DualQuaternion<T> const &p1, T stepSize, DualQuaternion<T> const &p2) {
        return PoseInterpolator<T>().compute(p1, stepSize, p2).getResult();
    }

    template<typename T>
    std::vector<DualQuaternion<T>> sclerp(DualQuaternion<T> const &p1, DualQuaternion<T> const &p2, int nrSteps) {
        return sclerp(p1, T(1.0) / nrSteps, p2);
    }

    template<typename T>
    inline DualQuaternion<T> averagePose(std::vector<DualQuaternion<T>> const &sequence) {
        DualQuaternion<T> averagePose = DualQuaternion<T>::identity();
        std::vector<Eigen::Matrix<T, 3, 1>> sequencePositions(sequence.size());
        std::vector<Eigen::Quaternion<T>> sequenceQuaternions(sequence.size());
        for (int i = 0; i < sequence.size(); ++i) {
            sequencePositions[i] = sequence[i].getTranslation();
            sequenceQuaternions[i] = sequence[i].getRotation();
        }
        averagePose.translate(average(sequencePositions));
        Eigen::Quaterniond averageRotation;
        qSetZero(averageRotation);
        // take the log of each quaternion, sum the logs up, divide by the number of quaternions, exponentiate it
        for (auto const &q: sequenceQuaternions) {
            qIncrement(averageRotation, qLog(q));
        }
        averagePose.addRotation(qExp(qDivScalar(averageRotation, (double) sequenceQuaternions.size())));
        return averagePose;
    }
}

#endif //ANDREIUTILS_UTILSDUALQUATERNIONS_HPP
