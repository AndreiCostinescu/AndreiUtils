//
// Created by Andrei on 24.11.23.
//

#include <AndreiUtils/utilsBinarySerialization.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

void testSerializeInt() {
    int vector[5] = {1, 2, 3, 4, 5};
    cout << "Vector to serialize: " << printVectorToString(vector, 5) << endl;
    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, vector, 5);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    int deserialize[5];
    cout << "Vector inside which deserialization occurs before: " << printVectorToString(deserialize, 5) << endl;

    AndreiUtils::deserialize(fin, deserialize, 5);
    cout << "Vector inside which deserialization occurs after: " << printVectorToString(deserialize, 5) << endl;
}

void testSerializeString() {
    string s = "Hello World!";
    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, s);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    string sDeserialized;
    AndreiUtils::deserialize(fin, sDeserialized);
    cout << "Deserialized string: " << sDeserialized << endl;
}

void testSerializeComplex() {
    vector<map<pair<int, double>, string>> toSerialize, deserialized;
    toSerialize.emplace_back(map<pair<int, double>, string>{{{1, 1.5},  "firstInterval"},
                                                            {{2, 2.89}, "secondInterval"},
                                                            {{3, 3.4},  "thirdInterval"}});
    toSerialize.emplace_back(map<pair<int, double>, string>{{{4, 4.21}, "firstInterval"},
                                                            {{5, 5.2},  "secondInterval"},
                                                            {{5, 5.63}, "thirdInterval"},
                                                            {{6, 6.4},  "fourthInterval"}});
    toSerialize.emplace_back();
    toSerialize.emplace_back(map<pair<int, double>, string>{{{7, 7.111}, "firstInterval"}});
    toSerialize.emplace_back(map<pair<int, double>, string>{{{8,  8.86},   "firstInterval"},
                                                            {{9,  9.9999}, "secondInterval"},
                                                            {{10, 10.36},  "thirdInterval"},
                                                            {{11, 11.81},  "fourthInterval"}});
    toSerialize.emplace_back();
    cout << "To serialize data: " << printVectorToString<map<pair<int, double>, string>>(
            toSerialize, [](map<pair<int, double>, string> const &m) {
                return "{" + printMapToStringConvertKey<pair<int, double>>(m, [](pair<int, double> const &key) {
                    return "[" + to_string(key.first) + ", " + to_string(key.second) + "]";
                }) + "}";
            }) << endl;

    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, toSerialize);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    AndreiUtils::deserialize(fin, deserialized);
    cout << "Deserialized data: " << printVectorToString<map<pair<int, double>, string>>(
            deserialized, [](map<pair<int, double>, string> const &m) {
                return "{" + printMapToStringConvertKey<pair<int, double>>(m, [](pair<int, double> const &key) {
                    return "[" + to_string(key.first) + ", " + to_string(key.second) + "]";
                }) + "}";
            }) << endl;
}

int main() {
    cout << "Hello World!" << endl;

    // testSerializeInt();
    // testSerializeString();
    testSerializeComplex();

    return 0;
}