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
// Created by Andrei on 11.11.20.
//

#pragma once

#include <string>

namespace AndreiUtils {
    bool stringIsInteger(std::string const &s);

    bool stringIsInteger(std::string const &s, int &res);

    int stringToInteger(std::string const &s);

    bool stringIsBool(std::string const &s);

    bool stringIsBool(std::string const &s, bool &res);

    bool stringToBool(std::string const &s);

    bool stringIsDouble(std::string const &s);

    bool stringIsDouble(std::string const &s, double &res);

    double stringToDouble(std::string const &s);

    bool stringIsFloat(std::string const &s);

    bool stringIsFloat(std::string const &s, float &res);

    float stringToFloat(std::string const &s);
} // namespace AndreiUtils
