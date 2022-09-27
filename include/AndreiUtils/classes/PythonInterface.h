//
// Created by Andrei on 26.09.22.
//

#ifndef ANDREIUTILS_PYTHONINTERFACE_H
#define ANDREIUTILS_PYTHONINTERFACE_H

#include <AndreiUtils/utilsMap.hpp>
#include <pybind11/embed.h>  // everything needed for embedding
// #include <pybind11/eigen.h>  // everything needed for Eigen conversions
// #include <pybind11/stl.h>    // everything needed for std:: types conversions

namespace py = pybind11;

namespace AndreiUtils {
    class PythonInterpreterGuard {
    protected:
        py::scoped_interpreter guard;  // NOLINT(cert-err58-cpp); start the interpreter and keep it alive
    };

    class PythonInterface {
    protected:
        static PythonInterpreterGuard guard;
        std::map<std::string, py::function> functions;
        py::module module;

    public:
        explicit PythonInterface(const std::string &moduleName, const std::vector<std::string> &toImportFunctionNames);

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
    };
}

#endif //ANDREIUTILS_PYTHONINTERFACE_H
