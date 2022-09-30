//
// Created by Andrei on 26.08.21.
//

#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsString.h>
#include <iostream>
#include <sys/stat.h>

#if defined(_WIN32)

#define NOMINMAX
#include <windows.h>

#else

#include <dirent.h>

#endif

using namespace std;

string AndreiUtils::getCurrentDirectory(bool withTrailingSeparator) {
    char buff[PATH_MAX], *res;
    #if defined(WIN32) || defined(WIN64)
    res = _getcwd(buff, PATH_MAX);
    #else
    res = getcwd(buff, PATH_MAX);
    #endif
    if (res == nullptr) {
        throw std::runtime_error("Can not determine the current directory!");
    }
    string dir(buff);
    if (withTrailingSeparator) {
        #if defined(WIN32) || defined(WIN64)
        dir += "\\";
        #else
        dir += "/";
        #endif
    }
    return dir;
}

bool AndreiUtils::fileExists(string const &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

bool AndreiUtils::createDirectory(string const &path) {
    if (path.empty()) {
        cout << "Directory path is empty!" << endl;
        return false;
    }
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

bool AndreiUtils::createNestedDirectory(string const &path) {
    string dirPath = replace(path, "\\", "/"), tmpPath;
    size_t nrSubdirs = stringCount(dirPath, "/");
    bool res;
    for (int i = 0; i <= nrSubdirs; i++) {
        tmpPath = firstParts(dirPath, "/", i + 1);
        res = createDirectory(tmpPath);
    }
    return res;
}

bool AndreiUtils::deleteFile(string const &fileName) {
    return remove(fileName.c_str()) == 0;
}

void addFilenameIfExtensionMatches(vector<string> &fileNames, string const &fileName, string const &extension,
                                   string const &folderName) {
    if (!extension.empty() && (fileName.length() <= extension.length() ||
                               fileName.compare(fileName.length() - extension.length(), string::npos, extension) ||
                               (fileName[0] == '.'))) {
        return;
    }
    string res = folderName;
    res.append("/");
    res.append(fileName);
    fileNames.push_back(res);
}

vector<string> AndreiUtils::listDirectoryFiles(string const &directoryName, string const &extension) {
    vector<string> fileNames;

#if defined(_WIN32)

    HANDLE directoryHandle;
    WIN32_FIND_DATA directoryEntry;

    if ((directoryHandle = FindFirstFileA((directoryName + "/*").c_str(), &directoryEntry)) != INVALID_HANDLE_VALUE) {
        do {
            addFilenameIfExtensionMatches(fileNames, directoryEntry.cFileName, extension, directoryName);
        } while (FindNextFileA(directoryHandle, &directoryEntry));
        FindClose(directoryHandle);
    }

#else

    DIR *directoryHandle;
    if ((directoryHandle = opendir(directoryName.c_str())) != nullptr) {
        struct dirent *directoryEntry;
        while ((directoryEntry = readdir(directoryHandle)) != nullptr) {
            addFilenameIfExtensionMatches(fileNames, directoryEntry->d_name, extension, directoryName);
        }
        closedir(directoryHandle);
    }

#endif

    return fileNames;
}
