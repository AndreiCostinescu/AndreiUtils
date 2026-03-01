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

    [[nodiscard]] std::string joinAndInterpretAsFilePath(std::vector<std::string> const &paths);
}
