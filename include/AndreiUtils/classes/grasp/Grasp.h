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

        explicit Grasp(std::vector<AndreiUtils::Posed> const &graspPointPoses = {});

        virtual ~Grasp();

        [[nodiscard]] size_t getNrPoints() const;

        [[nodiscard]] std::vector<AndreiUtils::Posed> const &getGraspPointPoses() const;

        void setRelativeTo(AndreiUtils::Posed graspRelativeTo);

        void composeRelativeTo(AndreiUtils::Posed const &graspRelativeTo);

    protected:
        void updateGraspPointPosesFromRelativeTo();

        AndreiUtils::Posed relativeTo;
        std::vector<AndreiUtils::Posed> graspPointPoses, originalGraspPointPoses;
    };
}
