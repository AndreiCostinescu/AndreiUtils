//
// Created by Andrei on 13.10.22.
//

#ifndef ANDREIUTILS_SYMMETRY_H
#define ANDREIUTILS_SYMMETRY_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <tuple>
#include <string>

namespace AndreiUtils {
    class SymmetryJsonSerializer;

    class Symmetry {
    public:
        friend class SymmetryJsonSerializer;

        std::string type;
        Eigen::Vector3d axis, axisDisplacementFromOrigin;
        std::pair<double, double> range;

        Symmetry();

        AndreiUtils::Posed getSymmetricTransformation(double rangeValue) const;

        AndreiUtils::Posed getSymmetricTransformation() const;

        std::vector<AndreiUtils::Posed> createSymmetricPoses(std::vector<AndreiUtils::Posed> const &poses,
                                                             double const &rangeValue) const;

        std::vector<AndreiUtils::Posed> createSymmetricPoses(std::vector<AndreiUtils::Posed> const &poses) const;

        AndreiUtils::Posed createSymmetricPose(AndreiUtils::Posed const &pose, double const &rangeValue) const;

        AndreiUtils::Posed createSymmetricPose(AndreiUtils::Posed const &pose) const;
    };
}

#endif //ANDREIUTILS_SYMMETRY_H
