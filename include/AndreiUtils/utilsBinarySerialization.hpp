//
// Created by Andrei on 16.10.22.
//

#ifndef ANDREIUTILS_UTILSBINARYSERIALIZATION_HPP
#define ANDREIUTILS_UTILSBINARYSERIALIZATION_HPP

#include <AndreiUtils/utilsFiles.h>
#include <fstream>
#include <stdexcept>

namespace AndreiUtils {
    template<typename T>
    void serialize(std::ofstream &out, T const &data) {
        out.write((char *) &data, sizeof(T));
    }

    template<typename T>
    void serialize(std::ofstream &out, T const *data, size_t nrElements) {
        out.write((char *) &data, sizeof(T) * nrElements);
    }

    template<typename T>
    void deserialize(std::ifstream &in, T &data) {
        if (reachedTheEndOfTheFile(in)) {
            throw std::runtime_error("There is no more data to be deserialized!");
        }
        in.read((char *) &data, sizeof(T));
        if (in.fail()) {
            throw std::runtime_error("Deserializing binary data failed!");
        }
    }

    template<typename T>
    T deserialize(std::ifstream &in) {
        T data;
        deserialize(in, data);
        return data;
    }

    template<typename T>
    void deserialize(std::ifstream &in, T *data, size_t nrElements) {
        for (size_t i = 0; i < nrElements; i++) {
            deserialize(in, data[i]);
        }
    }

    template<typename T>
    T *deserialize(std::ifstream &in, size_t nrElements) {
        T *data = new T[nrElements];
        try {
            deserialize(in, data, nrElements);
        } catch (std::runtime_error &e) {
            delete[] data;
            throw e;
        }
        return data;
    }
}

#endif //ANDREIUTILS_UTILSBINARYSERIALIZATION_HPP
