//
// Created by Andrei on 11.10.22.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>

namespace nlohmann {
    template<typename T, typename SFINAE>
    struct adl_serializer;
}

namespace AndreiUtils {
    class Grasp {
    public:
        friend nlohmann::adl_serializer<Grasp, void>;

        explicit Grasp(std::vector<AndreiUtils::Posed> const &graspPointPoses = {},
                       bool hasGraspAngleRange = false,
                       std::pair<double, double> const &graspAngleRange = {0, 0});

        virtual ~Grasp();

        [[nodiscard]] size_t getNrPoints() const;

        virtual void update();

        [[nodiscard]] std::vector<AndreiUtils::Posed> const &getCurrentGraspPointPoses();

        [[nodiscard]] std::vector<AndreiUtils::Posed> const &getGraspPointPoses() const;

        void setRelativeTo(AndreiUtils::Posed graspRelativeTo);

        void composeRelativeTo(AndreiUtils::Posed const &graspRelativeTo);

        [[nodiscard]] std::pair<double, double> const &getGraspAngleRange() const;

        [[nodiscard]] std::pair<double, double> &getGraspAngleRange();

        [[nodiscard]] bool hasAngleRange() const;

    protected:
        void updateGraspPointPosesFromRelativeTo();

        AndreiUtils::Posed relativeTo;
        std::vector<AndreiUtils::Posed> graspPointPoses, originalGraspPointPoses;
        std::pair<double, double> graspAngleRange;
        bool hasGraspAngleRange;
    };
}
