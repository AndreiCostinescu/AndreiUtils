//
// Created by Andrei on 03.10.22.
//

#include <AndreiUtils/exceptions/QuitRequestedException.h>

using namespace AndreiUtils;
using namespace std;

QuitRequestedException::QuitRequestedException() : std::runtime_error("Quit requested!") {};
