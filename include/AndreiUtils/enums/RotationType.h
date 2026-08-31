// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 19.02.21.
//

#ifndef ANDREIUTILS_ENUMS_ROTATIONTYPE_H
#define ANDREIUTILS_ENUMS_ROTATIONTYPE_H

namespace AndreiUtils {
    enum RotationType {
        NO_ROTATION = 0,
        LEFT_90 = 1,
        RIGHT_270 = 1,
        LEFT_180 = 2,
        RIGHT_180 = 2,
        LEFT_270 = 3,
        RIGHT_90 = 3,
    };
}

#endif // ANDREIUTILS_ENUMS_ROTATIONTYPE_H
