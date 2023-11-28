//
// Created by Andrei on 16.10.22.
//

#include <AndreiUtils/utilsBinarySerialization.hpp>

using namespace AndreiUtils;

void AndreiUtils::serialize(std::ofstream &out, std::string const &data) {
    serialize(out, data.c_str(), data.size());
    char nullEnding = 0;
    out.write((char *) &nullEnding, sizeof(char));
}

void AndreiUtils::deserialize(std::ifstream &in, std::string &data) {
    std::string localData;
    char datum;
    while (true) {
        deserialize(in, datum);
        if (datum == 0) {
            break;
        }
        localData += datum;
    }
    data = localData;
}
