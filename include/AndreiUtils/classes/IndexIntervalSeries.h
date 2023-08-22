//
// Created by Andrei on 16.06.23.
//

#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace AndreiUtils {
    class IndexIntervalSeries {
    public:
        IndexIntervalSeries();

        explicit IndexIntervalSeries(int maxNumberOfConsecutiveMissingFramesForSameInterval);

        virtual void addIndex(int newIndex);

        virtual void removeTooShortIntervals(int minIntervalSize);

        [[nodiscard]] std::string toString(int getOnlyLatest = -1) const;

        [[nodiscard]] size_t getSize() const;

        [[nodiscard]] std::vector<std::pair<int, int>> &getSeries();

        [[nodiscard]] std::vector<std::pair<int, int>> const &getSeries() const;

    protected:
        int maxNumberOfConsecutiveMissingFramesForSameInterval;
        std::vector<std::pair<int, int>> series;
    };
}
