//
// Created by Andrei on 20.11.23.
//

#pragma once

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/Interval.hpp>
#include <AndreiUtils/utilsGeometry.h>
#include <AndreiUtils/utilsMap.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace AndreiUtils {
    template<class T>
    class PoseParameterFunction {
    public:
        virtual ~PoseParameterFunction() = default;

        [[nodiscard]] virtual AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) const = 0;

        [[nodiscard]] virtual int getNrParameters() const = 0;

        [[nodiscard]] virtual std::vector<Interval<T>> getDefaultParameterRange() const = 0;

        [[nodiscard]] virtual std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const = 0;

    protected:
        static Eigen::Matrix<T, 3, 1> tZero;
        static Eigen::Quaternion<T> qIdentity;
    };

    template<class T>
    Eigen::Matrix<T, 3, 1> PoseParameterFunction<T>::tZero = Eigen::Matrix<T, 3, 1>::Zero();

    template<class T>
    Eigen::Quaternion<T> PoseParameterFunction<T>::qIdentity = AndreiUtils::qIdentity<T>();

    template<class T>
    class NoPoseVariation : PoseParameterFunction<T> {
    public:
        explicit NoPoseVariation(AndreiUtils::DualQuaternion<T> pose) : pose(std::move(pose)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.empty());
            return this->pose;
        }

        [[nodiscard]] int getNrParameters() const override {
            return 0;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (!parameterValues.empty()) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            return std::make_shared<NoPoseVariation<T>>(*this);
        }

    protected:
        AndreiUtils::DualQuaternion<T> pose;
    };

    template<class T>
    class VariableAngleInAxisAngle : PoseParameterFunction<T> {
    public:
        explicit VariableAngleInAxisAngle(Eigen::Matrix<T, 3, 1> axis) : axis(std::move(axis)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 1);
            return AndreiUtils::DualQuaternion<T>(Eigen::Quaternion<T>(
                    Eigen::AngleAxis<T>(parameterValues[0], this->axis)), this->tZero);
        }

        [[nodiscard]] int getNrParameters() const {
            return 1;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>(T(-M_PI), T(M_PI))};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 1) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableAngleInAxisAngle<T>>(*this);
            }
            T &angleValue = mapGet(parameterValues, 0);  // should throw an error if key is non-existing
            return std::make_shared<NoPoseVariation<T>>(this->get({angleValue}));
        }

    protected:
        Eigen::Matrix<T, 3, 1> axis;
    };

    template<class T>
    class VariableDegreeAngleInAxisAngle : PoseParameterFunction<T> {
    public:
        explicit VariableDegreeAngleInAxisAngle(Eigen::Matrix<T, 3, 1> axis) : axis(std::move(axis)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 1);
            return AndreiUtils::DualQuaternion<T>(Eigen::Quaternion<T>(
                    Eigen::AngleAxis<T>(AndreiUtils::deg2Rad(parameterValues[0]), this->axis)), this->tZero);
        }

        [[nodiscard]] int getNrParameters() const {
            return 1;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>(T(-180), T(180))};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 1) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableAngleInAxisAngle<T>>(*this);
            }
            T &angleValue = mapGet(parameterValues, 0);  // should throw an error if key is non-existing
            return std::make_shared<NoPoseVariation<T>>(this->get({angleValue}));
        }

    protected:
        Eigen::Matrix<T, 3, 1> axis;
    };

    template<class T>
    class VariableAxisInAxisAngle : PoseParameterFunction<T> {
    public:
        explicit VariableAxisInAxisAngle(T angle) : angle(std::move(angle)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 3);
            Eigen::Matrix<T, 3, 1> axis(parameterValues[0], parameterValues[1], parameterValues[2]);
            return AndreiUtils::DualQuaternion<T>(
                    Eigen::Quaternion<T>(Eigen::AngleAxis<T>(this->angle, axis.normalized())), this->tZero);
        }

        [[nodiscard]] int getNrParameters() const {
            return 3;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            auto axisRestriction = Interval<T>(-1, 1);
            return {axisRestriction, axisRestriction, axisRestriction};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (!(parameterValues.size() == 0 || parameterValues.size() == 3)) {
                throw std::runtime_error("Passed incorrect amount of parameters (" +
                                         std::to_string(parameterValues.size()) + ") to " +
                                         std::to_string(this->getNrParameters()) + "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableAxisInAxisAngle<T>>(*this);
            }
            return std::make_shared<NoPoseVariation<T>>(
                    this->get({mapGet(parameterValues, 0), mapGet(parameterValues, 1), mapGet(parameterValues, 2)}));
        }

    protected:
        T angle;
    };

    template<class T>
    class VariableXAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableXAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 1);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(parameterValues[0], this->t(1), this->t(2)));
        }

        [[nodiscard]] int getNrParameters() const {
            return 1;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 1) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableXAxisTranslation<T>>(*this);
            }
            return std::make_shared<NoPoseVariation<T>>(this->get({mapGet(parameterValues, 0)}));
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableYAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableYAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 1);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(this->t(0), parameterValues[0], this->t(2)));
        }

        [[nodiscard]] int getNrParameters() const {
            return 1;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 1) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableYAxisTranslation<T>>(*this);
            }
            return std::make_shared<NoPoseVariation<T>>(this->get({mapGet(parameterValues, 0)}));
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableZAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableZAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 1);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(this->t(0), this->t(1), parameterValues[0]));
        }

        [[nodiscard]] int getNrParameters() const {
            return 1;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 1) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableZAxisTranslation<T>>(*this);
            }
            return std::make_shared<NoPoseVariation<T>>(this->get({mapGet(parameterValues, 0)}));
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableXYAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableXYAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 2);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(parameterValues[0], parameterValues[1], this->t(2)));
        }

        [[nodiscard]] int getNrParameters() const {
            return 2;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange(), Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 2) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableXYAxisTranslation<T>>(*this);
            }
            double value1, value2;
            bool hasSecondValue = mapGet(parameterValues, 1, value2);
            if (mapGetIfContains(parameterValues, 0, value1)) {
                if (hasSecondValue) {
                    return std::make_shared<NoPoseVariation<T>>(this->get({value1, value2}));
                } else {
                    Eigen::Matrix<T, 3, 1> newAxis = this->t;
                    newAxis.x() = value1;
                    return std::make_shared<VariableYAxisTranslation<T>>(newAxis);
                }
            }
            Eigen::Matrix<T, 3, 1> newAxis = this->t;
            newAxis.y() = value2;
            return std::make_shared<VariableXAxisTranslation<T>>(newAxis);
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableYZAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableYZAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 2);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(this->t(0), parameterValues[0], parameterValues[1]));
        }

        [[nodiscard]] int getNrParameters() const {
            return 2;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange(), Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 2) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableYZAxisTranslation<T>>(*this);
            }
            double value1, value2;
            bool hasSecondValue = mapGet(parameterValues, 1, value2);
            if (mapGetIfContains(parameterValues, 0, value1)) {
                if (hasSecondValue) {
                    return std::make_shared<NoPoseVariation<T>>(this->get({value1, value2}));
                } else {
                    Eigen::Matrix<T, 3, 1> newAxis = this->t;
                    newAxis.y() = value1;
                    return std::make_shared<VariableZAxisTranslation<T>>(newAxis);
                }
            }
            Eigen::Matrix<T, 3, 1> newAxis = this->t;
            newAxis.z() = value2;
            return std::make_shared<VariableYAxisTranslation<T>>(newAxis);
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableXZAxisTranslation : PoseParameterFunction<T> {
    public:
        explicit VariableXZAxisTranslation(Eigen::Matrix<T, 3, 1> t) : t(std::move(t)) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 2);
            return AndreiUtils::DualQuaternion<T>(
                    this->qIdentity, Eigen::Matrix<T, 3, 1>(parameterValues[0], this->t(1), parameterValues[1]));
        }

        [[nodiscard]] int getNrParameters() const {
            return 2;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange(), Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 2) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableXZAxisTranslation<T>>(*this);
            }
            double value1, value2;
            bool hasSecondValue = mapGet(parameterValues, 1, value2);
            if (mapGetIfContains(parameterValues, 0, value1)) {
                if (hasSecondValue) {
                    return std::make_shared<NoPoseVariation<T>>(this->get({value1, value2}));
                } else {
                    Eigen::Matrix<T, 3, 1> newAxis = this->t;
                    newAxis.x() = value1;
                    return std::make_shared<VariableZAxisTranslation<T>>(newAxis);
                }
            }
            Eigen::Matrix<T, 3, 1> newAxis = this->t;
            newAxis.z() = value2;
            return std::make_shared<VariableXAxisTranslation<T>>(newAxis);
        }

    protected:
        Eigen::Matrix<T, 3, 1> t;
    };

    template<class T>
    class VariableTranslation : PoseParameterFunction<T> {
    public:
        VariableTranslation() = default;

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::vector<T> const &parameterValues) override {
            assert(parameterValues.size() == 3);
            return AndreiUtils::DualQuaternion<T>(this->qIdentity,
                                                  Eigen::Matrix<T, 3, 1>(parameterValues[0], parameterValues[1],
                                                                         parameterValues[2]));
        }

        [[nodiscard]] int getNrParameters() const {
            return 3;
        }

        [[nodiscard]] std::vector<Interval<T>> getDefaultParameterRange() const override {
            return {Interval<T>::createFullRange(), Interval<T>::createFullRange(), Interval<T>::createFullRange()};
        }

        [[nodiscard]] std::shared_ptr<PoseParameterFunction<T>> setParametersAndGetNewFunction(
                std::map<int, T> const &parameterValues) const override {
            if (parameterValues.size() > 2) {
                throw std::runtime_error("Passed too many arguments to " + std::to_string(this->getNrParameters()) +
                                         "-argument pose-function!");
            }
            if (parameterValues.empty()) {
                return std::make_shared<VariableTranslation<T>>(*this);
            }
            double value1, value2, value3;
            bool hasSecondValue = mapGet(parameterValues, 1, value2);
            bool hasThirdValue = mapGet(parameterValues, 2, value3);
            Eigen::Matrix<T, 3, 1> axis;
            if (mapGetIfContains(parameterValues, 0, value1)) {
                axis.x() = value1;
                if (hasSecondValue) {
                    axis.y() = value2;
                    if (hasThirdValue) {
                        return std::make_shared<NoPoseVariation<T>>(this->get({value1, value2, value3}));
                    } else {
                        return std::make_shared<VariableZAxisTranslation<T>>(axis);
                    }
                } else {
                    if (hasThirdValue) {
                        axis.z() = value3;
                        return std::make_shared<VariableYAxisTranslation<T>>(axis);
                    } else {
                        return std::make_shared<VariableYZAxisTranslation<T>>(axis);
                    }
                }
            }
            if (hasSecondValue) {
                axis.y() = value2;
                if (hasThirdValue) {
                    axis.z() = value3;
                    return std::make_shared<VariableXAxisTranslation<T>>(axis);
                } else {
                    return std::make_shared<VariableXZAxisTranslation<T>>(axis);
                }
            }
            axis.z() = value3;
            return std::make_shared<VariableXYAxisTranslation<T>>(axis);
        }

    protected:
    };

    template<class T>
    class ParametrizablePose {
    public:
        struct ParametrizablePoseParameter {
            ParametrizablePoseParameter(std::string name, int fIndex, int pIndex, AndreiUtils::Interval<T> range) :
                    parameterName(std::move(name)), functionIndex(fIndex), parameterIndex(pIndex),
                    valueRange(std::move(range)), sampler(valueRange.createSampler()) {}

            int functionIndex;
            int parameterIndex;
            std::string parameterName;
            AndreiUtils::Interval<T> valueRange;
            AndreiUtils::RandomNumberGenerator<T> sampler;
        };

        ParametrizablePose() = default;

        void addComposition(std::shared_ptr<PoseParameterFunction<T>> poseFunction) {
            std::vector<std::string> parameterNames;
            int functionIndex = this->poseComposition.size();
            for (int i = 0; i < poseFunction->getNrParameters(); ++i) {
                parameterNames.emplace_back("parameter_" + std::to_string(functionIndex) + "_" + std::to_string(i));
            }
            this->addComposition(std::move(poseFunction), parameterNames, poseFunction->getDefaultParameterRange());
        }

        void addComposition(std::shared_ptr<PoseParameterFunction<T>> poseFunction,
                            std::vector<std::string> const &poseFunctionParameters) {
            this->addComposition(std::move(poseFunction), poseFunctionParameters,
                                 poseFunction->getDefaultParameterRange());
        }

        void addComposition(std::shared_ptr<PoseParameterFunction<T>> poseFunction,
                            std::vector<AndreiUtils::Interval<T>> const &poseFunctionParameterValues) {
            std::vector<std::string> parameterNames;
            int functionIndex = this->poseComposition.size();
            for (int i = 0; i < poseFunction->getNrParameters(); ++i) {
                parameterNames.emplace_back("parameter_" + std::to_string(functionIndex) + "_" + std::to_string(i));
            }
            this->addComposition(std::move(poseFunction), parameterNames, poseFunctionParameterValues);
        }

        void addComposition(std::shared_ptr<PoseParameterFunction<T>> poseFunction,
                            std::vector<std::string> const &poseFunctionParameters,
                            std::vector<Interval<T>> const &poseFunctionParameterValues) {
            size_t functionIndex = this->poseComposition.size();
            this->poseComposition.emplace_back(std::move(poseFunction));
            for (int i = 0; i < poseFunctionParameters.size(); ++i) {
                std::string const &parameterName = poseFunctionParameters[i];
                if (AndreiUtils::mapContains(this->parameterNameAssignment, parameterName)) {
                    throw std::runtime_error("Duplicate parameter name: " + parameterName);
                }
                AndreiUtils::mapEmplace(this->parameterAssignment[functionIndex], i, parameterName, functionIndex, i,
                                        poseFunctionParameterValues[i]);
                AndreiUtils::mapEmplace(this->parameterNameAssignment, parameterName, functionIndex, i);
            }
        }

        [[nodiscard]] ParametrizablePose setParametersAndGetNewParametrizablePose(
                std::map<int, std::map<int, T>> const &parameterValues) const {
            ParametrizablePose res;
            for (int functionIndex = 0; functionIndex < this->poseComposition.size(); ++functionIndex) {
                std::vector<std::string> newParameterNames;
                std::vector<AndreiUtils::Interval<T>> newParameterRanges;
                std::shared_ptr<PoseParameterFunction<T>> newPoseFunction;
                std::map<int, T> const *toSetParametersForCurrentFunction;
                std::map<int, ParametrizablePoseParameter> const *parametersOfCurrentFunction;
                if (mapGetIfContains(this->parameterAssignment, functionIndex, parametersOfCurrentFunction)) {
                    if (mapGetIfContains(parameterValues, functionIndex, toSetParametersForCurrentFunction)) {
                        newPoseFunction = this->poseComposition[functionIndex]->setParametersAndGetNewFunction(
                                *toSetParametersForCurrentFunction);
                        for (auto const &parameterData: *parametersOfCurrentFunction) {
                            if (!mapContains(*toSetParametersForCurrentFunction, parameterData.first)) {
                                newParameterNames.emplace_back(parameterData.second.parameterName);
                                newParameterRanges.emplace_back(parameterData.second.valueRange);
                            }
                        }
                    } else {
                        newPoseFunction = this->poseComposition[functionIndex]->setParametersAndGetNewFunction({});
                        for (auto const &parameterData: *parametersOfCurrentFunction) {
                            newParameterNames.emplace_back(parameterData.second.parameterName);
                            newParameterRanges.emplace_back(parameterData.second.valueRange);
                        }
                    }
                } else {
                    newPoseFunction = this->poseComposition[functionIndex]->setParametersAndGetNewFunction({});
                }
                res.addComposition(newPoseFunction, newParameterNames, newParameterRanges);
            }
            return res;
        }

        [[nodiscard]] AndreiUtils::DualQuaternion<T> sample() {
            AndreiUtils::DualQuaternion<T> res = AndreiUtils::DualQuaternion<T>::one;
            for (int functionIndex = 0; functionIndex < this->poseComposition.size(); ++functionIndex) {
                std::vector<T> sampledParameters;
                std::map<int, ParametrizablePoseParameter> *functionParameters;
                if (AndreiUtils::mapGetIfContains(this->parameterAssignment, functionIndex, functionParameters)) {
                    for (auto &functionParameter: *functionParameters) {
                        sampledParameters.emplace_back(functionParameter.second.sampler.sample());
                    }
                }
                res *= this->poseComposition[functionIndex]->get(sampledParameters);
            }
            return res;
        }

        [[nodiscard]] AndreiUtils::DualQuaternion<T> get(std::map<int, std::vector<T>> const &parameters) {
            AndreiUtils::DualQuaternion<T> res = AndreiUtils::DualQuaternion<T>::one;
            for (int i = 0; i < this->poseComposition.size(); ++i) {
                std::vector<T> *poseCompositionParameters;
                if (AndreiUtils::mapContains(parameters, i, poseCompositionParameters)) {
                    res *= this->poseComposition[i]->get(*poseCompositionParameters);
                } else {
                    res *= this->poseComposition[i]->get({});
                }
            }
            return res;
        }

    protected:
        std::map<std::string, std::pair<int, int>> parameterNameAssignment;
        std::map<int, std::map<int, ParametrizablePoseParameter>> parameterAssignment;
        std::vector<std::shared_ptr<PoseParameterFunction<T>>> poseComposition;
    };

    using ParametrizablePosef = ParametrizablePose<float>;
    using ParametrizablePosed = ParametrizablePose<double>;
}
