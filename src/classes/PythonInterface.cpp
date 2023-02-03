//
// Created by Andrei on 26.09.22.
//

#include <AndreiUtils/classes/PythonInterface.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

PythonInterpreterGuard PythonInterface::guard;  // NOLINT(cert-err58-cpp)

PythonInterface::PythonInterface() : module(), functions() {}

PythonInterface::PythonInterface(string const &moduleName, vector<string> const &toImportFunctionNames) :
        PythonInterface() {
    this->reInitialize(moduleName, toImportFunctionNames);
}

PythonInterface::~PythonInterface() {
    this->cleanup();
}

void PythonInterface::reInitialize(string const &moduleName, vector<string> const &toImportFunctionNames) {
    this->module.release();
    this->functions.clear();
    try {
        this->module = pybind11::module::import(moduleName.c_str());
        for (auto &importNames: toImportFunctionNames) {
            this->functions[importNames] = this->module.attr(importNames.c_str());
        }
    } catch (exception &e) {
        throw std::runtime_error("Exception in python: " + string(e.what()));
    }
}

map<string, py::function> &PythonInterface::getFunctions() {
    return this->functions;
}

map<string, py::function> const &PythonInterface::getFunctions() const {
    return this->functions;
}

void PythonInterface::cleanup() {
    this->functions.clear();
    this->module.release();
}

size_t PythonInterface::getFunctionSize() const {
    return this->functions.size();
}
