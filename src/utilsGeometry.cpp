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
