//
// Created by andrei on 26.08.21.
//

#include <utilsFiles.h>
#include <iostream>
#include <sys/stat.h>
#include <utilsString.h>

using namespace std;

bool fileExists(const string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

bool createDirectory(const string &path) {
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

bool createDirectories(const string &path) {
    string dirPath = replace(path, "\\", "/"), tmpPath;
    int subdirs = stringCount(dirPath, "/");
    bool res;
    for (int i = 0; i < subdirs; i++) {
        tmpPath = firstParts(dirPath, "/", i + 1);
        res = createDirectory(tmpPath);
    }
    return res;
}

bool deleteFile(const string &fileName) {
    return remove(fileName.c_str()) == 0;
}
