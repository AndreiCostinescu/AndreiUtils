//
// Created by Andrei on 16.10.22.
//

#pragma once

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
        for (size_t i = 0; i < nrElements; ++i) {
            // this (with the right specialization) can be used to solve the networkToHost int/long byte problem
            serialize(out, data[i]);
        }
        // out.write((char *) &data, sizeof(T) * nrElements);
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
        for (size_t i = 0; i < nrElements; ++i) {
            // this (with the right specialization) can be used to solve the networkToHost int/long byte problem
            deserialize(in, data[i]);
        }
    }

    // upon success, make sure you delete the returned data allocated with new[]
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
