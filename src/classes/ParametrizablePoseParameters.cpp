//
// Created by Andrei on 08.12.23.
//

#include <AndreiUtils/classes/ParametrizablePoseParameters.hpp>

using namespace AndreiUtils;

std::string OperationType::convertOperationTypeToString(OperationType::OperationTypeEnum const &operationType) {
    switch (operationType) {
        case VALUE:
            return "v";
        case TRANSLATION_X:
            return "tx";
        case TRANSLATION_Y:
            return "ty";
        case TRANSLATION_Z:
            return "tz";
        case TRANSLATION_NORM:
            return "tNorm";
        case ORIENTATION_W:
            return "qw";
        case ORIENTATION_X:
            return "qx";
        case ORIENTATION_Y:
            return "qy";
        case ORIENTATION_Z:
            return "qz";
        case ORIENTATION_NORM:
            return "qNorm";
        case ORIENTATION_ANGLE:
            return "qAngle";
        case ORIENTATION_AXIS_X:
            return "qAxisX";
        case ORIENTATION_AXIS_Y:
            return "qAxisY";
        case ORIENTATION_AXIS_Z:
            return "qAxisZ";
        case DELTA_TRANSLATION:
            return "deltaT";
        case DELTA_ORIENTATION:
            return "deltaQ";
        case DELTA_POSE:
            return "deltaP";
        default: {
            throw std::runtime_error("Unknown operation type to cache: " + std::to_string(operationType));
        }
    }
}

OperationType::OperationTypeEnum OperationType::convertStringToOperationType(std::string const &stringOp) {
    OperationType::OperationTypeEnum operation;
    if (stringOp == "tx") {
        operation = TRANSLATION_X;
    } else if (stringOp == "ty") {
        operation = TRANSLATION_Y;
    } else if (stringOp == "tz") {
        operation = TRANSLATION_Z;
    } else if (stringOp == "tNorm") {
        operation = TRANSLATION_NORM;
    } else if (stringOp == "qw") {
        operation = ORIENTATION_W;
    } else if (stringOp == "qx") {
        operation = ORIENTATION_X;
    } else if (stringOp == "qy") {
        operation = ORIENTATION_Y;
    } else if (stringOp == "qz") {
        operation = ORIENTATION_Z;
    } else if (stringOp == "qNorm") {
        operation = ORIENTATION_NORM;
    } else if (stringOp == "qAngle") {
        operation = ORIENTATION_ANGLE;
    } else if (stringOp == "qAxisX") {
        operation = ORIENTATION_AXIS_X;
    } else if (stringOp == "qAxisY") {
        operation = ORIENTATION_AXIS_Y;
    } else if (stringOp == "qAxisZ") {
        operation = ORIENTATION_AXIS_Z;
    } else if (stringOp == "deltaT") {
        operation = DELTA_TRANSLATION;
    } else if (stringOp == "deltaQ") {
        operation = DELTA_ORIENTATION;
    } else if (stringOp == "deltaP") {
        operation = DELTA_POSE;
    } else {
        throw std::runtime_error("Unknown pose operation to apply on parameter: " + stringOp);
    }
    return operation;
}

PoseParameterType::PoseParameterTypeEnum OperationType::operationTypePropagation(
        OperationType::OperationTypeEnum const &operation,
        std::vector<PoseParameterType::PoseParameterTypeEnum> const &inputTypes) {
    switch (operation) {
        case VALUE: {
            if (inputTypes.size() != 1 || inputTypes[0] != PoseParameterType::VALUE) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::VALUE;
        }
        case TRANSLATION_X:
        case TRANSLATION_Y:
        case TRANSLATION_Z:
        case TRANSLATION_NORM: {
            if (inputTypes.size() != 1 ||
                (inputTypes[0] != PoseParameterType::TRANSLATION && inputTypes[0] != PoseParameterType::POSE &&
                 inputTypes[0] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::VALUE;
        }
        case ORIENTATION_W:
        case ORIENTATION_X:
        case ORIENTATION_Y:
        case ORIENTATION_Z:
        case ORIENTATION_NORM:
        case ORIENTATION_ANGLE: {
            if (inputTypes.size() != 1 ||
                (inputTypes[0] != PoseParameterType::ORIENTATION && inputTypes[0] != PoseParameterType::POSE &&
                 inputTypes[0] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::VALUE;
        }
        case ORIENTATION_AXIS_X:
        case ORIENTATION_AXIS_Y:
        case ORIENTATION_AXIS_Z: {
            if (inputTypes.size() != 1 ||
                (inputTypes[0] != PoseParameterType::ORIENTATION && inputTypes[0] != PoseParameterType::POSE &&
                 inputTypes[0] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::TRANSLATION;
        }
        case DELTA_TRANSLATION: {
            if (inputTypes.size() != 2 ||
                (inputTypes[0] != PoseParameterType::TRANSLATION && inputTypes[0] != PoseParameterType::POSE &&
                 inputTypes[0] != PoseParameterType::EXTERNAL_POSE) ||
                (inputTypes[1] != PoseParameterType::TRANSLATION && inputTypes[1] != PoseParameterType::POSE &&
                 inputTypes[1] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::TRANSLATION;
        }
        case DELTA_ORIENTATION: {
            if (inputTypes.size() != 2 ||
                (inputTypes[0] != PoseParameterType::ORIENTATION && inputTypes[0] != PoseParameterType::POSE &&
                 inputTypes[0] != PoseParameterType::EXTERNAL_POSE) ||
                (inputTypes[1] != PoseParameterType::ORIENTATION && inputTypes[1] != PoseParameterType::POSE &&
                 inputTypes[1] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::ORIENTATION;
        }
        case DELTA_POSE: {
            if (inputTypes.size() != 2 ||
                (inputTypes[0] != PoseParameterType::POSE && inputTypes[0] != PoseParameterType::EXTERNAL_POSE) ||
                (inputTypes[1] != PoseParameterType::POSE && inputTypes[1] != PoseParameterType::EXTERNAL_POSE)) {
                return PoseParameterType::EMPTY;
            }
            return PoseParameterType::POSE;
        }
        default: {
            throw std::runtime_error("Can not apply operation: " + std::to_string(operation));
        }
    }
    return PoseParameterType::EMPTY;
}
