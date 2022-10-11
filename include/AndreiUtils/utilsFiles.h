//
// Created by Andrei on 26.08.21.
//

#ifndef ANDREIUTILS_UTILSFILES_H
#define ANDREIUTILS_UTILSFILES_H

#include <string>
#include <unistd.h>
#include <vector>

namespace AndreiUtils {
    std::string getCurrentDirectory(bool withTrailingSeparator = false);

    bool fileExists(std::string const &name);

    bool createDirectory(std::string const &path);

    bool createNestedDirectory(std::string const &path, bool fromFilePath = false);

    bool deleteFile(std::string const &fileName);

    std::vector<std::string> listDirectoryFiles(std::string const &directoryName,
                                                std::string const &extensionFilter = "");

    bool isFilePathAbsolute(std::string const &path);
}

#endif //ANDREIUTILS_UTILSFILES_H
