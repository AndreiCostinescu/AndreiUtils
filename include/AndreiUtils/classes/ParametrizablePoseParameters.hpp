//
// Created by Andrei on 08.12.23.
//

#pragma once

#include <AndreiUtils/classes/ParametrizablePose.hpp>
#include <AndreiUtils/utilsJson.hpp>
#include <AndreiUtils/utilsString.h>
#include <cassert>

namespace AndreiUtils {
    template<typename T>
    class ExternalPoseInterface {
    public:
        virtual ~ExternalPoseInterface() = default;

        virtual AndreiUtils::DualQuaternion<T> getPose() const = 0;
    };

    namespace PoseParameterType {
        enum PoseParameterTypeEnum {
            EMPTY,
            VALUE,
            TRANSLATION,
            ORIENTATION,
            POSE,
            EXTERNAL_POSE
        };
    }

    namespace OperationType {
        enum OperationTypeEnum {
            VALUE,

            RESERVED_TRANSLATION_ENTRY_START,
            TRANSLATION_X,
            TRANSLATION_Y,
            TRANSLATION_Z,
            TRANSLATION_NORM,
            RESERVED_TRANSLATION_ENTRY_END,

            RESERVED_ORIENTATION_ENTRY_START,
            ORIENTATION_W,
            ORIENTATION_X,
            ORIENTATION_Y,
            ORIENTATION_Z,
            ORIENTATION_NORM,
            ORIENTATION_ANGLE,
            RESERVED_ORIENTATION_ENTRY_END,

            RESERVED_ORIENTATION_AXIS_START,
            ORIENTATION_AXIS_X,
            ORIENTATION_AXIS_Y,
            ORIENTATION_AXIS_Z,
            RESERVED_ORIENTATION_AXIS_END,

            RESERVED_DELTA_START,
            DELTA_TRANSLATION,
            DELTA_ORIENTATION,
            DELTA_POSE,
            RESERVED_DELTA_END,
        };

        std::string convertOperationTypeToString(OperationTypeEnum const &operationType);

        OperationTypeEnum convertStringToOperationType(std::string const &operationTypeEnum);

        PoseParameterType::PoseParameterTypeEnum operationTypePropagation(
                OperationTypeEnum const &operation,
                std::vector<PoseParameterType::PoseParameterTypeEnum> const &inputTypes);
    }

    template<typename T>
    class PoseParameter {
    public:
        PoseParameter() : val(), parameterType(PoseParameterType::EMPTY), externalPose(nullptr) {}

        explicit PoseParameter(T val) : val(std::move(val)), parameterType(PoseParameterType::VALUE),
                                        externalPose(nullptr) {}

        explicit PoseParameter(Eigen::Matrix<T, 3, 1> t) :
                val(), t(std::move(t)), parameterType(PoseParameterType::TRANSLATION), externalPose(nullptr) {}

        explicit PoseParameter(Eigen::Quaternion<T> q) :
                val(), q(std::move(q)), parameterType(PoseParameterType::ORIENTATION), externalPose(nullptr) {}

        explicit PoseParameter(AndreiUtils::DualQuaternion<T> p) :
                val(), parameterType(PoseParameterType::POSE), p(std::move(p)), externalPose(nullptr) {}

        explicit PoseParameter(std::shared_ptr<ExternalPoseInterface<T>> ref) :
                val(), externalPose(std::move(ref)), parameterType(PoseParameterType::EXTERNAL_POSE) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> getPoseFromPoseData() const {
            if (this->parameterType == PoseParameterType::POSE) {
                return this->p;
            } else if (this->parameterType == PoseParameterType::EXTERNAL_POSE) {
                return this->externalPose->getPose();
            }
            throw std::runtime_error(
                    "Can't retrieve pose from this parameter type: " + std::to_string(this->parameterType));
        }

        // value functions

        [[nodiscard]] T getValue() const {
            return this->getValue(nullptr, "");
        }

        [[nodiscard]] PoseParameter<T> getValueParameter() const {
            return this->getValueParameter(nullptr, "");
        }

        [[nodiscard]] T getValue(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getValue(&cache, parameterName);
        }

        [[nodiscard]] PoseParameter<T> getValueParameter(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getValueParameter(&cache, parameterName);
        }

        [[nodiscard]] T getValue(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            PoseParameter<T> *cachedParameter;
            std::string cacheEntry = parameterName + " -> v";
            if (cache != nullptr && mapGetIfContains(*cache, cacheEntry, cachedParameter)) {
                assert(cachedParameter->parameterType == PoseParameterType::VALUE);
                return cachedParameter->val;
            }
            if (this->parameterType != PoseParameterType::VALUE) {
                throw std::runtime_error("Can not return value from a " + std::to_string(this->parameterType) + " parameter type!");
            }
            if (cache != nullptr) {
                mapEmplace(*cache, cacheEntry, this->value);
            }
            return this->value;
        }

        [[nodiscard]] PoseParameter<T> getValueParameter(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            return PoseParameter<T>(this->getValue(cache, parameterName));
        }

        // translation functions

        [[nodiscard]] Eigen::Matrix<T, 3, 1> getTranslation() const {
            return this->getTranslation(nullptr, "");
        }

        [[nodiscard]] PoseParameter<T> getTranslationParameter() const {
            return this->getTranslationParameter(nullptr, "");
        }

        [[nodiscard]] Eigen::Matrix<T, 3, 1> getTranslation(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getTranslation(&cache, parameterName);
        }

        [[nodiscard]] PoseParameter<T> getTranslationParameter(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getTranslationParameter(&cache, parameterName);
        }

        [[nodiscard]] Eigen::Matrix<T, 3, 1> getTranslation(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            PoseParameter<T> *cachedParameter;
            std::string cacheEntry = parameterName + " -> t";
            if (cache != nullptr && mapGetIfContains(*cache, cacheEntry, cachedParameter)) {
                assert(cachedParameter->parameterType == PoseParameterType::TRANSLATION);
                return cachedParameter->t;
            }
            if (this->parameterType != PoseParameterType::TRANSLATION &&
                this->parameterType != PoseParameterType::POSE &&
                this->parameterType != PoseParameter::EXTERNAL_POSE) {
                throw std::runtime_error("Can not return translation from a " + std::to_string(this->parameterType) + " parameter type!");
            }

            if (this->parameterType == PoseParameterType::TRANSLATION) {
                if (cache != nullptr) {
                    mapEmplace(*cache, cacheEntry, this->t);
                }
                return this->t;
            }

            auto pose = this->getPose(cache, parameterName);
            auto t = pose.getTranslation();
            if (cache != nullptr) {
                mapEmplace(*cache, cacheEntry, t);
            }
            return t;
        }

        [[nodiscard]] PoseParameter<T> getTranslationParameter(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            return PoseParameter<T>(this->getTranslation(cache, parameterName));
        }

        // orientation functions

        [[nodiscard]] Eigen::Quaternion<T> getOrientation() const {
            return this->getOrientation(nullptr, "");
        }

        [[nodiscard]] PoseParameter<T> getOrientationParameter() const {
            return this->getOrientationParameter(nullptr, "");
        }

        [[nodiscard]] Eigen::Quaternion<T> getOrientation(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getOrientation(&cache, parameterName);
        }

        [[nodiscard]] PoseParameter<T> getOrientationParameter(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getOrientationParameter(&cache, parameterName);
        }

        [[nodiscard]] Eigen::Quaternion<T> getOrientation(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            PoseParameter<T> *cachedParameter;
            std::string cacheEntry = parameterName + " -> q";
            if (cache != nullptr && mapGetIfContains(*cache, cacheEntry, cachedParameter)) {
                assert(cachedParameter->parameterType == PoseParameterType::ORIENTATION);
                return cachedParameter->q;
            }
            if (this->parameterType != PoseParameterType::ORIENTATION &&
                this->parameterType != PoseParameterType::POSE &&
                this->parameterType != PoseParameter::EXTERNAL_POSE) {
                throw std::runtime_error("Can not return orientation from a " + std::to_string(this->parameterType) + " parameter type!");
            }

            if (this->parameterType == PoseParameterType::ORIENTATION) {
                if (cache != nullptr) {
                    mapEmplace(*cache, cacheEntry, this->q);
                }
                return this->q;
            }

            auto pose = this->getPose(cache, parameterName);
            auto q = pose.getOrientation();
            if (cache != nullptr) {
                mapEmplace(*cache, cacheEntry, q);
            }
            return q;
        }

        [[nodiscard]] PoseParameter<T> getOrientationParameter(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            return PoseParameter<T>(this->getOrientation(cache, parameterName));
        }

        // pose functions

        [[nodiscard]] AndreiUtils::DualQuaternion<T> getPose() const {
            return this->getPose(nullptr, "");
        }

        [[nodiscard]] PoseParameter<T> getPoseParameter() const {
            return this->getPoseParameter(nullptr, "");
        }

        [[nodiscard]] AndreiUtils::DualQuaternion<T> getPose(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getPose(&cache, parameterName);
        }

        [[nodiscard]] PoseParameter<T> getPoseParameter(std::map<std::string, PoseParameter<T>> &cache, std::string const &parameterName) const {
            return this->getPoseParameter(&cache, parameterName);
        }

        [[nodiscard]] AndreiUtils::DualQuaternion<T> getPose(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            PoseParameter<T> *cachedParameter;
            std::string cacheEntry = parameterName + " -> p";
            if (cache != nullptr && mapGetIfContains(*cache, cacheEntry, cachedParameter)) {
                assert(cachedParameter->parameterType == PoseParameterType::POSE);
                return cachedParameter->p;
            }
            if (this->parameterType != PoseParameterType::POSE &&
                this->parameterType != PoseParameter::EXTERNAL_POSE) {
                throw std::runtime_error("Can not return pose from a " + std::to_string(this->parameterType) + " parameter type!");
            }

            auto pose = this->getPoseFromPoseData();
            if (cache != nullptr) {
                mapEmplace(*cache, cacheEntry, pose);
            }
            return pose;
        }

        [[nodiscard]] PoseParameter<T> getPoseParameter(std::map<std::string, PoseParameter<T>> *cache, std::string const &parameterName) const {
            return PoseParameter<T>(this->getPose(cache, parameterName));
        }

        PoseParameterType::PoseParameterTypeEnum parameterType;
        T val;
        Eigen::Matrix<T, 3, 1> t;
        Eigen::Quaternion<T> q;
        AndreiUtils::DualQuaternion<T> p;
        std::shared_ptr<ExternalPoseInterface<T>> externalPose;
    };

    template<typename T>
    class ParameterOperation {
        using OperationType = OperationType::OperationTypeEnum;
    public:
        static PoseParameterType::PoseParameterTypeEnum operationTypePropagation(
                OperationType const &operation, std::vector<PoseParameter<T>> const &inputs) {
            std::vector<PoseParameterType::PoseParameterTypeEnum> inputTypes(inputs.size());
            for (size_t i = 0; i < inputs.size(); ++i) {
                inputTypes[i] = inputs[i].parameterType;
            }
            return AndreiUtils::OperationType::operationTypePropagation(operation, inputTypes);
        }

        PoseParameter<T> performOperations(
                std::vector<PoseParameter<T>> const &inputs) const {
            return this->performOperations(inputs, nullptr);
        }

        PoseParameter<T> performOperations(
                std::vector<PoseParameter<T>> const &inputs, std::map<std::string, PoseParameter<T>> &cache) {
            return this->performOperations(inputs, &cache);
        }

        PoseParameter<T> performOperations(
                std::vector<PoseParameter<T>> const &inputs, std::map<std::string, PoseParameter<T>> *cache) {
            if (this->operations.empty()) {
                return {};
            }
            std::vector<PoseParameter<T>> results;
            std::vector<std::string> resultNames;
            for (auto const &opData: this->operations) {
                std::vector<PoseParameter<T>> operationInputs;
                std::vector<std::string> operationInputNames;
                std::string parameterCachePrepend;
                for (auto const &parameterIndex: opData.second) {
                    if (!parameterCachePrepend.empty()) {
                        parameterCachePrepend += ", ";
                    }
                    if (parameterIndex >= 0) {
                        operationInputs.emplace_back(inputs[parameterIndex]);
                        operationInputNames.emplace_back(std::to_string(parameterIndex));
                    } else {
                        if (-parameterIndex > results.size()) {
                            throw std::runtime_error(
                                    "Operation sequence is misformed; requested result of " +
                                    std::to_string(parameterIndex) + " but the size of the result is " +
                                    std::to_string(results.size()));
                        }
                        operationInputs.emplace_back(*(results.end() + parameterIndex));  // parameterIndex <= -1
                        operationInputNames = *(resultNames.end() + parameterIndex);
                    }
                    parameterCachePrepend += "(" + operationInputNames.back() + ")";
                }
                resultNames.emplace_back(
                        parameterCachePrepend + " -> " + AndreiUtils::OperationType::convertOperationTypeToString(opData.first));
                results.emplace_back(ParameterOperation::doOperation(
                        opData.first, operationInputs, cache, resultNames.back(), operationInputNames));
            }
            return results.back();
        }

        static PoseParameter<T> doOperation(
                OperationType const &operationType, std::vector<PoseParameter<T>> const &inputs,
                std::map<std::string, PoseParameter<T>> *cache, std::string const &operationCacheEntry,
                std::vector<std::string> inputNames) {
            PoseParameter<T> *cachedParameter;
            if (cache != nullptr && mapGetIfContains(*cache, operationCacheEntry, cachedParameter)) {
                return *cachedParameter;
            }

            if (operationType == OperationType::VALUE) {
                assert(inputs.size() == 1);
                auto const &inputParam = inputs[0];
                auto const &inputParamName = inputNames[0];

                return inputParam.getValueParameter(cache, inputParamName);  // caching is done inside this function! ;)
            }
            if (operationType > OperationType::RESERVED_TRANSLATION_ENTRY_START &&
                operationType < OperationType::RESERVED_TRANSLATION_ENTRY_END) {
                assert(inputs.size() == 1);
                auto const &inputParam = inputs[0];
                auto const &inputParamName = inputNames;

                auto t = inputParam.getTranslation(cache, inputParamName);
                auto res = ParameterOperation::translationEntryGetter(t);
                if (cache != nullptr) {
                    return mapEmplace(*cache, operationCacheEntry, res)->second;  // cache result!
                }
                return PoseParameter<T>(res);
            }
            if ((operationType > OperationType::RESERVED_ORIENTATION_ENTRY_START &&
                 operationType < OperationType::RESERVED_ORIENTATION_ENTRY_END) ||
                (operationType > OperationType::RESERVED_ORIENTATION_AXIS_START &&
                 operationType < OperationType::RESERVED_ORIENTATION_AXIS_END)) {
                assert(inputs.size() == 1);
                auto const &inputParam = inputs[0];
                auto const &inputParamName = inputNames[0];

                auto q = inputParam.getOrientation(cache, inputParamName);

                if (operationType > OperationType::RESERVED_ORIENTATION_AXIS_START &&
                    operationType < OperationType::RESERVED_ORIENTATION_AXIS_END) {
                    auto res = ParameterOperation::orientationAxisGetter(q);
                    if (cache != nullptr) {
                        return mapEmplace(*cache, operationCacheEntry, res)->second;  // cache result!
                    }
                    return PoseParameter<T>(res);
                }

                auto res = ParameterOperation::orientationEntryGetter(q);
                if (cache != nullptr) {
                    return mapEmplace(*cache, operationCacheEntry, res)->second;  // cache result!
                }
                return PoseParameter<T>(res);
            }
            if (operationType > OperationType::RESERVED_DELTA_START &&
                operationType < OperationType::RESERVED_DELTA_END) {
                assert(inputs.size() == 2);
                auto const &inputParam1 = inputs[0];
                auto const &inputParam2 = inputs[1];
                auto const &inputParam1Name = inputNames[0];
                auto const &inputParam2Name = inputNames[1];

                if (operationType == OperationType::DELTA_TRANSLATION) {
                    auto t1 = inputParam1.getTranslation(cache, inputParam1Name);
                    auto t2 = inputParam2.getTranslation(cache, inputParam2Name);
                    Eigen::Matrix<T, 3, 1> res = t2 - t1;
                    if (cache != nullptr) {
                        return mapEmplace(*cache, operationCacheEntry, res)->second;  // cache result!
                    }
                    return PoseParameter<T>(res);
                }

                if (operationType == OperationType::DELTA_ORIENTATION) {
                    auto q1 = inputParam1.getOrientation(cache, inputParam1Name);
                    auto q2 = inputParam2.getOrientation(cache, inputParam2Name);
                    Eigen::Quaternion<T> res = q1.inv() * q2;
                    if (cache != nullptr) {
                        return mapEmplace(*cache, operationCacheEntry, res)->second;  // cache result!
                    }
                    return PoseParameter<T>(res);
                }

                assert(operationType == OperationType::DELTA_POSE);
                auto p1 = inputParam1.getPose(cache, inputParam1Name);
                auto p2 = inputParam2.getPose(cache, inputParam2Name);
                AndreiUtils::Posed res = p1.inverse() * p2;
                if (cache != nullptr) {
                    return mapEmplace(cache, operationCacheEntry, res)->second;  // cache result!
                }
                return PoseParameter<T>(res);
            }

            throw std::runtime_error("Unknown operation to execute: " + std::to_string(operationType));
        }

        explicit ParameterOperation(nlohmann::json const &operationConfig) {
            if (operationConfig.is_array()) {
                for (auto const &opConfig: operationConfig.get<std::vector<nlohmann::json>>()) {
                    this->processOneOperation(opConfig);
                }
            } else if (operationConfig.is_object()) {
                this->processOneOperation(operationConfig);
            } else {
                throw std::runtime_error("Unknown operation config type!");
            }
        }

        std::vector<std::pair<OperationType, std::vector<int>>> operations;

    protected:
        void processOneOperation(nlohmann::json const &operationConfig) {
            assert(operationConfig.contains("op") &&
                   (operationConfig.contains("parameter") || operationConfig.contains("parameters")));
            std::vector<int> parameterIndices;
            if (operationConfig.contains("parameter")) {
                parameterIndices.emplace_back(operationConfig.at("parameter").get<int>());
            } else {
                assert(operationConfig.contains("parameters"));
                parameterIndices = operationConfig.at("parameters").get<std::vector<int>>();
            }
            std::string stringOp = operationConfig.at("op").get<std::string>();
            OperationType operation = AndreiUtils::OperationType::convertStringToOperationType(stringOp);

            this->operations.emplace_back(operation, parameterIndices);
        }

        [[nodiscard]] static T translationEntryGetter(
                Eigen::Matrix<T, 3, 1> const &t, OperationType const &operationType) {
            assert(operationType > OperationType::RESERVED_TRANSLATION_ENTRY_START &&
                   operationType < OperationType::RESERVED_TRANSLATION_ENTRY_END);
            return operationType == OperationType::TRANSLATION_X ? t.x() :
                   (operationType == OperationType::TRANSLATION_Y ? t.y() :
                    (operationType == OperationType::TRANSLATION_Z ? t.z() : t.norm()));
        }

        [[nodiscard]] static T orientationEntryGetter(
                Eigen::Quaternion<T> const &q, OperationType const &operationType) {
            assert(operationType > OperationType::RESERVED_ORIENTATION_ENTRY_START &&
                   operationType < OperationType::RESERVED_ORIENTATION_ENTRY_END);
            return operationType == OperationType::ORIENTATION_W ? q.w() :
                   (operationType == OperationType::ORIENTATION_X ? q.x() :
                    (operationType == OperationType::ORIENTATION_Y ? q.y() :
                     (operationType == OperationType::ORIENTATION_Z ? q.z() :
                      (operationType == OperationType::ORIENTATION_NORM ?
                       q.norm() : Eigen::AngleAxis<T>(q).angle()))));
        }

        [[nodiscard]] static Eigen::Matrix<T, 3, 1> orientationAxisGetter(
                Eigen::Quaternion<T> const &q, OperationType const &operationType) {
            assert(operationType > OperationType::RESERVED_ORIENTATION_AXIS_START &&
                   operationType < OperationType::RESERVED_ORIENTATION_AXIS_END);
            if (operationType == OperationType::ORIENTATION_AXIS_X) {
                return q.toRotationMatrix().col(0);
            } else if (operationType == OperationType::ORIENTATION_AXIS_Y) {
                return q.toRotationMatrix().col(1);
            } else {
                assert(operationType == OperationType::ORIENTATION_AXIS_Z);
                return q.toRotationMatrix().col(2);
            }
        }
    };

    template<typename T>
    class PoseParameters {
        using PoseParameterType = PoseParameterType::PoseParameterTypeEnum;
        using ExternalPoseCreator = std::function<bool(nlohmann::json const &,
                                                       std::shared_ptr<ExternalPoseInterface<T>> &)>;
    public:
        PoseParameters() = default;

        explicit PoseParameters(nlohmann::json const &parametersConfig) : PoseParameters(parametersConfig, {}) {}

        PoseParameters(nlohmann::json const &parametersConfig, ExternalPoseCreator externalPoseCreator)
                : outputTransformation() {
            if (parametersConfig.contains("parameterData")) {
                auto parameterOperations = parametersConfig.at(
                        "parameterData").get<std::map<std::string, nlohmann::json>>();
                for (auto const &parameterOp: parameterOperations) {
                    mapEmplace(this->outputTransformation, parameterOp.first, parameterOp.second);
                }
            }
            assert(this->outputTransformation.empty() || parametersConfig.contains("parameters"));
            if (parametersConfig.contains("parameters")) {
                auto parametersJson = parametersConfig.at("parameters").get<std::vector<nlohmann::json>>();
                AndreiUtils::DualQuaternion<T> res;
                for (auto const &param: parametersJson) {
                    if (param.is_number()) {
                        this->inputParameters.emplace_back(param.get<T>());
                        continue;
                    }
                    if (param.is_array()) {
                        // first try if translation or orientation
                        // ONLY AFTER try if pose; pose can be constructed from orientation or translation alone
                        if (param.size() == 3) {
                            if (param[0].is_number() && param[1].is_number() && param[2].is_number()) {
                                this->inputParameters.emplace_back(Eigen::Matrix<T, 3, 1>{
                                        param[0].get<T>(), param[0].get<T>(), param[2].get<T>()});
                                continue;
                            } else if (canConvertJsonTo(param, res)) {
                                this->inputParameters.emplace_back(res);
                                continue;
                            }
                        } else if (param.size() == 4) {
                            if (param[0].is_number() && param[1].is_number() && param[2].is_number() &&
                                param[3].is_number()) {
                                this->inputParameters.emplace_back(Eigen::Quaternion<T>{
                                        param[0].get<T>(), param[0].get<T>(), param[2].get<T>(), param[3].get<T>()});
                                continue;
                            } else if (canConvertJsonTo(param, res)) {
                                this->inputParameters.emplace_back(res);
                                continue;
                            }
                        } else if (canConvertJsonTo(param, res)) {
                            this->inputParameters.emplace_back(res);
                            continue;
                        }
                    }
                    std::shared_ptr<ExternalPoseInterface<T>> externalPose;
                    if (externalPoseCreator && externalPoseCreator(param, externalPose)) {
                        this->inputParameters.emplace_back(externalPose);
                    } else {
                        throw std::runtime_error("Can't process pose parameter: " + param.dump(4));
                    }
                }
            }

            // do a trial run of passing the parameters through the operation sequence and ensure that a VALUE is result
            for (auto const &parameterOp: this->outputTransformation) {
                std::vector<PoseParameterType> results;
                for (auto const &opData: parameterOp.second.operations) {
                    std::vector<PoseParameterType> inputTypes;
                    for (auto const &parameterIndex: opData.second) {
                        if (parameterIndex >= 0) {
                            inputTypes.emplace_back(this->inputParameters[parameterIndex].parameterType);
                        } else {
                            if (-parameterIndex > results.size()) {
                                throw std::runtime_error(
                                        "Operation sequence for " + parameterOp.first +
                                        " is misformed; requested result of " + std::to_string(parameterIndex) +
                                        " but the size of the result is " + std::to_string(results.size()));
                            }
                            inputTypes.emplace_back(results.end() + parameterIndex);  // parameterIndex <= -1
                        }
                    }
                    results.emplace_back(OperationType::operationTypePropagation(opData.first, inputTypes));
                }
                if (results.back() != PoseParameterType::VALUE) {
                    throw std::runtime_error(
                            "Operation sequence for " + parameterOp.first + " is mis-formed; " +
                            "end-result of the sequence is not a VALUE, but a " + std::to_string(results.back()));
                }
            }
        }

        [[nodiscard]] std::map<std::string, T> get() const {
            std::map<std::string, PoseParameter<T>> cache;
            std::map<std::string, T> parameterAssignment;
            for (auto const &paramData: this->outputTransformation) {
                auto result = paramData.second.performOperations(this->inputParameters, cache);
                if (result.parameterType != PoseParameterType::VALUE) {
                    throw std::runtime_error("Operation sequence did not result in a value, but in " +
                                             std::to_string(result.parameterType));
                }
                T parameterValue = result.val;
                /*
                cout << "Param " << paramData.first << " from "
                     << ParameterOperation::getCacheEntryString(paramData.second.operation) << ": " << parameterValue << endl;
                //*/
                mapEmplace(parameterAssignment, paramData.first, parameterValue);
            }
            return parameterAssignment;
        }

    protected:
        std::vector<PoseParameter<T>> inputParameters;
        std::map<std::string, ParameterOperation<T>> outputTransformation;
    };

    using ParametrizablePosefParameters = PoseParameters<float>;
    using ParametrizablePosedParameters = PoseParameters<double>;
}

