/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        averagePose.addRotation(qAverage(sequenceQuaternions));
        return averagePose;
    }
} // namespace AndreiUtils

#endif // ANDREIUTILS_UTILSDUALQUATERNIONS_HPP
