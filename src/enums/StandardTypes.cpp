//
// Created by Andrei on 20-Oct-21.
//

#include <AndreiUtils/enums/StandardTypes.h>
#include <stdexcept>
#include <string>

using namespace std;

int AndreiUtils::getStandardTypeByteAmount(StandardTypes type) {
    switch (type) {
        case TYPE_BOOL:
            return 0;
        case TYPE_UINT_8:
        case TYPE_INT_8:
            return 1;
        case TYPE_UINT_16:
        case TYPE_INT_16:
        case TYPE_FLOAT_16:
            return 2;
        case TYPE_UINT_32:
        case TYPE_INT_32:
        case TYPE_FLOAT_32:
            return 4;
        case TYPE_UINT_64:
        case TYPE_INT_64:
        case TYPE_FLOAT_64:
            return 8;
        case TYPE_UINT_128:
        case TYPE_INT_128:
        case TYPE_FLOAT_128:
            return 16;
        default: {
            throw runtime_error("Unknown Standard Type " + to_string(type));
        }
    }
}

int AndreiUtils::getStandardTypeBitAmount(StandardTypes type) {
    switch (type) {
        case TYPE_BOOL:
            return 1;
        case TYPE_UINT_8:
        case TYPE_INT_8:
            return 8;
        case TYPE_UINT_16:
        case TYPE_INT_16:
        case TYPE_FLOAT_16:
            return 16;
        case TYPE_UINT_32:
        case TYPE_INT_32:
        case TYPE_FLOAT_32:
            return 32;
        case TYPE_UINT_64:
        case TYPE_INT_64:
        case TYPE_FLOAT_64:
            return 64;
        case TYPE_UINT_128:
        case TYPE_INT_128:
        case TYPE_FLOAT_128:
            return 128;
        default: {
            throw runtime_error("Unknown Standard Type " + to_string(type));
        }
    }
}

int AndreiUtils::convertStandardTypesToOpenCVType(AndreiUtils::StandardTypes type) {
    if (type == StandardTypes::TYPE_UINT_8) {
        return 0;
    } else if (type == StandardTypes::TYPE_INT_8) {
        return 1;
    } else if (type == StandardTypes::TYPE_UINT_16) {
        return 2;
    } else if (type == StandardTypes::TYPE_INT_16) {
        return 3;
    } else if (type == StandardTypes::TYPE_INT_32) {
        return 4;
    } else if (type == StandardTypes::TYPE_FLOAT_32) {
        return 5;
    } else if (type == StandardTypes::TYPE_FLOAT_64) {
        return 6;
    } else if (type == StandardTypes::TYPE_FLOAT_16) {
        return 7;
    }
    throw runtime_error("Can not convert " + to_string(type) + " to OpenCV type...");
}

AndreiUtils::StandardTypes AndreiUtils::convertOpenCVTypeToStandardTypes(int type) {
    type = type & 7;
    if (type == 0) {
        return StandardTypes::TYPE_UINT_8;
    } else if (type == 1) {
        return StandardTypes::TYPE_INT_8;
    } else if (type == 2) {
        return StandardTypes::TYPE_UINT_16;
    } else if (type == 3) {
        return StandardTypes::TYPE_INT_16;
    } else if (type == 4) {
        return StandardTypes::TYPE_INT_32;
    } else if (type == 5) {
        return StandardTypes::TYPE_FLOAT_32;
    } else if (type == 6) {
        return StandardTypes::TYPE_FLOAT_64;
    } else if (type == 7) {
        return StandardTypes::TYPE_FLOAT_16;
    }
    throw runtime_error("Can not convert OpenCV type " + to_string(type) + " to StandardTypes...");
}
