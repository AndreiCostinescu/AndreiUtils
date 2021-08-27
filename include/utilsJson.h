//
// Created by Andrei on 13.08.2021.
//

#ifndef ANDREIUTILS_JSON_H
#define ANDREIUTILS_JSON_H

#include <string>
#include <json/json.hpp>

nlohmann::json readJsonFile(const std::string &path);

void writeJsonFile(const std::string &path, const nlohmann::json &content);

#endif //ANDREIUTILS_JSON_H
