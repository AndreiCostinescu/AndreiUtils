//
// Created by Andrei on 13.08.2021.
//

#pragma once

#include <AndreiUtils/json.hpp>
#include <string>

namespace AndreiUtils {
    nlohmann::json readJsonFile(std::string const &path);

    void writeJsonFile(std::string const &path, const nlohmann::json &content);
}
