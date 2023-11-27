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

int main() {
    cout << "Hello World!" << endl;

    // testSerializeInt();
    testSerializeString();

    return 0;
}