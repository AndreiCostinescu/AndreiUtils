//
// Created by Andrei on 13.08.2021.
//

#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsJson.hpp>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsString.h>
#include <iostream>

using namespace AndreiUtils;
using namespace nlohmann;
using namespace std;

json AndreiUtils::readJsonFile(string const &path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    json content;
    fin >> content;
    return content;
}

bool skipWhiteSpaces(vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex) {
    while (true) {
        if (lineIndex >= lineByLineContent.size()) {
            // throw std::runtime_error("Reached the end of the file!");
            return false;
        }
        if (characterIndex >= lineByLineContent[lineIndex].size() ||
            lineByLineContent[lineIndex][characterIndex] == '\n') {
            ++lineIndex;
            characterIndex = 0;
            continue;
        }
        if (lineByLineContent[lineIndex][characterIndex] != ' ') {
            break;
        }
        ++characterIndex;
    }
    return true;
}

bool skipToNextEntryInThisLevelOfJsonContent(  // NOLINT(misc-no-recursion)
        vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex,
        nlohmann::json *skippedData = nullptr) {
    if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex)) {
        return false;
    }
    char const &c = lineByLineContent[lineIndex][characterIndex];
    if (c == '[') {
        // array
        vector<nlohmann::json> data;
        ++characterIndex;
        while (true) {
            data.emplace_back();
            if (!skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex,
                                                         skippedData != nullptr ? &data.back() : nullptr)) {
                return false;
            }
            if (lineByLineContent[lineIndex][characterIndex] == ']') {
                break;
            }
            ++characterIndex;
        }
        ++characterIndex;
        if (skippedData != nullptr) {
            *skippedData = std::move(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == '{') {
        // object
        std::map<std::string, nlohmann::json> data;
        ++characterIndex;
        while (true) {
            std::string key;
            nlohmann::json keyData;
            if (!skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex, &keyData)) {
                return false;
            }
            assert(keyData.is_string());
            key = keyData.get<string>();
            assert(lineByLineContent[lineIndex][characterIndex] == ':');
            ++characterIndex;
            if (!skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex,
                                                         skippedData != nullptr ? &(mapEmplace(data, key)->second)
                                                                                : nullptr)) {
                return false;
            }
            if (lineByLineContent[lineIndex][characterIndex] == '}') {
                break;
            }
            ++characterIndex;
        }
        ++characterIndex;
        if (skippedData != nullptr) {
            *skippedData = std::move(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == '\"') {
        // string
        string data;
        ++characterIndex;
        while (true) {
            if (lineByLineContent[lineIndex][characterIndex] == '\"' &&
                lineByLineContent[lineIndex][characterIndex - 1] != '\\') {
                break;
            }
            if (skippedData != nullptr) {
                data += lineByLineContent[lineIndex][characterIndex];
            }
            ++characterIndex;
        }
        ++characterIndex;
        if (skippedData != nullptr) {
            *skippedData = std::move(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        // number
        string data;
        ++characterIndex;
        while (true) {
            char const &chr = lineByLineContent[lineIndex][characterIndex];
            if (!(chr == '.' || (chr >= '0' && chr <= '9'))) {
                break;
            }
            if (skippedData != nullptr) {
                data += lineByLineContent[lineIndex][characterIndex];
            }
            ++characterIndex;
        }
        if (skippedData != nullptr) {
            *skippedData = stod(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == 't' &&
               lineByLineContent[lineIndex][characterIndex + 1] == 'r' &&
               lineByLineContent[lineIndex][characterIndex + 2] == 'u' &&
               lineByLineContent[lineIndex][characterIndex + 3] == 'e') {
        characterIndex += 4;
        if (skippedData != nullptr) {
            *skippedData = true;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == 'f' &&
               lineByLineContent[lineIndex][characterIndex + 1] == 'a' &&
               lineByLineContent[lineIndex][characterIndex + 2] == 'l' &&
               lineByLineContent[lineIndex][characterIndex + 3] == 's' &&
               lineByLineContent[lineIndex][characterIndex + 4] == 'e') {
        characterIndex += 5;
        if (skippedData != nullptr) {
            *skippedData = false;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    } else if (c == 'n' &&
               lineByLineContent[lineIndex][characterIndex + 1] == 'u' &&
               lineByLineContent[lineIndex][characterIndex + 2] == 'l' &&
               lineByLineContent[lineIndex][characterIndex + 3] == 'l') {
        characterIndex += 4;
        if (skippedData != nullptr) {
            *skippedData = nullptr;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
        return true;
    }
    cout << "Unknown character to process: " << c << endl;
    return false;
}

bool collectStringJsonContentKeepOrder(  // NOLINT(misc-no-recursion)
        stringstream &stringContent, json const &content, json const &originalContent,
        vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex, int const &indentLevel = 0) {
    if (content.type() != originalContent.type()) {
        stringContent << collectStringJsonContent(content, indentLevel);
        return skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex);
    }

    // first determine the type of the data at the content
    if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex)) {
        return false;
    }
    char const &c = lineByLineContent[lineIndex][characterIndex];
    if (c == '[') {
        // array
        assert(originalContent.is_array());

        vector<nlohmann::json> contentArray = content.get<vector<nlohmann::json>>();
        vector<nlohmann::json> originalContentArray = originalContent.get<vector<nlohmann::json>>();
        if (contentArray.size() != originalContentArray.size()) {
            stringContent << collectStringJsonContent(content, indentLevel);
            return skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex);
        }

        ++characterIndex;  // move past '[' character
        if (contentArray.empty()) {
            if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex)) {
                return false;
            }
            assert(lineByLineContent[lineIndex][characterIndex] == ']');
            stringContent << "[]";
            skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
            return true;
        }

        stringContent << "[" << endl;
        for (size_t index = 0; index < contentArray.size(); ++index) {
            if (index > 0) {
                stringContent << ",";
            }
            if (!collectStringJsonContentKeepOrder(stringContent, contentArray[index], originalContentArray[index],
                                                   lineByLineContent, lineIndex, characterIndex, indentLevel + 1)) {
                return false;
            }
        }
        assert(lineByLineContent[lineIndex][characterIndex] == ']');
        ++characterIndex;
        stringContent << AndreiUtils::tab * indentLevel << "]";
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
    } else if (c == '{') {
        // object
        assert(originalContent.is_object());
        map<string, nlohmann::json> contentMap = content.get<map<string, nlohmann::json>>();
        map<string, nlohmann::json> originalContentMap = originalContent.get<map<string, nlohmann::json>>();

        // keep the entries that are common; and write the extra entries that are new!
        stringstream commonEntries;
        stringstream newEntries;
        bool firstNewEntry = true, firstCommonEntry = true;

        std::map<std::string, bool> definedKeys;
        ++characterIndex;
        while (true) {
            nlohmann::json keyDatum;
            skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex, &keyDatum);
            assert(keyDatum.is_string());
            assert(lineByLineContent[lineIndex][characterIndex] == ':');
            ++characterIndex;  // skips the colon ':' character
            string key = keyDatum.get<string>();
            definedKeys[key] = true;

            nlohmann::json const *commonDatum;
            if (mapGetIfContains(contentMap, key, commonDatum)) {
                cout << "Add common entry " << key << endl;
                if (!firstCommonEntry) {
                    commonEntries << "," << endl << AndreiUtils::tab * (indentLevel + 1);
                }
                commonEntries << "\"" << key << "\": ";
                nlohmann::json const &originalCommonDatum = mapGet(originalContentMap, key);
                if (!collectStringJsonContentKeepOrder(commonEntries, *commonDatum, originalCommonDatum,
                                                       lineByLineContent, lineIndex, characterIndex, indentLevel + 1)) {
                    return false;
                }
                firstCommonEntry = false;
            } else {
                cout << "Key " << key << " is not contained in the new data!" << endl;
            }

            if (lineByLineContent[lineIndex][characterIndex] == '}') {
                break;
            }
            ++characterIndex;
        }
        ++characterIndex;
        for (auto const &mapData: contentMap) {
            if (!mapContains(definedKeys, mapData.first)) {
                cout << "Add new entry " << mapData.first << endl;
                if (!firstNewEntry) {
                    newEntries << "," << endl << AndreiUtils::tab * (indentLevel + 1);
                }
                newEntries << "\"" << mapData.first << "\": ";
                newEntries << collectStringJsonContent(mapData.second, indentLevel + 1);
                firstNewEntry = false;
            }
        }

        string commonString = commonEntries.str(), newString = newEntries.str();
        if (!commonString.empty() && !newString.empty()) {
            commonString += ",\n" + AndreiUtils::tab * (indentLevel + 1);
        }
        stringContent << "{" << endl;
        stringContent << AndreiUtils::tab * (indentLevel + 1) << commonString << newEntries.str() << endl;
        stringContent << AndreiUtils::tab * indentLevel << "}";
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex);
    } else {
        stringContent << collectStringJsonContent(content, indentLevel);
        skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex);
    }
    return true;
    throw std::runtime_error("Unexpected path through code!");
}

void AndreiUtils::writeJsonFileKeepOrder(string const &path, json const &content) {  // NOLINT(misc-no-recursion)
    if (!fileExists(path)) {
        writeJsonFile(path, content, false);
    }
    json originalJsonContent(std::move(readJsonFile(path)));
    if (originalJsonContent.type() != content.type() || !originalJsonContent.is_object()) {
        writeJsonFile(path, content, false);
    }

    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    std::string line;
    vector<string> lineByLineContent;
    while (getline(fin, line)) {
        lineByLineContent.emplace_back(line);
    }

    stringstream stringContent;
    int lineIndex = 0, characterIndex = 0;
    if (!collectStringJsonContentKeepOrder(stringContent, content, originalJsonContent, lineByLineContent, lineIndex,
                                           characterIndex)) {
        throw std::runtime_error("Parsing of the json data succeeded but the line-by-line parsing did not!");
    }

    ofstream fout(path);
    if (!fout.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    fout << stringContent.str() << endl;
    fout.close();
}

std::string AndreiUtils::collectStringJsonContent(nlohmann::json const &content, int currentIndentLevel) {
    return content.dumpWithExistingIndent(4, ' ', 4 * currentIndentLevel);
}

void AndreiUtils::collectStringJsonContent(std::stringstream &stringContent, nlohmann::json const &content) {
    stringContent << collectStringJsonContent(content);
}

void AndreiUtils::writeJsonFile(string const &path, json const &content, bool keepOrder) {  // NOLINT(misc-no-recursion)
    if (keepOrder) {
        writeJsonFileKeepOrder(path, content);
        return;
    }
    createNestedDirectory(path, true);
    ofstream fout(path);
    if (!fout.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    fout << collectStringJsonContent(content) << endl;
    fout.close();
}

void adl_serializer<AndreiUtils::ImageParameters>::to_json(json &j, AndreiUtils::ImageParameters const &data) {
    j.clear();
    j["h"] = data.h;
    j["w"] = data.w;
}

void adl_serializer<AndreiUtils::ImageParameters>::from_json(json const &j, AndreiUtils::ImageParameters &data) {
    data.h = j.at("h").get<int>();
    data.w = j.at("w").get<int>();
}

void adl_serializer<AndreiUtils::CameraIntrinsicParameters>::to_json(json &j, CameraIntrinsicParameters const &data) {
    j.clear();
    j["fx"] = data.fx;
    j["fy"] = data.fy;
    j["ppx"] = data.ppx;
    j["ppy"] = data.ppy;
    j["distortionModel"] = AndreiUtils::convertImageDistortionModelToString(data.distortionModel);
    j["distortionCoefficients"] = data.getDistortionCoefficientsAsVector();
}

void adl_serializer<AndreiUtils::CameraIntrinsicParameters>::from_json(json const &j, CameraIntrinsicParameters &data) {
    data.setImageParameters(j.at("fx").get<double>(), j.at("fy").get<double>(), j.at("ppx").get<double>(),
                            j.at("ppy").get<double>());
    data.setDistortionParameters(
            AndreiUtils::convertStringToImageDistortionModel(j.at("distortionModel").get<std::string>()),
            j.at("distortionCoefficients").get<std::vector<float>>());
}

void adl_serializer<AndreiUtils::ImageCaptureParameters>::to_json(json &j, ImageCaptureParameters const &data) {
    j.clear();
    j["fps"] = data.fps;
    j["size"] = data.size;
}

void adl_serializer<AndreiUtils::ImageCaptureParameters>::from_json(json const &j, ImageCaptureParameters &data) {
    data.fps = j.at("fps").get<double>();
    data.size = j.at("size").get<AndreiUtils::ImageParameters>();
}

void adl_serializer<ImageCaptureParametersWithIntrinsics>::to_json(
        json &j, ImageCaptureParametersWithIntrinsics const &data) {
    j = (AndreiUtils::ImageCaptureParameters) data;
    j["intrinsics"] = data.intrinsics;
}

void adl_serializer<ImageCaptureParametersWithIntrinsics>::from_json(
        json const &j, ImageCaptureParametersWithIntrinsics &data) {
    data.ImageCaptureParameters::setFromOther(j.get<AndreiUtils::ImageCaptureParameters>());
    data.intrinsics = j.at("intrinsics").get<AndreiUtils::CameraIntrinsicParameters>();
}
