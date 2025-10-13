//
// Created by Andrei on 26.09.22.
//

#include <AndreiUtils/classes/PythonInterface.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

py::scoped_interpreter *PythonInterface::guard = nullptr;  // NOLINT(cert-err58-cpp)

PythonInterface::PythonInterface() noexcept : functions(), module(), initialized(false) {}

PythonInterface::PythonInterface(string const &moduleName, vector<string> const &toImportFunctionNames) :
        PythonInterface() {
    this->reInitialize(moduleName, toImportFunctionNames);
}

PythonInterface::~PythonInterface() {
    this->cleanup();
}

void PythonInterface::reInitialize(string const &moduleName, vector<string> const &toImportFunctionNames) {
    AndreiUtils::PythonInterface::initInterpreter();
    this->module.release();
    this->functions.clear();
    try {
        this->module = pybind11::module::import(moduleName.c_str());
        for (auto &importNames: toImportFunctionNames) {
            this->functions[importNames] = this->module.attr(importNames.c_str());
        }
        this->initialized = true;
    } catch (exception &e) {
        this->initialized = false;
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
    this->initialized = false;
}

size_t PythonInterface::getFunctionSize() const {
    return this->functions.size();
}

bool PythonInterface::isInitialized() const {
    return this->initialized;
}

void PythonInterface::initInterpreter() {
    if (PythonInterface::guard == nullptr) {
        PythonInterface::guard = new py::scoped_interpreter;  // intentionally leak memory... to be reclaimed by OS
    }
}
