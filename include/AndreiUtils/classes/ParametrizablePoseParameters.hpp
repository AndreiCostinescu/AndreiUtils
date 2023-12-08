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

    template<typename T>
    class PoseParameter {
    public:
        enum PoseParameterType {
            EMPTY,
            VALUE,
            TRANSLATION,
            ORIENTATION,
            POSE,
            REFERENCE
        };

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
                val(), externalPose(std::move(ref)), parameterType(PoseParameterType::REFERENCE) {}

        [[nodiscard]] AndreiUtils::DualQuaternion<T> getPoseFromPoseData() const {
            if (this->parameterType == PoseParameterType::POSE) {
                return this->p;
            } else if (this->parameterType == PoseParameterType::REFERENCE) {
                return this->externalPose->getPose();
            }
            throw std::runtime_error(
                    "Can't retrieve pose from this parameter type: " + std::to_string(this->parameterType));
        }

        PoseParameterType parameterType;
        T val;
        Eigen::Matrix<T, 3, 1> t;
        Eigen::Quaternion<T> q;
        AndreiUtils::DualQuaternion<T> p;
        std::shared_ptr<ExternalPoseInterface<T>> externalPose;
    };

    class ParameterOperation {
    public:
        enum OperationType {
            VALUE,

            TRANSLATION_ENTRY_START,
            TRANSLATION_X,
            TRANSLATION_Y,
            TRANSLATION_Z,
            TRANSLATION_NORM,
            TRANSLATION_ENTRY_END,

            ORIENTATION_ENTRY_START,
            ORIENTATION_W,
            ORIENTATION_X,
            ORIENTATION_Y,
            ORIENTATION_Z,
            ORIENTATION_NORM,
            ORIENTATION_ANGLE,
            ORIENTATION_ENTRY_END,

            ORIENTATION_AXIS_START,
            ORIENTATION_AXIS_X_X,
            ORIENTATION_AXIS_X_Y,
            ORIENTATION_AXIS_X_Z,
            ORIENTATION_AXIS_Y_X,
            ORIENTATION_AXIS_Y_Y,
            ORIENTATION_AXIS_Y_Z,
            ORIENTATION_AXIS_Z_X,
            ORIENTATION_AXIS_Z_Y,
            ORIENTATION_AXIS_Z_Z,
            ORIENTATION_AXIS_END,

            DELTA_ORIENTATION_AXIS_START,
            DELTA_ORIENTATION_AXIS_X_X,
            DELTA_ORIENTATION_AXIS_X_Y,
            DELTA_ORIENTATION_AXIS_X_Z,
            DELTA_ORIENTATION_AXIS_Y_X,
            DELTA_ORIENTATION_AXIS_Y_Y,
            DELTA_ORIENTATION_AXIS_Y_Z,
            DELTA_ORIENTATION_AXIS_Z_X,
            DELTA_ORIENTATION_AXIS_Z_Y,
            DELTA_ORIENTATION_AXIS_Z_Z,
            DELTA_ORIENTATION_AXIS_END
        };

        static std::string getCacheEntryString(OperationType const &operationType);

        static OperationType operationReduction(OperationType const &operationType);

        explicit ParameterOperation(nlohmann::json const &operationConfig);

        OperationType operation;
        std::vector<int> parameterIndices;
    };

    template<typename T>
    class PoseParameters {
        using PoseParameterType = typename PoseParameter<T>::PoseParameterType;
        using ExternalPoseCreator = typename std::function<bool(nlohmann::json const &,
                                                                std::shared_ptr<ExternalPoseInterface<T>> &)>;
    public:
        PoseParameters() = default;

        explicit PoseParameters(nlohmann::json const &parametersConfig) : PoseParameters(parametersConfig, {}) {}

        PoseParameters(nlohmann::json const &parametersConfig, ExternalPoseCreator externalPoseCreator) {
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
        }

        [[nodiscard]] std::map<std::string, T> get() const {
            std::map<std::string, PoseParameter<T>> cache;
            std::map<std::string, T> parameterAssignment;
            for (auto const &paramData: this->outputTransformation) {
                T parameterValue = this->getParameterValue(cache, paramData.second);
                /*
                cout << "Param " << paramData.first << " from "
                     << ParameterOperation::getCacheEntryString(paramData.second.operation) << ": " << parameterValue << endl;
                //*/
                mapEmplace(parameterAssignment, paramData.first, parameterValue);
            }
            return parameterAssignment;
        }

    protected:
        [[nodiscard]] T getParameterValue(
                std::map<std::string, PoseParameter<T>> &cache, ParameterOperation const &operation) const {
            std::vector<PoseParameter<T> const *> inputParams;
            std::string parameterCachePrepend;
            for (auto const &parameterIndex: operation.parameterIndices) {
                inputParams.emplace_back(&this->inputParameters[parameterIndex]);
                parameterCachePrepend += std::to_string(parameterIndex) + " || ";
            }

            auto operationType = operation.operation;
            std::string operationCache = parameterCachePrepend + ParameterOperation::getCacheEntryString(operationType);

            PoseParameter<T> *cachedParameter;
            if (mapGetIfContains(cache, operationCache, cachedParameter)) {
                assert(cachedParameter->parameterType == PoseParameter<T>::VALUE);
                return cachedParameter->val;
            }

            if (operationType == ParameterOperation::VALUE) {
                assert(inputParams.size() == 1);
                auto const &inputParam = *inputParams[0];
                if (inputParam.parameterType != PoseParameterType::VALUE) {
                    throw std::runtime_error("Can't perform operation VALUE on an input of type " +
                                             std::to_string(inputParam.parameterType));
                }
                mapEmplace(cache, operationCache, inputParam);  // cache result!
                return inputParam.val;
            } else if (operationType > ParameterOperation::TRANSLATION_ENTRY_START &&
                       operationType < ParameterOperation::TRANSLATION_ENTRY_END) {
                assert(inputParams.size() == 1);
                auto const &inputParam = *inputParams[0];
                if (inputParam.parameterType == PoseParameterType::EMPTY ||
                    inputParam.parameterType == PoseParameterType::VALUE ||
                    inputParam.parameterType == PoseParameterType::ORIENTATION) {
                    throw std::runtime_error("Can't perform operation " + std::to_string(operationType) +
                                             " on an input of type " + std::to_string(inputParam.parameterType));
                }

                std::string tCacheEntry = operationCache + ": t";
                if (mapGetIfContains<std::string>(cache, tCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::TRANSLATION);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                if (inputParam.parameterType == PoseParameterType::TRANSLATION) {
                    cachedParameter = &(mapEmplace(cache, tCacheEntry, inputParam.t)->second);  // cache result!
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                std::string pCacheEntry = operationCache + ": p";
                if (mapGetIfContains<std::string>(cache, pCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::POSE);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                cachedParameter = &(mapEmplace(cache, tCacheEntry,
                                               inputParam.getPoseFromPoseData())->second);  // cache result!
                return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
            } else if ((operationType > ParameterOperation::ORIENTATION_ENTRY_START &&
                        operationType < ParameterOperation::ORIENTATION_ENTRY_END) ||
                       (operationType > ParameterOperation::ORIENTATION_AXIS_START &&
                        operationType < ParameterOperation::ORIENTATION_AXIS_END)) {
                assert(inputParams.size() == 1);
                auto const &inputParam = *inputParams[0];
                if (inputParam.parameterType == PoseParameterType::EMPTY ||
                    inputParam.parameterType == PoseParameterType::VALUE ||
                    inputParam.parameterType == PoseParameterType::TRANSLATION) {
                    throw std::runtime_error("Can't perform operation " + std::to_string(operationType) +
                                             " on an input of type " + std::to_string(inputParam.parameterType));
                }

                if (operationType > ParameterOperation::ORIENTATION_AXIS_START &&
                    operationType < ParameterOperation::ORIENTATION_AXIS_END) {
                    std::string axisCacheEntry = AndreiUtils::removeRight(operationCache, 2) + ": axis";
                    if (mapGetIfContains<std::string>(cache, axisCacheEntry, cachedParameter)) {
                        assert(cachedParameter->parameterType == PoseParameterType::TRANSLATION);
                        return PoseParameters::getParameterValue(
                                cache, ParameterOperation::operationReduction(operationType), *cachedParameter,
                                operationCache);
                    }
                }

                std::string qCacheEntry = operationCache + ": q";
                if (mapGetIfContains<std::string>(cache, qCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::ORIENTATION);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                if (inputParam.parameterType == PoseParameterType::ORIENTATION) {
                    cachedParameter = &(mapEmplace(cache, qCacheEntry, inputParam.q)->second);  // cache result!
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                std::string pCacheEntry = operationCache + ": p";
                if (mapGetIfContains<std::string>(cache, pCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::POSE);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                cachedParameter = &(mapEmplace(cache, pCacheEntry,
                                               inputParam.getPoseFromPoseData())->second);  // cache result!
                return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
            } else if (operationType > ParameterOperation::DELTA_ORIENTATION_AXIS_START &&
                       operationType < ParameterOperation::DELTA_ORIENTATION_AXIS_END) {
                assert(inputParams.size() == 2);
                auto const &inputParam1 = *inputParams[0];
                auto const &inputParam2 = *inputParams[1];
                if (inputParam1.parameterType == PoseParameterType::EMPTY ||
                    inputParam1.parameterType == PoseParameterType::VALUE ||
                    inputParam1.parameterType == PoseParameterType::TRANSLATION ||
                    inputParam2.parameterType == PoseParameterType::EMPTY ||
                    inputParam2.parameterType == PoseParameterType::VALUE ||
                    inputParam2.parameterType == PoseParameterType::TRANSLATION) {
                    throw std::runtime_error("Can't perform operation " + std::to_string(operationType) +
                                             " on inputs of type " + std::to_string(inputParam1.parameterType) +
                                             " and " + std::to_string(inputParam2.parameterType));
                }
                // remove the delta from the operation type
                operationType = ParameterOperation::operationReduction(operationType);

                std::string axisCacheEntry = AndreiUtils::removeRight(operationCache, 2) + ": axis";
                if (mapGetIfContains<std::string>(cache, axisCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::TRANSLATION);
                    return PoseParameters::getParameterValue(
                            cache, ParameterOperation::operationReduction(operationType), *cachedParameter,
                            operationCache);
                }
                std::string qCacheEntry = operationCache + ": q";
                if (mapGetIfContains<std::string>(cache, qCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::ORIENTATION);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }
                std::string pCacheEntry = operationCache + ": p";
                if (mapGetIfContains<std::string>(cache, pCacheEntry, cachedParameter)) {
                    assert(cachedParameter->parameterType == PoseParameterType::POSE);
                    return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
                }

                AndreiUtils::Posed pose1 = inputParam1.getPoseFromPoseData();
                AndreiUtils::Posed pose2 = inputParam2.getPoseFromPoseData();
                AndreiUtils::Posed deltaP = pose1.inverse() * pose2;
                cachedParameter = &(mapEmplace(cache, pCacheEntry, deltaP)->second);  // cache result!
                return PoseParameters::getParameterValue(cache, operationType, *cachedParameter, operationCache);
            } else {
                throw std::runtime_error("Unknown operation to execute on pose parameter: " +
                                         std::to_string(operationType));
            }
        }

        [[nodiscard]] static T getParameterValue(
                std::map<std::string, PoseParameter<T>> &cache, ParameterOperation::OperationType const &operationType,
                PoseParameter<T> const &cachedParameter, std::string const &operationCache) {
            switch (cachedParameter.parameterType) {
                case PoseParameterType::EMPTY: {
                    throw std::runtime_error("Can't perform any operation on an empty parameter!");
                }
                case PoseParameterType::VALUE: {
                    if (operationType != ParameterOperation::VALUE) {
                        throw std::runtime_error(
                                "A value parameter can only give result to a value operation, not to " +
                                operationCache);
                    }
                    mapEmplace(cache, operationCache, cachedParameter.val);
                    return cachedParameter.val;
                }
                case PoseParameterType::TRANSLATION: {
                    if (!(operationType > ParameterOperation::TRANSLATION_ENTRY_START &&
                          operationType < ParameterOperation::TRANSLATION_ENTRY_END)) {
                        throw std::runtime_error(
                                "A translation parameter can only give result to translation operations, not to " +
                                operationCache);
                    }
                    T parameterValue = PoseParameters::translationEntryGetter(cachedParameter.t, operationType);
                    mapEmplace<std::string>(cache, operationCache, parameterValue);
                    return parameterValue;
                }
                case PoseParameterType::ORIENTATION: {
                    if (!((operationType > ParameterOperation::ORIENTATION_ENTRY_START &&
                           operationType < ParameterOperation::ORIENTATION_ENTRY_END) ||
                          (operationType > ParameterOperation::ORIENTATION_AXIS_START &&
                           operationType < ParameterOperation::ORIENTATION_AXIS_END))) {
                        throw std::runtime_error(
                                "An orientation parameter can only give result to orientation operations, not to " +
                                operationCache);
                    }

                    if (operationType > ParameterOperation::ORIENTATION_ENTRY_START &&
                        operationType < ParameterOperation::ORIENTATION_ENTRY_END) {
                        T parameterValue = PoseParameters::orientationEntryGetter(cachedParameter.q, operationType);
                        mapEmplace<std::string>(cache, operationCache, parameterValue);
                        return parameterValue;
                    }

                    Eigen::Vector3d axis = PoseParameters::orientationAxisGetter(cachedParameter.q, operationType);
                    std::string axisCacheEntry = AndreiUtils::removeRight(operationCache, 2) + ": axis";
                    return PoseParameters::getParameterValue(
                            cache, ParameterOperation::operationReduction(operationType),
                            mapEmplace<std::string>(cache, axisCacheEntry, axis)->second, operationCache);
                }
                case PoseParameterType::POSE:
                case PoseParameterType::REFERENCE: {
                    if (!((operationType > ParameterOperation::TRANSLATION_ENTRY_START &&
                           operationType < ParameterOperation::TRANSLATION_ENTRY_END) ||
                          (operationType > ParameterOperation::ORIENTATION_ENTRY_START &&
                           operationType < ParameterOperation::ORIENTATION_ENTRY_END) ||
                          (operationType > ParameterOperation::ORIENTATION_AXIS_START &&
                           operationType < ParameterOperation::ORIENTATION_AXIS_END))) {
                        throw std::runtime_error("A pose/reference parameter can only give result to a "
                                                 "translation, orientation, or pose operation, not to " +
                                                 operationCache);
                    }

                    std::string tCacheEntry = operationCache + ": t";
                    std::string qCacheEntry = operationCache + ": q";
                    PoseParameter<T> *passAlong;
                    AndreiUtils::Posed res = cachedParameter.getPoseFromPoseData();
                    if (operationType > ParameterOperation::TRANSLATION_ENTRY_START &&
                        operationType < ParameterOperation::TRANSLATION_ENTRY_END) {
                        passAlong = &(mapEmplace(cache, tCacheEntry, res.getTranslation())->second);
                        mapEmplace(cache, qCacheEntry, res.getRotation());
                    } else {
                        mapEmplace(cache, tCacheEntry, res.getTranslation());
                        passAlong = &(mapEmplace(cache, qCacheEntry, res.getRotation())->second);
                    }
                    return PoseParameters::getParameterValue(cache, operationType, *passAlong, operationCache);
                }
                default: {
                    throw std::runtime_error(
                            "Unknown pose parameter type: " + std::to_string(cachedParameter.parameterType));
                }
            }
        }

        [[nodiscard]] static T translationEntryGetter(
                Eigen::Matrix<T, 3, 1> const &t, ParameterOperation::OperationType const &operationType) {
            assert(operationType > ParameterOperation::TRANSLATION_ENTRY_START &&
                   operationType < ParameterOperation::TRANSLATION_ENTRY_END);
            return operationType == ParameterOperation::TRANSLATION_X ? t.x() :
                   (operationType == ParameterOperation::TRANSLATION_Y ? t.y() :
                    (operationType == ParameterOperation::TRANSLATION_Z ? t.z() : t.norm()));
        }

        [[nodiscard]] static T orientationEntryGetter(
                Eigen::Quaternion<T> const &q, ParameterOperation::OperationType const &operationType) {
            assert(operationType > ParameterOperation::ORIENTATION_ENTRY_START &&
                   operationType < ParameterOperation::ORIENTATION_ENTRY_END);
            return operationType == ParameterOperation::ORIENTATION_W ? q.w() :
                   (operationType == ParameterOperation::ORIENTATION_X ? q.x() :
                    (operationType == ParameterOperation::ORIENTATION_Y ? q.y() :
                     (operationType == ParameterOperation::ORIENTATION_Z ? q.z() :
                      (operationType == ParameterOperation::ORIENTATION_NORM ?
                       q.norm() : Eigen::AngleAxis<T>(q).angle()))));
        }

        [[nodiscard]] static Eigen::Matrix<T, 3, 1> orientationAxisGetter(
                Eigen::Quaternion<T> const &q, ParameterOperation::OperationType const &operationType) {
            assert(operationType > ParameterOperation::ORIENTATION_AXIS_START &&
                   operationType < ParameterOperation::ORIENTATION_AXIS_END);
            if (operationType == ParameterOperation::ORIENTATION_AXIS_X_X ||
                operationType == ParameterOperation::ORIENTATION_AXIS_X_Y ||
                operationType == ParameterOperation::ORIENTATION_AXIS_X_Z) {
                return q.toRotationMatrix().col(0);
            } else if (operationType == ParameterOperation::ORIENTATION_AXIS_Y_X ||
                       operationType == ParameterOperation::ORIENTATION_AXIS_Y_Y ||
                       operationType == ParameterOperation::ORIENTATION_AXIS_Y_Z) {
                return q.toRotationMatrix().col(1);
            } else {
                assert(operationType == ParameterOperation::ORIENTATION_AXIS_Z_X ||
                       operationType == ParameterOperation::ORIENTATION_AXIS_Z_Y ||
                       operationType == ParameterOperation::ORIENTATION_AXIS_Z_Z);
                return q.toRotationMatrix().col(2);
            }
        }

        std::vector<PoseParameter<T>> inputParameters;
        std::map<std::string, ParameterOperation> outputTransformation;
    };

    using ParametrizablePosefParameters = PoseParameters<float>;
    using ParametrizablePosedParameters = PoseParameters<double>;
}