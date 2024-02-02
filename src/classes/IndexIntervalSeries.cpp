//
// Created by Andrei on 16.06.23.
//

#include <AndreiUtils/classes/IndexIntervalSeries.h>
#include <AndreiUtils/utilsVector.hpp>

using namespace AndreiUtils;
using namespace std;

IndexIntervalSeries::IndexIntervalSeries() : IndexIntervalSeries(0) {}

IndexIntervalSeries::IndexIntervalSeries(int maxNumberOfConsecutiveMissingFramesForSameInterval) :
        maxNumberOfConsecutiveMissingFramesForSameInterval(maxNumberOfConsecutiveMissingFramesForSameInterval) {}

void IndexIntervalSeries::addIndex(int newIndex) {
    if (!this->series.empty() &&
        this->series.back().second + 1 + this->maxNumberOfConsecutiveMissingFramesForSameInterval >= newIndex) {
        this->series.back().second = newIndex;
    } else {
        this->series.emplace_back(newIndex, newIndex);
    }
}

void IndexIntervalSeries::removeTooShortIntervals(int minIntervalSize) {
    vectorRemoveAllValueMatches<pair<int, int>>(this->series, [&](pair<int, int> const &intervalData) -> bool {
        return intervalData.second - intervalData.first + 1 < minIntervalSize;
    });
}

string IndexIntervalSeries::toString(int getOnlyLatest) const {
    int seriesSize = (int) this->series.size();
    string result;
    if (getOnlyLatest >= 0) {
        int startIndex = fastMax(seriesSize, getOnlyLatest) - getOnlyLatest;
        result = printVectorToString<pair<int, int>>(spliceVector(this->series, startIndex, seriesSize - 1),
                                                     [](pair<int, int> const &p) {
                                                         return "[" + to_string(p.first) + ", " + to_string(p.second) +
                                                                "]";
                                                     });
    } else {
        result = printVectorToString<pair<int, int>>(this->series, [](pair<int, int> const &p) {
            return "[" + to_string(p.first) + ", " + to_string(p.second) + "]";
        });
    }
    return result;
}

size_t IndexIntervalSeries::getSize() const {
    return this->series.size();
}

std::vector<std::pair<int, int>> &IndexIntervalSeries::getSeries() {
    return this->series;
}

std::vector<std::pair<int, int>> const &IndexIntervalSeries::getSeries() const {
    return this->series;
}
