//
// Created by Andrei on 16.10.22.
//

#pragma once

#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsMap.hpp>
#include <fstream>
#include <stdexcept>

namespace AndreiUtils {
    // DEFINITIONS

    template<typename T>
    void serialize(std::ofstream &out, T const &data);

    template<typename T>
    void serialize(std::ofstream &out, T const *data, size_t nrElements);

    template<typename T>
    void deserializeIn(std::ifstream &in, T &data);

    template<typename T>
    T deserializeReturn(std::ifstream &in);

    template<typename T>
    void deserializeIn(std::ifstream &in, T *data, size_t nrElements);

    // upon success, make sure you delete the returned data allocated with new[]
    template<typename T>
    T *deserializeReturn(std::ifstream &in, size_t nrElements);

    // DEFINITION SPECIALIZATIONS

    void serialize(std::ofstream &out, std::string const &data);

    template<typename T>
    void serialize(std::ofstream &out, std::vector<T> const &data);

    template<typename T1, typename T2>
    void serialize(std::ofstream &out, std::pair<T1, T2> const &data);

    template<typename T1, typename T2, typename C, typename A>
    void serialize(std::ofstream &out, std::map<T1, T2, C, A> const &data);

    void deserializeIn(std::ifstream &in, std::string &data);

    template<typename T>
    void deserializeIn(std::ifstream &in, std::vector<T> &data);

    template<typename T1, typename T2>
    void deserializeIn(std::ifstream &in, std::pair<T1, T2> &data);

    template<typename T1, typename T2, typename C, typename A>
    void deserializeIn(std::ifstream &in, std::map<T1, T2, C, A> &data);

    // IMPLEMENTATIONS

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
    [[nodiscard]] bool skip(std::ifstream &in, int count = 1) {
        long posBeforeSkip = in.tellg();
        if (reachedTheEndOfTheFile(in)) {
            in.seekg(posBeforeSkip);
            return false;
        }
        in.seekg(count * sizeof(T), std::ios::cur);
        if (in.tellg() > getFileSize(in)) {
            in.seekg(posBeforeSkip);
            return false;
        }
        return true;
    }

    template<typename T>
    void deserializeIn(std::ifstream &in, T &data) {
        if (reachedTheEndOfTheFile(in)) {
            throw std::runtime_error("There is no more data to be deserialized!");
        }
        in.read((char *) &data, sizeof(T));
        if (in.fail()) {
            throw std::runtime_error("Deserializing binary data failed!");
        }
    }

    template<typename T>
    T deserializeReturn(std::ifstream &in) {
        T data;
        deserializeIn(in, data);
        return data;
    }

    template<typename T>
    void deserializeIn(std::ifstream &in, T *data, size_t nrElements) {
        for (size_t i = 0; i < nrElements; ++i) {
            // this (with the right specialization) can be used to solve the networkToHost int/long byte problem
            deserializeIn(in, data[i]);
        }
    }

    // upon success, make sure you delete the returned data allocated with new[]
    template<typename T>
    T *deserializeReturn(std::ifstream &in, size_t nrElements) {
        T *data = new T[nrElements];
        try {
            deserializeIn(in, data, nrElements);
        } catch (std::runtime_error &e) {
            delete[] data;
            throw e;
        }
        return data;
    }

    // IMPLEMENTATION SPECIALIZATIONS

    template<typename T>
    void serialize(std::ofstream &out, std::vector<T> const &data) {
        serialize(out, (int) data.size());
        serialize(out, data.data(), data.size());
    }

    template<typename T1, typename T2>
    void serialize(std::ofstream &out, std::pair<T1, T2> const &data) {
        serialize(out, data.first);
        serialize(out, data.second);
    }

    template<typename T1, typename T2, typename C, typename A>
    void serialize(std::ofstream &out, std::map<T1, T2, C, A> const &data) {
        serialize(out, (int) data.size());
        for (auto const &datum: data) {
            serialize(out, datum.first);
            serialize(out, datum.second);
        }
    }

    template<typename T>
    void deserializeIn(std::ifstream &in, std::vector<T> &data) {
        data.clear();
        int nrElements = deserializeReturn<int>(in);
        for (int i = 0; i < nrElements; ++i) {
            data.emplace_back(deserializeReturn<T>(in));
        }
    }

    template<typename T1, typename T2>
    void deserializeIn(std::ifstream &in, std::pair<T1, T2> &data) {
        deserializeIn(in, data.first);
        deserializeIn(in, data.second);
    }

    template<typename T1, typename T2, typename C, typename A>
    void deserializeIn(std::ifstream &in, std::map<T1, T2, C, A> &data) {
        data.clear();
        int nrElements = deserializeReturn<int>(in);
        for (int i = 0; i < nrElements; ++i) {
            T1 key;
            T2 value;
            deserializeIn(in, key);
            deserializeIn(in, value);
            mapEmplace(data, key, value);
        }
    }
}
