//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <vector>

namespace AndreiUtils {
    class TrajectoryBase {
    public:
        explicit TrajectoryBase(int size = 0);

        explicit TrajectoryBase(std::vector<double> const &times);

        explicit TrajectoryBase(std::vector<double> &&times);

        explicit TrajectoryBase(std::vector<double> *times);

        TrajectoryBase(TrajectoryBase const &other);

        TrajectoryBase(TrajectoryBase &&other) noexcept;

        virtual ~TrajectoryBase();

        TrajectoryBase &operator=(TrajectoryBase const &other);

        TrajectoryBase &operator=(TrajectoryBase &&other) noexcept;

        [[nodiscard]] virtual std::shared_ptr<TrajectoryBase> clone() const;

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        virtual void reserveNewSize(size_t newSize);

        [[nodiscard]] std::vector<double> const &getTimes() const;

        [[nodiscard]] std::vector<double> &getTimes();

        [[nodiscard]] size_t const &getSize() const;

    protected:
        void addNewData(std::vector<double> const &newTimes);

        void addNewData(std::vector<double> &&newTimes);

        void addNewDatum(double const &newTime);

        void addNewDatum(double &&newTime);

        void updatePointers(TrajectoryBase const &other);

        void updatePointers(TrajectoryBase &&other);

        std::shared_ptr<std::vector<double>> timesData;

        std::vector<double> *times;
        size_t size;  // actual size of the trajectory; may be less than poses.size()
    };
}
