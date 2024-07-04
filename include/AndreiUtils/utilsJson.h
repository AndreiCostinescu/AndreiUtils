//
// Created by Andrei on 13.08.2021.
//

#pragma once

#include <AndreiUtils/json.hpp>
#include <string>
#include <sstream>

namespace AndreiUtils {
    nlohmann::json setJsonFromString(std::string const &jsonContent);

    nlohmann::json readJsonFile(std::string const &path);

    void writeJsonFileKeepOrder(std::string const &path, nlohmann::json const &content, bool keepNewLines = true,
                                std::string const &originalContentFilePath = "");

    std::string collectStringJsonContent(nlohmann::json const &content, int currentIndentLevel = 0);

    void collectStringJsonContent(std::stringstream &stringContent, nlohmann::json const &content);

    void writeJsonFile(std::string const &path, nlohmann::json const &content, std::string const &originalFilePath = "",
                       bool keepNewLines = false);
}
