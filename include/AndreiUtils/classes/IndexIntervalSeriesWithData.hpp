// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 02.08.23.
//

#pragma once

#include <AndreiUtils/classes/IndexIntervalSeries.h>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsVector.hpp>

namespace AndreiUtils {
    template<typename DataType>
    class IndexIntervalSeriesWithData : public IndexIntervalSeries {
    public:
        IndexIntervalSeriesWithData() : IndexIntervalSeries(0), seriesData() {}

        explicit IndexIntervalSeriesWithData(int maxNumberOfConsecutiveMissingFramesForSameInterval) :
            IndexIntervalSeries(maxNumberOfConsecutiveMissingFramesForSameInterval), seriesData() {}

        void addIndex(int newIndex) override { this->addIndexAndMoveData(newIndex, std::move(DataType())); }

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

        void removeTooShortIntervals(int minIntervalSize) override {
            int shiftIndex = 0;
            vectorRemoveAllValueMatches<std::pair<int, int>>(
                    this->series, [&, this](std::pair<int, int> const &intervalData, int const &intervalIndex) -> bool {
                        bool removeCondition = intervalData.second - intervalData.first + 1 < minIntervalSize;
                        if (removeCondition) {
                            ++shiftIndex;
                        } else if (shiftIndex > 0) {
                            mapSet(this->seriesData, intervalIndex - shiftIndex,
                                   std::move(mapGet(this->seriesData, intervalIndex)));
                        }
                        if (shiftIndex > 0) {
                            mapDelete(this->seriesData, intervalIndex);
                        }
                        return removeCondition;
                    });
        }

        [[nodiscard]] std::map<int, std::vector<DataType>> &getSeriesData() { return this->seriesData; }

        [[nodiscard]] std::map<int, std::vector<DataType>> const &getSeriesData() const { return this->seriesData; }

    protected:
        std::map<int, std::vector<DataType>> seriesData;
    };
} // namespace AndreiUtils
