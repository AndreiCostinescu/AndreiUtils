//
// Created by Andrei on 26.08.21.
//

#ifndef ANDREIUTILS_UTILSFILES_H
#define ANDREIUTILS_UTILSFILES_H

#include <string>
#include <unistd.h>

namespace AndreiUtils {
    bool fileExists(const std::string &name);

    bool createDirectory(const std::string &path);

    bool createDirectories(const std::string &path);

    bool deleteFile(const std::string &fileName);
}

#endif //ANDREIUTILS_UTILSFILES_H
