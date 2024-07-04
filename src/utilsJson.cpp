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

json AndreiUtils::setJsonFromString(std::string const &jsonContent) {
    return json::parse(jsonContent);
}

json AndreiUtils::readJsonFile(string const &path) {
    ifstream fin(path);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + path);
    }
    json content;
    fin >> content;
    return content;
}

bool skipWhiteSpaces(vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex,
                     int *countNewLines = nullptr) {
    while (true) {
        if (lineIndex >= lineByLineContent.size()) {
            // throw std::runtime_error("Reached the end of the file!");
            return false;
        }
        if (characterIndex >= lineByLineContent[lineIndex].size() ||
            lineByLineContent[lineIndex][characterIndex] == '\n') {
            if (countNewLines != nullptr) {
                ++(*countNewLines);
            }
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
        vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex, int *emptyLinesCounter = nullptr,
        nlohmann::json *skippedData = nullptr) {
    if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter)) {
        return false;
    }
    char const &c = lineByLineContent[lineIndex][characterIndex];
    if (c == '[') {
        // array
        vector<nlohmann::json> data;
        ++characterIndex;
        if (lineByLineContent[lineIndex][characterIndex] != ']') {
            while (true) {
                data.emplace_back();
                if (!skipToNextEntryInThisLevelOfJsonContent(
                        lineByLineContent, lineIndex, characterIndex, emptyLinesCounter,
                        skippedData != nullptr ? &data.back() : nullptr)) {
                    return false;
                }
                if (lineByLineContent[lineIndex][characterIndex] == ']') {
                    break;
                }
                ++characterIndex;
            }
        }
        ++characterIndex;
        if (skippedData != nullptr) {
            *skippedData = std::move(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
        return true;
    } else if (c == '{') {
        // object
        std::map<std::string, nlohmann::json> data;
        ++characterIndex;
        if (lineByLineContent[lineIndex][characterIndex] != '}') {
            while (true) {
                std::string key;
                nlohmann::json keyData;
                if (!skipToNextEntryInThisLevelOfJsonContent(
                        lineByLineContent, lineIndex, characterIndex, emptyLinesCounter, &keyData)) {
                    return false;
                }
                assert(keyData.is_string());
                key = keyData.get<string>();
                assert(lineByLineContent[lineIndex][characterIndex] == ':');
                ++characterIndex;
                if (!skipToNextEntryInThisLevelOfJsonContent(
                        lineByLineContent, lineIndex, characterIndex, emptyLinesCounter,
                        skippedData != nullptr ? &(mapEmplace(data, key)->second) : nullptr)) {
                    return false;
                }
                if (lineByLineContent[lineIndex][characterIndex] == '}') {
                    break;
                }
                ++characterIndex;
            }
        }
        ++characterIndex;
        if (skippedData != nullptr) {
            *skippedData = std::move(data);
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
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
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
        return true;
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        // number
        string data;
        ++characterIndex;
        while (true) {
            char const &chr = lineByLineContent[lineIndex][characterIndex];
            if (!(chr == '.' || (chr >= '0' && chr <= '9') || chr == 'e' || chr == '-' || chr == '+')) {
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
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
        return true;
    } else if (c == 't' &&
               lineByLineContent[lineIndex][characterIndex + 1] == 'r' &&
               lineByLineContent[lineIndex][characterIndex + 2] == 'u' &&
               lineByLineContent[lineIndex][characterIndex + 3] == 'e') {
        characterIndex += 4;
        if (skippedData != nullptr) {
            *skippedData = true;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
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
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
        return true;
    } else if (c == 'n' &&
               lineByLineContent[lineIndex][characterIndex + 1] == 'u' &&
               lineByLineContent[lineIndex][characterIndex + 2] == 'l' &&
               lineByLineContent[lineIndex][characterIndex + 3] == 'l') {
        characterIndex += 4;
        if (skippedData != nullptr) {
            *skippedData = nullptr;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, emptyLinesCounter);
        return true;
    }
    cout << "Unknown character to process: " << c << endl;
    return false;
}

bool collectStringJsonContentKeepOrder(  // NOLINT(misc-no-recursion)
        stringstream &stringContent, json const &content, json const &originalContent,
        vector<string> const &lineByLineContent, int &lineIndex, int &characterIndex, int const &indentLevel = 0,
        bool keepNewLines = true, int *upperNewLinesCounter = nullptr, bool verbose = false) {
    if (content.type() != originalContent.type()) {
        stringContent << collectStringJsonContent(content, indentLevel);
        return skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex,
                                                       upperNewLinesCounter);
    }

    // first determine the type of the data at the content
    int preDataNewLinesCounter = 0;
    if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, &preDataNewLinesCounter)) {
        return false;
    }
    if (keepNewLines && preDataNewLinesCounter > 0) {
        if (verbose) {
            cout << "Write new line (x" << preDataNewLinesCounter << ") at L: " << lineIndex << " C: " << characterIndex
                 << endl;
        }
        stringContent << string("\n") * preDataNewLinesCounter << (AndreiUtils::tab * indentLevel);
        preDataNewLinesCounter = 0;
    }
    char const &c = lineByLineContent[lineIndex][characterIndex];
    if (c == '[') {
        // array
        assert(originalContent.is_array());

        vector<nlohmann::json> contentArray = content.get<vector<nlohmann::json>>();
        vector<nlohmann::json> originalContentArray = originalContent.get<vector<nlohmann::json>>();
        if (contentArray.size() != originalContentArray.size()) {
            stringContent << collectStringJsonContent(content, indentLevel);
            return skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex,
                                                           upperNewLinesCounter);
        }

        ++characterIndex;  // move past '[' character
        if (contentArray.empty()) {
            int betweenBracesNewLinesCounter = 0;
            if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, &betweenBracesNewLinesCounter)) {
                return false;
            }
            assert(lineByLineContent[lineIndex][characterIndex] == ']');
            ++characterIndex;
            stringContent << "[";
            if (keepNewLines && betweenBracesNewLinesCounter > 0) {
                stringContent << string("\n") * betweenBracesNewLinesCounter << (AndreiUtils::tab * indentLevel);
            }
            stringContent << "]";
            skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, upperNewLinesCounter);
            return true;
        }

        bool keepArrayValueCondensed = (contentArray.begin()->type() != json::value_t::object &&
                                        contentArray.begin()->type() != json::value_t::array);
        stringContent << "[";
        if (!keepArrayValueCondensed && !keepNewLines) {
            stringContent << endl << AndreiUtils::tab * (indentLevel + 1);
        }
        int valuePostDataNewLinesCounter = 0;
        for (size_t index = 0; index < contentArray.size(); ++index) {
            bool addSpace = false;
            if (index > 0) {
                stringContent << ",";
                if (!keepArrayValueCondensed) {
                    if (keepNewLines) {
                        if (valuePostDataNewLinesCounter > 0) {
                            stringContent << string("\n") * valuePostDataNewLinesCounter
                                          << AndreiUtils::tab * (indentLevel + 1);
                            valuePostDataNewLinesCounter = 0;
                        }
                    } else {
                        stringContent << endl << AndreiUtils::tab * (indentLevel + 1);
                    }
                } else {
                    addSpace = true;
                }
                ++characterIndex;  // skip comma ',' character between array values
            }
            stringstream subArrayValueContent;
            if (!collectStringJsonContentKeepOrder(
                    subArrayValueContent, contentArray[index], originalContentArray[index], lineByLineContent, lineIndex,
                    characterIndex, indentLevel + 1, keepNewLines, &valuePostDataNewLinesCounter, verbose)) {
                return false;
            }
            if (addSpace && !startsWith(subArrayValueContent.str(), "\n")) {
                stringContent << " ";
            }
            stringContent << subArrayValueContent.str();
            if (verbose) {
                cout << " ---------------------- " << endl;
                cout << stringContent.str() << endl;
                cout << " ---------------------- " << endl;
            }
        }
        assert(lineByLineContent[lineIndex][characterIndex] == ']');
        ++characterIndex;
        if (!keepArrayValueCondensed || keepNewLines) {
            if (keepNewLines) {
                if (valuePostDataNewLinesCounter > 0) {
                    stringContent << string("\n") * valuePostDataNewLinesCounter << AndreiUtils::tab * indentLevel;
                }
            } else {
                stringContent << "\n" << AndreiUtils::tab * indentLevel;
            }
        }
        stringContent << "]";
        if (verbose) {
            cout << " ====================== " << endl;
            cout << stringContent.str() << endl;
            cout << " ====================== " << endl;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, upperNewLinesCounter);
    } else if (c == '{') {
        // object
        assert(originalContent.is_object());
        map<string, nlohmann::json> contentMap = content.get<map<string, nlohmann::json>>();
        map<string, nlohmann::json> originalContentMap = originalContent.get<map<string, nlohmann::json>>();

        ++characterIndex;
        if (originalContentMap.empty()) {
            int betweenBracesNewLinesCounter = 0;
            if (!skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, &betweenBracesNewLinesCounter)) {
                return false;
            }
            assert(lineByLineContent[lineIndex][characterIndex] == '}');
            ++characterIndex;
            if (contentMap.empty()) {
                stringContent << "{";
                if (keepNewLines && betweenBracesNewLinesCounter > 0) {
                    stringContent << string("\n") * betweenBracesNewLinesCounter << AndreiUtils::tab * indentLevel;
                }
                stringContent << "}";
            } else {
                stringContent << collectStringJsonContent(content, indentLevel);
            }
            skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, upperNewLinesCounter);
            return true;
        }

        // keep the entries that are common; and write the extra entries that are new!
        stringstream commonEntries;
        stringstream newEntries;
        bool firstNewEntry = true, firstCommonEntry = true;

        std::map<std::string, bool> definedKeys;
        int valuePostDataNewLinesCounter = 0;
        while (true) {
            nlohmann::json keyDatum;
            int keyPreDataNewLinesCounter = 0, keyPostDataNewLinesCounter = 0;
            skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, &keyPreDataNewLinesCounter);
            if (verbose) {
                cout << "KeyPreData: " << keyPreDataNewLinesCounter << endl;
            }
            skipToNextEntryInThisLevelOfJsonContent(
                    lineByLineContent, lineIndex, characterIndex, &keyPostDataNewLinesCounter, &keyDatum);
            if (verbose) {
                cout << "KeyPostData: " << keyPostDataNewLinesCounter << endl;
            }
            assert(keyDatum.is_string());
            assert(lineByLineContent[lineIndex][characterIndex] == ':');
            ++characterIndex;  // skips the colon ':' character
            string key = keyDatum.get<string>();
            definedKeys[key] = true;

            nlohmann::json const *commonDatum;
            if (mapGetIfContains(contentMap, key, commonDatum)) {
                if (!firstCommonEntry) {
                    if (keepNewLines) {
                        if (valuePostDataNewLinesCounter > 0) {
                            commonEntries << string("\n") * valuePostDataNewLinesCounter
                                          << AndreiUtils::tab * (indentLevel + 1) << ",";
                            valuePostDataNewLinesCounter = 0;
                        } else {
                            commonEntries << ",";
                        }
                    } else {
                        commonEntries << ",\n" << AndreiUtils::tab * (indentLevel + 1);
                    }
                }
                if (keepNewLines && keyPreDataNewLinesCounter > 0) {
                    if (verbose) {
                        cout << "Write key pre data (x" << keyPreDataNewLinesCounter << ")" << endl;
                    }
                    commonEntries << string("\n") * keyPreDataNewLinesCounter << AndreiUtils::tab * (indentLevel + 1);
                }
                commonEntries << "\"" << key << "\"";
                if (keepNewLines && keyPostDataNewLinesCounter > 0) {
                    if (verbose) {
                        cout << "Write key post data (x" << keyPostDataNewLinesCounter << ")" << endl;
                    }
                    commonEntries << string("\n") * keyPostDataNewLinesCounter << AndreiUtils::tab * (indentLevel + 1);
                }
                commonEntries << ": ";
                nlohmann::json const &originalCommonDatum = mapGet(originalContentMap, key);
                if (!collectStringJsonContentKeepOrder(
                        commonEntries, *commonDatum, originalCommonDatum, lineByLineContent, lineIndex, characterIndex,
                        indentLevel + 1, keepNewLines, &valuePostDataNewLinesCounter, verbose)) {
                    return false;
                }
                if (verbose) {
                    cout << " ---------------------- " << endl;
                    cout << commonEntries.str() << endl;
                    cout << " ---------------------- " << endl;
                    cout << "Value Post Data: " << valuePostDataNewLinesCounter << endl;
                }
                firstCommonEntry = false;
            } else {
                // Use valuePostDataNewLinesCounter to add the lines that were skipped when an item is deleted!
                if (!skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex,
                                                             &valuePostDataNewLinesCounter)) {
                    return false;
                }
            }

            if (lineByLineContent[lineIndex][characterIndex] == '}') {
                break;
            }
            assert(lineByLineContent[lineIndex][characterIndex] == ',');
            ++characterIndex; // skips the ',' character
        }
        ++characterIndex;
        if (keepNewLines && valuePostDataNewLinesCounter > 0) {
            if (verbose) {
                cout << "After key-value-for-loop: value post new lines = " << valuePostDataNewLinesCounter << endl;
            }
            commonEntries << string("\n") * valuePostDataNewLinesCounter;
        }
        for (auto const &mapData: contentMap) {
            if (!mapContains(definedKeys, mapData.first)) {
                if (!firstNewEntry) {
                    newEntries << "," << endl << AndreiUtils::tab * (indentLevel + 1);
                }
                newEntries << "\"" << mapData.first << "\": ";
                newEntries << collectStringJsonContent(mapData.second, indentLevel + 1);
                firstNewEntry = false;
            }
        }

        string commonString = commonEntries.str(), newString = newEntries.str();
        if (verbose) {
            cout << "CommonString:" << endl;
            cout << "<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
            cout << commonString << endl;
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
            cout << "NewString:" << endl;
            cout << "<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
            cout << newString << endl;
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
        }
        if (!commonString.empty() && !newString.empty()) {
            if (verbose) {
                cout << "?????????????????????" << endl;
                cout << commonString << endl;
                cout << "?????????????????????" << endl;
            }
            if (keepNewLines && (commonString.back() == '\n' || commonString.back() == ' ')) {
                size_t index = commonString.size() - 1;
                while (index > 0 && (commonString[index - 1] == '\n' || commonString[index - 1] == ' ')) {
                    --index;
                }
                commonString[index] = ',';
            } else {
                commonString += ",";
            }
            commonString += "\n" + AndreiUtils::tab * (indentLevel + 1);
            if (verbose) {
                cout << "?????????????????????" << endl;
                cout << commonString << endl;
                cout << "?????????????????????" << endl;
            }
        } else if (!commonString.empty() && newString.empty()) {
            if (keepNewLines) {
                commonString += AndreiUtils::tab * indentLevel;
            }
        } else if (commonString.empty() && !newString.empty()) {
            if (keepNewLines) {
                newString = "\n" + (AndreiUtils::tab * (indentLevel + 1)) + newString;
            }
        } else {
            throw std::runtime_error("No common entries and no new entries should have been processed!");
        }
        if (keepNewLines) {
            if (!newString.empty()) {
                newString += "\n" + (AndreiUtils::tab * indentLevel);
            }
            stringContent << "{" << commonString << newString << "}";
        } else {
            stringContent << "{" << endl;
            stringContent << AndreiUtils::tab * (indentLevel + 1) << commonString << newString << endl;
            stringContent << AndreiUtils::tab * indentLevel << "}";
        }
        if (verbose) {
            cout << " ====================== " << endl;
            cout << stringContent.str() << endl;
            cout << " ====================== " << endl;
        }
        skipWhiteSpaces(lineByLineContent, lineIndex, characterIndex, upperNewLinesCounter);
    } else {
        stringContent << collectStringJsonContent(content, indentLevel);
        skipToNextEntryInThisLevelOfJsonContent(lineByLineContent, lineIndex, characterIndex, upperNewLinesCounter);
    }
    return true;
}

void AndreiUtils::writeJsonFileKeepOrder(  // NOLINT(misc-no-recursion)
        string const &path, json const &content, bool keepNewLines, std::string const &originalContentFilePath) {
    if (originalContentFilePath.empty() || !fileExists(originalContentFilePath)) {
        writeJsonFile(path, content);
    }
    json originalJsonContent(std::move(readJsonFile(originalContentFilePath)));
    if (originalJsonContent.type() != content.type() || !originalJsonContent.is_object()) {
        writeJsonFile(path, content);
    }

    ifstream fin(originalContentFilePath);
    if (!fin.is_open()) {
        throw runtime_error("Can not open file " + originalContentFilePath);
    }
    std::string line;
    vector<string> lineByLineContent;
    while (getline(fin, line)) {
        lineByLineContent.emplace_back(line);
    }

    stringstream stringContent;
    int lineIndex = 0, characterIndex = 0, postDataNewLines = 0;
    if (!collectStringJsonContentKeepOrder(stringContent, content, originalJsonContent, lineByLineContent, lineIndex,
                                           characterIndex, 0, keepNewLines, &postDataNewLines)) {
        throw std::runtime_error("Parsing of the json data succeeded but the line-by-line parsing did not!");
    }
    if (keepNewLines && postDataNewLines > 1) {
        // -1 because the write function adds its own end-line character
        stringContent << string("\n") * (postDataNewLines - 1);
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

void AndreiUtils::writeJsonFile(  // NOLINT(misc-no-recursion)
        string const &path, json const &content, std::string const &originalFilePath, bool keepEmptyLines) {
    if (!originalFilePath.empty()) {
        writeJsonFileKeepOrder(path, content, keepEmptyLines, originalFilePath);
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
