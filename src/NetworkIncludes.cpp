//
// Created by Andrei on 21.04.21.
//

#include <AndreiUtils/NetworkIncludes.h>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
bool AndreiUtils::socketsStarted = false;
#else
bool AndreiUtils::socketsStarted = true;
#endif
