//
// Created by Andrei on 05.08.23.
//

#pragma once

#include <AndreiUtils/classes/trajectory/TrajectoryBase.h>
#include <Eigen/Dense>
#include <vector>

namespace AndreiUtils {
    template<int NrJoints>
    class JointTrajectory : public TrajectoryBase {
    public:
        using JointValues = Eigen::Matrix<double, NrJoints, 1>;

        explicit JointTrajectory(int size = 0) : TrajectoryBase(size), jointValues(),
                                                 jointValuesData(std::make_shared<std::vector<JointValues>>()) {
            this->jointValues = this->jointValuesData.get();
            if (this->size != 0) {
                this->jointValues->resize(size);
            }
        }

        JointTrajectory(std::vector<JointValues> const &jointValues, std::vector<double> const &times) :
                TrajectoryBase(times), jointValuesData(std::make_shared<std::vector<JointValues>>(jointValues)) {
            this->jointValues = this->jointValuesData.get();
        }

        JointTrajectory(std::vector<JointValues> &&jointValues, std::vector<double> &&times) :
                TrajectoryBase(std::move(times)),
                jointValuesData(std::make_shared<std::vector<JointValues>>(std::move(jointValues))) {
            this->jointValues = this->jointValuesData.get();
        }

        JointTrajectory(std::vector<JointValues> *jointValues, std::vector<double> *times) :
                TrajectoryBase(times), jointValuesData(), jointValues(jointValues) {}

        JointTrajectory(JointTrajectory const &other) : TrajectoryBase(other), jointValuesData(), jointValues() {
            this->JointTrajectory::updatePointers(other);
        }

        JointTrajectory(JointTrajectory &&other) noexcept:
                TrajectoryBase(std::move(other)), jointValuesData(), jointValues() {
            this->JointTrajectory::updatePointers(std::move(other));
        }

        ~JointTrajectory() override = default;

        JointTrajectory &operator=(JointTrajectory const &other) {
            if (this != &other) {
                TrajectoryBase::operator=(other);
                this->JointTrajectory::updatePointers(other);
            }
            return *this;
        }

        JointTrajectory &operator=(JointTrajectory &&other) noexcept {
            if (this != &other) {
                TrajectoryBase::operator=(std::move(other));
                this->JointTrajectory::updatePointers(std::move(other));
            }
            return *this;
        }

        [[nodiscard]] virtual std::shared_ptr<TrajectoryBase> clone() const override {
            return std::make_shared<JointTrajectory<NrJoints>>(*this);
        }

        // just reserves data in the arrays; does NOT change the trajectory size (e.g. with empty elements)!
        void reserveNewSize(size_t newSize) override {
            TrajectoryBase::reserveNewSize(newSize);
            this->jointValues->reserve(newSize);
        }

        void addNewData(std::vector<JointValues> const &newJointValues, std::vector<double> const &newTimes) {
            TrajectoryBase::addNewData(newTimes);
            vectorAppendInPlace(*this->jointValues, newJointValues);
        }

        void addNewData(std::vector<JointValues> &&newJointValues, std::vector<double> &&newTimes) {
            TrajectoryBase::addNewData(std::move(newTimes));
            vectorMoveAppendInPlace(*this->jointValues, std::move(newJointValues));
        }

        void addNewDatum(JointValues const &newJointValues, double const &newTime) {
            TrajectoryBase::addNewDatum(newTime);
            this->jointValues->template emplace_back(newJointValues);
        }

        void addNewDatum(JointValues &&newJointValues, double &&newTime) {
            TrajectoryBase::addNewDatum(std::move(newTime));
            this->jointValues->template emplace_back(std::move(newJointValues));
        }

        [[nodiscard]] std::vector<JointValues> const &getJointValues() const {
            return *this->jointValues;
        }

        [[nodiscard]] std::vector<JointValues> &getJointValues() {
            return *this->jointValues;
        }

    protected:
        void updatePointers(JointTrajectory const &other) {
            this->jointValuesData = std::make_shared<std::vector<JointValues>>(*other.jointValues);
            this->jointValues = this->jointValuesData.get();
        }

        void updatePointers(JointTrajectory &&other) {
            this->jointValuesData = std::move(other.jointValuesData);
            this->jointValues = std::move(other.jointValues);
        }

        std::shared_ptr<std::vector<JointValues>> jointValuesData;
        std::vector<JointValues> *jointValues;
    };
}
