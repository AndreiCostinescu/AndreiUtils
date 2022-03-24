//
// Created by Andrei on 07-Mar-22.
//

#include <AndreiUtils/utilsGeometry.h>
#include <iostream>

using namespace std;

float AndreiUtils::deg2Rad(float x) {
    return (float) (x * AndreiUtils::DEG2RAD_FACTOR);
}

double AndreiUtils::deg2Rad(double x) {
    return (double) (x * AndreiUtils::DEG2RAD_FACTOR);
}

long double AndreiUtils::deg2Rad(long double x) {
    return x * AndreiUtils::DEG2RAD_FACTOR;
}

float AndreiUtils::rad2Deg(float x) {
    return (float) (x * AndreiUtils::RAD2DEG_FACTOR);
}

double AndreiUtils::rad2Deg(double x) {
    return (double) (x * AndreiUtils::RAD2DEG_FACTOR);
}

long double AndreiUtils::rad2Deg(long double x) {
    return x * AndreiUtils::RAD2DEG_FACTOR;
}

bool AndreiUtils::inContact(const float (&p1)[3], const float (&p2)[3], double threshold) {
    return (pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2)) <= pow(threshold, 2);
}
