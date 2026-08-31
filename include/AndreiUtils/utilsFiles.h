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
// Created by Andrei on 26.08.21.
//

#pragma once

#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

namespace AndreiUtils {
    std::string getCurrentDirectory(bool withTrailingSeparator = false);

    bool fileExists(std::string const &name);

    bool createDirectory(std::string const &path, bool verbose = false);

    bool createNestedDirectory(std::string const &path, bool fromFilePath = false);

    bool deleteFile(std::string const &fileName);

    std::vector<std::string> listDirectoryFiles(std::string const &directoryName,
                                                std::string const &extensionFilter = "");

    bool isFilePathAbsolute(std::string const &path);

    std::string getRelativeDirectoryOfPath(std::string const &path);

    std::string simplifyRelativePath(std::string const &path);

    bool reachedTheEndOfTheFile(std::ifstream &in);

    size_t getFileSize(std::ifstream &in);

    [[nodiscard]] std::string joinToPath(std::vector<std::string> const &pathSegments);
} // namespace AndreiUtils
