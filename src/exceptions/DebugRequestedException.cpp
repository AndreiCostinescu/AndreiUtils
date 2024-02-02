//
// Created by Andrei on 02.08.23.
//

#include <AndreiUtils/exceptions/DebugRequestedException.h>

using namespace AndreiUtils;
using namespace std;

DebugRequestedException::DebugRequestedException() : std::runtime_error("Debug requested!") {};
