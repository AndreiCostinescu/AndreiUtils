//
// Created by Andrei on 13.08.2021.
//

#include <andrei_utils/utilsJson.h>
#include <fstream>
#include <iostream>

using namespace nlohmann;
using namespace std;

json readJsonFile(const string &path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    json content;
    fin >> content;
    return content;
}

void writeJsonFile(const string &path, const json &content) {
    ofstream fout(path);
    if (!fout.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    fout << content.dump(4) << endl;
    fout.close();
}