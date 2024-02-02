//
// Created by Andrei on 03.10.22.
//

#pragma once

#include <stdexcept>

namespace AndreiUtils {
    class QuitRequestedException : public std::runtime_error {
    public:
        QuitRequestedException();
    };
}
