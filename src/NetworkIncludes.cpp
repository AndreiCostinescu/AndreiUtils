//
// Created by andrei on 21.04.21.
//

#include <NetworkIncludes.h>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
bool socketsStarted = false;
#else
bool socketsStarted = true;
#endif
