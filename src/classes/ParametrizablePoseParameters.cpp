//
// Created by Andrei on 08.12.23.
//

#include <AndreiUtils/classes/ParametrizablePoseParameters.hpp>

using namespace AndreiUtils;

std::string ParameterOperation::getCacheEntryString(OperationType const &operationType) {
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
        case ORIENTATION_AXIS_X_X:
            return "q_x_x";
        case ORIENTATION_AXIS_X_Y:
            return "q_x_y";
        case ORIENTATION_AXIS_X_Z:
            return "q_x_z";
        case ORIENTATION_AXIS_Y_X:
            return "q_y_x";
        case ORIENTATION_AXIS_Y_Y:
            return "q_y_y";
        case ORIENTATION_AXIS_Y_Z:
            return "q_y_z";
        case ORIENTATION_AXIS_Z_X:
            return "q_z_x";
        case ORIENTATION_AXIS_Z_Y:
            return "q_z_y";
        case ORIENTATION_AXIS_Z_Z:
            return "q_z_z";
        case DELTA_ORIENTATION_AXIS_X_X:
            return "delta_q_x_x";
        case DELTA_ORIENTATION_AXIS_X_Y:
            return "delta_q_x_y";
        case DELTA_ORIENTATION_AXIS_X_Z:
            return "delta_q_x_z";
        case DELTA_ORIENTATION_AXIS_Y_X:
            return "delta_q_y_x";
        case DELTA_ORIENTATION_AXIS_Y_Y:
            return "delta_q_y_y";
        case DELTA_ORIENTATION_AXIS_Y_Z:
            return "delta_q_y_z";
        case DELTA_ORIENTATION_AXIS_Z_X:
            return "delta_q_z_x";
        case DELTA_ORIENTATION_AXIS_Z_Y:
            return "delta_q_z_y";
        case DELTA_ORIENTATION_AXIS_Z_Z:
            return "delta_q_z_z";
        default: {
            throw std::runtime_error("Unknown operation type to cache: " + std::to_string(operationType));
        }
    }
}

ParameterOperation::OperationType ParameterOperation::operationReduction(
        ParameterOperation::OperationType const &operationType) {
    switch (operationType) {
        case ORIENTATION_AXIS_X_X:
        case ORIENTATION_AXIS_Y_X:
        case ORIENTATION_AXIS_Z_X:
            return TRANSLATION_X;
        case ORIENTATION_AXIS_X_Y:
        case ORIENTATION_AXIS_Y_Y:
        case ORIENTATION_AXIS_Z_Y:
            return TRANSLATION_Y;
        case ORIENTATION_AXIS_X_Z:
        case ORIENTATION_AXIS_Y_Z:
        case ORIENTATION_AXIS_Z_Z:
            return TRANSLATION_Z;
        case DELTA_ORIENTATION_AXIS_X_X:
            return ORIENTATION_AXIS_X_X;
        case DELTA_ORIENTATION_AXIS_X_Y:
            return ORIENTATION_AXIS_X_Y;
        case DELTA_ORIENTATION_AXIS_X_Z:
            return ORIENTATION_AXIS_X_Z;
        case DELTA_ORIENTATION_AXIS_Y_X:
            return ORIENTATION_AXIS_Y_X;
        case DELTA_ORIENTATION_AXIS_Y_Y:
            return ORIENTATION_AXIS_Y_Y;
        case DELTA_ORIENTATION_AXIS_Y_Z:
            return ORIENTATION_AXIS_Y_Z;
        case DELTA_ORIENTATION_AXIS_Z_X:
            return ORIENTATION_AXIS_Z_X;
        case DELTA_ORIENTATION_AXIS_Z_Y:
            return ORIENTATION_AXIS_Z_Y;
        case DELTA_ORIENTATION_AXIS_Z_Z:
            return ORIENTATION_AXIS_Z_Z;
        default: {
            throw std::runtime_error(
                    "Operation " + ParameterOperation::getCacheEntryString(operationType) + " is not reducible!");
        }
    }
}

ParameterOperation::ParameterOperation(nlohmann::json const &operationConfig) {
    assert(operationConfig.contains("op") &&
           (operationConfig.contains("parameter") || operationConfig.contains("parameters")));
    if (operationConfig.contains("parameter")) {
        this->parameterIndices.emplace_back(operationConfig.at("parameter").get<int>());
    } else {
        assert(operationConfig.contains("parameters"));
        this->parameterIndices = operationConfig.at("parameters").get<std::vector<int>>();
    }
    std::string stringOp = operationConfig.at("op").get<std::string>();
    if (stringOp == "tx") {
        this->operation = TRANSLATION_X;
    } else if (stringOp == "ty") {
        this->operation = TRANSLATION_Y;
    } else if (stringOp == "tz") {
        this->operation = TRANSLATION_Z;
    } else if (stringOp == "tNorm") {
        this->operation = TRANSLATION_NORM;
    } else if (stringOp == "qw") {
        this->operation = ORIENTATION_W;
    } else if (stringOp == "qx") {
        this->operation = ORIENTATION_X;
    } else if (stringOp == "qy") {
        this->operation = ORIENTATION_Y;
    } else if (stringOp == "qz") {
        this->operation = ORIENTATION_Z;
    } else if (stringOp == "qNorm") {
        this->operation = ORIENTATION_NORM;
    } else if (stringOp == "qAngle") {
        this->operation = ORIENTATION_ANGLE;
    } else if (stringOp == "q_x_x") {
        this->operation = ORIENTATION_AXIS_X_X;
    } else if (stringOp == "q_x_y") {
        this->operation = ORIENTATION_AXIS_X_Y;
    } else if (stringOp == "q_x_z") {
        this->operation = ORIENTATION_AXIS_X_Z;
    } else if (stringOp == "q_y_x") {
        this->operation = ORIENTATION_AXIS_Y_X;
    } else if (stringOp == "q_y_y") {
        this->operation = ORIENTATION_AXIS_Y_Y;
    } else if (stringOp == "q_y_z") {
        this->operation = ORIENTATION_AXIS_Y_Z;
    } else if (stringOp == "q_z_x") {
        this->operation = ORIENTATION_AXIS_Z_X;
    } else if (stringOp == "q_z_y") {
        this->operation = ORIENTATION_AXIS_Z_Y;
    } else if (stringOp == "q_z_z") {
        this->operation = ORIENTATION_AXIS_Z_Z;
    } else if (stringOp == "delta_q_x_x") {
        this->operation = DELTA_ORIENTATION_AXIS_X_X;
    } else if (stringOp == "delta_q_x_y") {
        this->operation = DELTA_ORIENTATION_AXIS_X_Y;
    } else if (stringOp == "delta_q_x_z") {
        this->operation = DELTA_ORIENTATION_AXIS_X_Z;
    } else if (stringOp == "delta_q_y_x") {
        this->operation = DELTA_ORIENTATION_AXIS_Y_X;
    } else if (stringOp == "delta_q_y_y") {
        this->operation = DELTA_ORIENTATION_AXIS_Y_Y;
    } else if (stringOp == "delta_q_y_z") {
        this->operation = DELTA_ORIENTATION_AXIS_Y_Z;
    } else if (stringOp == "delta_q_z_x") {
        this->operation = DELTA_ORIENTATION_AXIS_Z_X;
    } else if (stringOp == "delta_q_z_y") {
        this->operation = DELTA_ORIENTATION_AXIS_Z_Y;
    } else if (stringOp == "delta_q_z_z") {
        this->operation = DELTA_ORIENTATION_AXIS_Z_Z;
    } else {
        throw std::runtime_error("Unknown pose operation to apply on parameter: " + stringOp);
    }
}
