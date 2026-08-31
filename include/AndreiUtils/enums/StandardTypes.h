/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        TYPE_BOOL = 14,
    };

    int getStandardTypeByteAmount(StandardTypes type);

    int getStandardTypeBitAmount(StandardTypes type);

    int convertStandardTypesToOpenCVType(StandardTypes type);

    StandardTypes convertOpenCVTypeToStandardTypes(int type);
} // namespace AndreiUtils

#endif // ANDREIUTILS_STANDARDTYPES_H
