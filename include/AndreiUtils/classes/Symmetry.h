//
// Created by Andrei on 13.10.22.
//

#ifndef ANDREIUTILS_SYMMETRY_H
#define ANDREIUTILS_SYMMETRY_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <tuple>
#include <string>

namespace AndreiUtils {
    class Symmetry {
    public:
        Symmetry();

        [[nodiscard]] bool operator==(Symmetry const &other) const;

        Posed getSymmetricTransformation(double rangeValue) const;

        Posed getSymmetricTransformation() const;

        std::vector<Posed> createSymmetricPoses(std::vector<Posed> const &poses, double const &rangeValue) const;

        std::vector<Posed> createSymmetricPoses(std::vector<Posed> const &poses) const;

        Posed createSymmetricPose(Posed const &pose, double const &rangeValue) const;

        Posed createSymmetricPose(Posed const &pose) const;

        bool checkIfOrientationFitsSymmetry(Eigen::Quaterniond const &q, double axisSimilarityThreshold) const;

        std::string type;
        Eigen::Vector3d axis, axisDisplacementFromOrigin;
        std::pair<double, double> range;
    protected:
        static double const pi_2;
    };
}

#endif //ANDREIUTILS_SYMMETRY_H
