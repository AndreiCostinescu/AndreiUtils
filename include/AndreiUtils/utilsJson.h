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
// Created by Andrei on 13.08.2021.
//

#pragma once

#include <AndreiUtils/json.hpp>
#include <sstream>
#include <string>

namespace AndreiUtils {
    nlohmann::json setJsonFromString(std::string const &jsonContent);

    nlohmann::json readJsonFile(std::string const &path);

    void writeJsonFileKeepOrder(std::string const &path, nlohmann::json const &content, bool keepNewLines = true,
                                std::string const &originalContentFilePath = "");

    std::string collectStringJsonContent(nlohmann::json const &content, int currentIndentLevel = 0);

    void collectStringJsonContent(std::stringstream &stringContent, nlohmann::json const &content);

    void writeJsonFile(std::string const &path, nlohmann::json const &content, std::string const &originalFilePath = "",
                       bool keepEmptyLines = false);
} // namespace AndreiUtils
