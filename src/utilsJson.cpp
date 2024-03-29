//
// Created by Andrei on 13.08.2021.
//

#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsFiles.h>
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

json AndreiUtils::readJsonFile(const string &path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    json content;
    fin >> content;
    return content;
}

void AndreiUtils::writeJsonFile(const string &path, const json &content) {
    createNestedDirectory(path, true);
    ofstream fout(path);
    if (!fout.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    fout << content.dump(4) << endl;
    fout.close();
}