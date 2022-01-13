//
// Created by Andrei on 26.08.21.
//

#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsString.h>
#include <climits>
#include <iostream>
#include <sys/stat.h>

using namespace std;

string AndreiUtils::getCurrentDirectory(bool withTrainingSeparator) {
    char buff[PATH_MAX];
    #if defined(WIN32) || defined(WIN64)
    _getcwd(buff, PATH_MAX);
    #else
    getcwd(buff, PATH_MAX);
    #endif
    string dir(buff);
    if (withTrainingSeparator) {
        #if defined(WIN32) || defined(WIN64)
        dir += "\\";
        #else
        dir += "/";
        #endif
    }
    return dir;
}

bool AndreiUtils::fileExists(const string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

bool AndreiUtils::createDirectory(const string &path) {
    bool res;
#if defined(WIN32) || defined(WIN64)
    res = (mkdir(path.c_str()) != -1);
#else
    res = (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != -1);
#endif
    if (!res) {
        cout << "Directory already exists? " << path << endl;
        return false;
    }
    return true;
}

bool AndreiUtils::createDirectories(const string &path) {
    string dirPath = replace(path, "\\", "/"), tmpPath;
    size_t subdirs = stringCount(dirPath, "/");
    bool res;
    for (int i = 0; i < subdirs; i++) {
        tmpPath = firstParts(dirPath, "/", i + 1);
        res = createDirectory(tmpPath);
    }
    return res;
}

bool AndreiUtils::deleteFile(const string &fileName) {
    return remove(fileName.c_str()) == 0;
}
