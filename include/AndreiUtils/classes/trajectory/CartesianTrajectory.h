//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/trajectory/TrajectoryBase.h>
#include <vector>

namespace AndreiUtils {
    class CartesianTrajectory : public TrajectoryBase {
    public:
        explicit CartesianTrajectory(int size = 0);

        CartesianTrajectory(std::vector<Posed> const &poses, std::vector<double> const &times);

        CartesianTrajectory(std::vector<Posed> &&poses, std::vector<double> &&times);

        CartesianTrajectory(std::vector<Posed> *poses, std::vector<double> *times);

        CartesianTrajectory(CartesianTrajectory const &other);

        CartesianTrajectory(CartesianTrajectory &&other) noexcept;

        ~CartesianTrajectory() override;

        CartesianTrajectory &operator=(CartesianTrajectory const &other);

        CartesianTrajectory &operator=(CartesianTrajectory &&other) noexcept;

        [[nodiscard]] std::shared_ptr<TrajectoryBase> clone() const override;

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        void reserveNewSize(size_t newSize) override;

        void addNewData(std::vector<Posed> const &newPoses, std::vector<double> const &newTimes);

        void addNewData(std::vector<Posed> &&newPoses, std::vector<double> &&newTimes);

        void addNewDatum(Posed const &newPose, double const &newTime);

        void addNewDatum(Posed &&newPose, double &&newTime);

        [[nodiscard]] std::vector<Posed> const &getPoses() const;

        [[nodiscard]] std::vector<Posed> &getPoses();

    protected:
        void updatePointers(CartesianTrajectory const &other);

        void updatePointers(CartesianTrajectory &&other);

        std::shared_ptr<std::vector<Posed>> posesData;
        std::vector<Posed> *poses;
    };
}
