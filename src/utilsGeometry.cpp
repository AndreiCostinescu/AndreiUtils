/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 07-Mar-22.
//

#include <AndreiUtils/utilsGeometry.h>

using namespace std;

long double const AndreiUtils::DEG2RAD_FACTOR = M_PI / 180.0;
long double const AndreiUtils::RAD2DEG_FACTOR = 180.0 / M_PI;

float AndreiUtils::deg2Rad(float x) { return (float) (x * AndreiUtils::DEG2RAD_FACTOR); }

double AndreiUtils::deg2Rad(double x) { return (double) (x * AndreiUtils::DEG2RAD_FACTOR); }

long double AndreiUtils::deg2Rad(long double x) { return x * AndreiUtils::DEG2RAD_FACTOR; }

float AndreiUtils::rad2Deg(float x) { return (float) (x * AndreiUtils::RAD2DEG_FACTOR); }

double AndreiUtils::rad2Deg(double x) { return (double) (x * AndreiUtils::RAD2DEG_FACTOR); }

long double AndreiUtils::rad2Deg(long double x) { return x * AndreiUtils::RAD2DEG_FACTOR; }

bool AndreiUtils::inContact(const float (&p1)[3], const float (&p2)[3], double threshold) {
    return (pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2)) <= pow(threshold, 2);
}

bool AndreiUtils::inContact(const float (&p1)[3], const float (&p2)[3], double threshold, double &distance) {
    distance = pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2);
    bool check = (distance <= pow(threshold, 2));
    distance = sqrt(distance);
    return check;
}
