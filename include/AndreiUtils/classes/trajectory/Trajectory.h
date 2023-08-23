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

        Trajectory(std::vector<Posed> *poses, std::vector<double> *times);

        Trajectory(Trajectory const &other);

        Trajectory(Trajectory &&other) noexcept;

        virtual ~Trajectory();

        Trajectory &operator=(Trajectory const &other);

        Trajectory &operator=(Trajectory &&other) noexcept;

        [[nodiscard]] virtual std::shared_ptr<Trajectory> clone() const;

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        void reserveNewSize(size_t newSize);

        void addNewData(std::vector<Posed> const &newPoses, std::vector<double> const &newTimes);

        void addNewData(std::vector<Posed> &&newPoses, std::vector<double> &&newTimes);

        void addNewDatum(Posed const &newPose, double const &newTime);

        void addNewDatum(Posed &&newPose, double &&newTime);

        [[nodiscard]] std::vector<Posed> const &getPoses() const;

        [[nodiscard]] std::vector<Posed> &getPoses();

        [[nodiscard]] std::vector<double> const &getTimes() const;

        [[nodiscard]] std::vector<double> &getTimes();

        [[nodiscard]] size_t const &getSize() const;

    protected:
        void updatePointers(Trajectory const &other);

        void updatePointers(Trajectory &&other);

        std::shared_ptr<std::vector<Posed>> posesData;
        std::shared_ptr<std::vector<double>> timesData;

        std::vector<Posed> *poses;
        std::vector<double> *times;
        size_t size;  // actual size of the trajectory; may be less than poses.size()
    };
}
