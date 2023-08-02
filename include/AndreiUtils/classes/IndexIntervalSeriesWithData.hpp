//
// Created by Andrei on 02.08.23.
//

#pragma once

#include <AndreiUtils/classes/IndexIntervalSeries.h>
#include <AndreiUtils/utilsMap.hpp>

namespace AndreiUtils {
    template<typename DataType>
    class IndexIntervalSeriesWithData : public IndexIntervalSeries {
    public:
        IndexIntervalSeriesWithData() : IndexIntervalSeries(0) {}

        explicit IndexIntervalSeriesWithData(int consecutiveMissingFramesForSameIntervalThreshold) :
                IndexIntervalSeries(consecutiveMissingFramesForSameIntervalThreshold) {}

        void addIndex(int newIndex) override {
            this->addIndexAndMoveData(newIndex, std::move(DataType()));
        }

        void addIndexAndData(int newIndex, DataType const &data) {
            int prevSeriesSize = this->series.size(), prevIndex;
            if (prevSeriesSize != 0) {
                prevIndex = this->series.back().second;
            }
            IndexIntervalSeries::addIndex(newIndex);
            int seriesIndex = this->series.size() - 1;
            std::vector<DataType> *sequenceData;
            if (!mapGetIfContains(this->seriesData, seriesIndex, sequenceData)) {
                sequenceData = &(mapEmplace(this->seriesData, seriesIndex, std::vector<DataType>{})->second);
            } else if (prevSeriesSize != 0) {
                // in the case no new series was created, how much new data to add?
                for (int i = 0; i < newIndex - prevIndex - 1; ++i) {
                    sequenceData->template emplace_back(data);
                }
            }
            sequenceData->template emplace_back(data);
        }

        void addIndexAndMoveData(int newIndex, DataType &&data) {
            int prevSeriesSize = this->series.size(), prevIndex;
            if (prevSeriesSize != 0) {
                prevIndex = this->series.back().second;
            }
            IndexIntervalSeries::addIndex(newIndex);
            int seriesIndex = this->series.size() - 1;
            std::vector<DataType> *sequenceData;
            if (!mapGetIfContains(this->seriesData, seriesIndex, sequenceData)) {
                sequenceData = &(mapEmplace(this->seriesData, seriesIndex, std::vector<DataType>{})->second);
            } else if (prevSeriesSize != 0) {
                // in the case no new series was created, how much new data to add?
                for (int i = 0; i < newIndex - prevIndex - 1; ++i) {
                    sequenceData->template emplace_back(data);
                }
            }
            sequenceData->template emplace_back(std::move(data));
        }

        [[nodiscard]] std::map<int, std::vector<DataType>> const &getSeriesData() const {
            return this->seriesData;
        }

    protected:
        std::map<int, std::vector<DataType>> seriesData;
    };
}
