//
// Created by Andrei on 26.09.22.
//

#pragma once

#include <AndreiUtils/utilsMap.hpp>
#include <pybind11/embed.h>  // everything needed for embedding
// #include <pybind11/eigen.h>  // everything needed for Eigen conversions
// #include <pybind11/stl.h>    // everything needed for std:: types conversions

namespace py = pybind11;

namespace AndreiUtils {
    class PythonInterface {
    public:
        PythonInterface() noexcept;

        PythonInterface(std::string const &moduleName, std::vector<std::string> const &toImportFunctionNames);

        virtual ~PythonInterface();

        void reInitialize(std::string const &moduleName, std::vector<std::string> const &toImportFunctionNames);

        std::map<std::string, py::function> &getFunctions();

        [[nodiscard]] std::map<std::string, py::function> const &getFunctions() const;

        template<typename... Args>
        py::object callFunction(std::string const &functionName, Args... args) {
            py::function *functionPtr;
            if (!mapGetIfContains(this->functions, functionName, functionPtr)) {
                throw std::runtime_error("PythonInterface does not contain the function \"" + functionName + "\"");
            }
            return (*functionPtr)(args...);
        }

        void cleanup();

        [[nodiscard]] size_t getFunctionSize() const;

        [[nodiscard]] bool isInitialized() const;

    protected:
        static void initInterpreter();

        // Intentionally leak memory because third-party packages (e.g. numpy) use global variables...
        // The destruction order of global variables is not guaranteed in C++ and py::objects call PY_DECREF upon destruction
        // However, if the global/static interpreter is destroyed before those global objects
        //  (because destruction order is not guaranteed in c++!),
        //  them calling PY_DECREF causes segmentation faults because the interpreter was already destroyed!
        // This is the cause for errors at the end of the program!
        // Leaked memory will/should be reclaimed by the OS
        static py::scoped_interpreter *guard;
        std::map<std::string, py::function> functions;
        py::module module;
        bool initialized;
    };
}
