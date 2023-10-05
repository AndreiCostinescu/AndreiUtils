//
// Created by Andrei on 02.08.23.
//

#pragma once

#include <stdexcept>

namespace AndreiUtils {
    class DebugRequestedException : public std::runtime_error {
    public:
        DebugRequestedException();
    };
}
