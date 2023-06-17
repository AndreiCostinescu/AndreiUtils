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

        explicit IndexIntervalSeries(int consecutiveMissingFramesForSameIntervalThreshold);

        void addIndex(int newIndex);

        [[nodiscard]] std::string toString(int getOnlyLatest = -1) const;

        [[nodiscard]] size_t getSize() const;

        [[nodiscard]] std::vector<std::pair<int, int>> const &getSeries() const;

    protected:
        int consecutiveMissingFramesForSameIntervalThreshold;
        std::vector<std::pair<int, int>> series;
    };
}
