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
// Created by Andrei on 16.10.22.
//

#include <AndreiUtils/utilsBinarySerialization.hpp>

using namespace AndreiUtils;

void AndreiUtils::serialize(std::ofstream &out, std::string const &data) {
    serialize(out, data.c_str(), data.size());
    char nullEnding = 0;
    out.write((char *) &nullEnding, sizeof(char));
}

void AndreiUtils::deserializeIn(std::ifstream &in, std::string &data) {
    std::string localData;
    char datum;
    while (true) {
        deserializeIn(in, datum);
        if (datum == 0) {
            break;
        }
        localData += datum;
    }
    data = localData;
}
