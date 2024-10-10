//
// Created by Andrei on 24.11.23.
//

#include <AndreiUtils/utilsBinarySerialization.hpp>
#include <AndreiUtils/utilsVector.hpp>
#include <iostream>
#include <gtest/gtest.h>

using namespace AndreiUtils;
using namespace std;

void testSerializeInt() {
    int vector[5] = {1, 2, 3, 4, 5};
    cout << "Vector to serialize: " << printVectorToString(vector, 5) << endl;
    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, vector, 5);
    fout.close();

    int deserialize[5];
    cout << "Vector inside which deserialization occurs before: " << printVectorToString(deserialize, 5) << endl;

    ifstream fin("tmp.bin", std::ios::binary);
    AndreiUtils::deserializeIn(fin, deserialize, 5);
    cout << "Vector inside which deserialization occurs after: " << printVectorToString(deserialize, 5) << endl;

    ifstream fin2("tmp.bin", std::ios::binary);
    cout << fin2.bad() << ", " << fin2.fail() << endl;
    cout << fin2.tellg() << endl;
    cout << AndreiUtils::skip<int>(fin2, 3) << endl;
    cout << fin2.tellg() << endl;
    cout << AndreiUtils::skip<int>(fin2, 3) << endl;
    cout << fin2.tellg() << endl;
    cout << AndreiUtils::skip<int>(fin2, 2) << endl;
    cout << fin2.tellg() << endl;
    cout << AndreiUtils::skip<int>(fin2, 1) << endl;
    cout << fin2.tellg() << endl;
}

void testSerializeString() {
    string s = "Hello World!";
    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, s);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    string sDeserialized;
    AndreiUtils::deserializeIn(fin, sDeserialized);
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
    AndreiUtils::deserializeIn(fin, deserialized);
    cout << "Deserialized data: " << printVectorToString<map<pair<int, double>, string>>(
            deserialized, [](map<pair<int, double>, string> const &m) {
                return "{" + printMapToStringConvertKey<pair<int, double>>(m, [](pair<int, double> const &key) {
                    return "[" + to_string(key.first) + ", " + to_string(key.second) + "]";
                }) + "}";
            }) << endl;
}

TEST(SerializeTest, TestSerializeInt) {
    int vector[5] = {1, 2, 3, 4, 5};

    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, vector, 5);
    fout.close();

    int deserialize[5] = {0};

    ifstream fin("tmp.bin", std::ios::binary);
    AndreiUtils::deserializeIn(fin, deserialize, 5);

    ifstream fin2("tmp.bin", std::ios::binary);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(vector[i], deserialize[i]);
    }

    EXPECT_FALSE(fin2.bad());
    EXPECT_FALSE(fin2.fail());
    EXPECT_FALSE(fin2.tellg());

    bool result = AndreiUtils::skip<int>(fin2, 3);
    EXPECT_TRUE(result);
    EXPECT_EQ(fin2.tellg(), 3 * sizeof(int));

    result = AndreiUtils::skip<int>(fin2, 3);
    EXPECT_FALSE(result);
    EXPECT_EQ(fin2.tellg(), 3 * sizeof(int));

    EXPECT_TRUE(fin2.good());
}

TEST(SerializeTest, SerializeStringTest) {
    std::string s = "Hello World!";
    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, s);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    string sDeserialized;
    AndreiUtils::deserializeIn(fin, sDeserialized);
    EXPECT_EQ(s, sDeserialized);
}

TEST(SerializeTest, SerializeComplex) {
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

    ofstream fout("tmp.bin", std::ios::binary);
    serialize(fout, toSerialize);
    fout.close();

    ifstream fin("tmp.bin", std::ios::binary);
    AndreiUtils::deserializeIn(fin, deserialized);

    ASSERT_EQ(toSerialize.size(), deserialized.size()) ;

    for (size_t i = 0; i < toSerialize.size(); ++i) {
        EXPECT_EQ(toSerialize[i], deserialized[i]);
    }
}

int main(int argc, char **argv) {
    cout << "Hello World!" << endl;

    // testSerializeInt();
    // testSerializeString();
    // testSerializeComplex();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}