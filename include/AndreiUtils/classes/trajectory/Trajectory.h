//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <vector>

namespace AndreiUtils {
    class Trajectory {
    public:
        explicit Trajectory(int size = 0);

        Trajectory(std::vector<Posed> const &poses, std::vector<double> const &times);

        Trajectory(std::vector<Posed> &&poses, std::vector<double> &&times);

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        void reserveNewSize(size_t newSize);

        void addNewData(std::vector<Posed> const &newPoses, std::vector<double> const &newTimes);

        void addNewData(std::vector<Posed> &&newPoses, std::vector<double> &&newTimes);

        [[nodiscard]] std::vector<Posed> const &getPoses() const;

        [[nodiscard]] std::vector<Posed> &getPoses();

        [[nodiscard]] std::vector<double> const &getTimes() const;

        [[nodiscard]] std::vector<double> &getTimes();

        [[nodiscard]] size_t const &getSize() const;

        std::vector<Posed> poses;
        std::vector<double> times;
        size_t size;  // actual size of the trajectory; may be less than poses.size()
    };
}
