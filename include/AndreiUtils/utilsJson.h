//
// Created by Andrei on 13.08.2021.
//

#ifndef ANDREIUTILS_JSON_H
#define ANDREIUTILS_JSON_H

#include <AndreiUtils/json.hpp>
#include <string>
#include <complex>

namespace AndreiUtils {
    nlohmann::json readJsonFile(const std::string &path);

    void writeJsonFile(const std::string &path, const nlohmann::json &content);
}

namespace nlohmann {
    template<class T>
    struct adl_serializer<std::complex<T>> {
        static void to_json(nlohmann::json &j, const std::complex<T> &v) {
            j["r"] = v.real();
            j["i"] = v.imag();
        }

        static void from_json(const nlohmann::json &j, std::complex<T> &v) {
            v.real(j["r"].get<T>());
            v.imag(j["i"].get<T>());
        }
    };
}

#endif //ANDREIUTILS_JSON_H
