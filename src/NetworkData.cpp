//
// Created by Andrei on 27.08.21.
//

#include <AndreiUtils/NetworkData.h>
#include <AndreiUtils/NetworkIncludes.h>
#include <cstdint>

using namespace AndreiUtils;

int NetworkData::bigEndian = -1;

void setBigEndian() {
    auto p = (short) 0xfeff;
    auto *c = (uint8_t *) &p;
    auto x = short(256 * (*c) + (*(c + 1)));
    // cout << "x1 = " << x1 << " and x2 = " << x2 << " : " << (x1 == x2) << endl;
    NetworkData::bigEndian = (p == x);
}

bool NetworkData::isBigEndian() {
    if (NetworkData::bigEndian < 0) {
        setBigEndian();
    }
    return NetworkData::bigEndian == 1;
}

bool NetworkData::isLittleEndian() {
    return !isBigEndian();
}

void NetworkData::shortToNetworkBytes(char *buffer, int start, short value) {
    short x = htons(value);
    memcpy(buffer + start, &x, sizeof(short));
}

short NetworkData::networkBytesToShort(const char *buffer, int start) {
    auto a = static_cast<short>(static_cast<unsigned char>(buffer[start]) << 8 |
                                static_cast<unsigned char>(buffer[start + 1]));
    return a;
}

void NetworkData::intToNetworkBytes(char *buffer, int start, int value) {
    int x = htonl(value);
    memcpy(buffer + start, &x, sizeof(int));
}

int NetworkData::networkBytesToInt(const char *buffer, int start) {
    auto a = static_cast<int>(static_cast<unsigned char>(buffer[start]) << 24 |
                              static_cast<unsigned char>(buffer[start + 1]) << 16 |
                              static_cast<unsigned char>(buffer[start + 2]) << 8 |
                              static_cast<unsigned char>(buffer[start + 3]));
    return a;
}

void NetworkData::longLongToNetworkBytes(char *buffer, int start, long long value) {
    // Network Bytes = Big Endian (0xfeff is stored as 0xfe and 0xff)
    //              Little Endian (0xfeff is stored as 0xff and 0xfe)
    char *c = (char *) &value, inc = 1, size = sizeof(long long) - 1;
    if (isLittleEndian()) {
        c = c + size;
        inc = -1;
    }
    for (int i = 0; i <= size; i++, c += inc) {
        buffer[start + i] = *c;
    }
}

long long NetworkData::networkBytesToLongLong(const char *buffer, int start) {
    long long a = 0;
    char inc = 0, size = sizeof(long long);
    for (int i = size - 1; i >= 0; i--, inc += 8) {
        a |= ((long long) ((unsigned char) buffer[start + i])) << inc;
    }
    return a;
}

void NetworkData::floatToNetworkBytes(char *buffer, int start, float value) {
    memcpy(buffer + start, &value, sizeof(float));
}

float NetworkData::networkBytesToFloat(const char *buffer, int start) {
    float a;
    memcpy(&a, buffer + start, sizeof(float));
    return a;
}

void NetworkData::doubleToNetworkBytes(char *buffer, int start, double value) {
    memcpy(buffer + start, &value, sizeof(double));
}

double NetworkData::networkBytesToDouble(const char *buffer, int start) {
    double a;
    memcpy(&a, buffer + start, sizeof(double));
    return a;
    /*
    auto a = static_cast<double>(static_cast<unsigned char>(buffer[start]) << 56 |
                                 static_cast<unsigned char>(buffer[start + 1]) << 48 |
                                 static_cast<unsigned char>(buffer[start + 2]) << 40 |
                                 static_cast<unsigned char>(buffer[start + 3]) << 32 |
                                 static_cast<unsigned char>(buffer[start + 4]) << 24 |
                                 static_cast<unsigned char>(buffer[start + 5]) << 16 |
                                 static_cast<unsigned char>(buffer[start + 6]) << 8 |
                                 static_cast<unsigned char>(buffer[start + 7]));
    return a;
    //*/
}
