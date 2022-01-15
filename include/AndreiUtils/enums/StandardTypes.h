//
// Created by Andrei on 20-Oct-21.
//

#ifndef ANDREIUTILS_STANDARDTYPES_H
#define ANDREIUTILS_STANDARDTYPES_H

namespace AndreiUtils {
    enum StandardTypes {
        TYPE_UINT_8 = 0,
        TYPE_INT_8 = 1,
        TYPE_UINT_16 = 2,
        TYPE_INT_16 = 3,
        TYPE_UINT_32 = 4,
        TYPE_INT_32 = 5,
        TYPE_UINT_64 = 6,
        TYPE_INT_64 = 7,
        TYPE_UINT_128 = 8,
        TYPE_INT_128 = 9,
        TYPE_FLOAT_16 = 10,
        TYPE_FLOAT_32 = 11,
        TYPE_FLOAT_64 = 12,
        TYPE_FLOAT_128 = 13,
    };

    int getStandardTypeByteAmount(StandardTypes type);

    int convertStandardTypesToOpenCVType(StandardTypes type);

    StandardTypes convertOpenCVTypeToStandardTypes(int type);
}

#endif //ANDREIUTILS_STANDARDTYPES_H
