# AndreiUtils

[![CI](https://github.com/AndreiCostinescu/AndreiUtils/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/AndreiCostinescu/AndreiUtils/actions/workflows/ci.yml)
[![Lint](https://github.com/AndreiCostinescu/AndreiUtils/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/AndreiCostinescu/AndreiUtils/actions/workflows/lint.yml)

A collection of general-purpose C++ utility classes and functions, built as a modular CMake library:
the "core" (no external dependencies) is always available, and optional pieces are compiled in only
when the library that backs them is enabled.

- **Core utilities**: string/file/time helpers, binary serialization, random number generation,
  networking, and general-purpose data structures (`SlidingWindow`, `CircularArray`, `FifoQueue`,
  `PriorityQueue`, `UnionFind`, `MixedDataContainer`, `AnyType`, `Interval`, `Timer`, ...) and a
  templated `Graph` with `BFS`/`DFS` traversal.
- **JSON** (`WITH_JSON`): (de)serialization helpers on top of [nlohmann/json](https://github.com/nlohmann/json),
  plus a small hierarchical `Parameters`/`ConfigurationParameters` config system.
- **Eigen** (`WITH_EIGEN`): geometry helpers, dual quaternions, pose interpolation/filtering
  (`SlerpInterpolator`, `PoseDecoupledInterpolator`, `QuaternionLowPassFilter`), and Cartesian
  trajectory utilities.
- **OpenCV** / **librealsense** (`WITH_OPENCV` / `WITH_REALSENSE`): camera intrinsics/capture
  parameter types and conversions between OpenCV, RealSense, and (when Eigen is also enabled) Eigen
  representations.
- **OpenMP** / **pthread** (`WITH_OPENMP` / `WITH_PTHREAD`): parallelization helpers.
- **Python** (`WITH_PYTHON`): embedding Python in C++ via [pybind11](https://github.com/pybind/pybind11).

See the [Library Components](#library-components) section for how these map to CMake targets you can
link against individually.

### Requirements
- CMake 3.16+
- A C++20 compiler (tested with GCC and Clang)

## Building the Library
- Clone the repository: <br>
  ```git clone https://github.com/AndreiCostinescu/AndreiUtils.git```
- Build instructions: <br>
  ```
  cd AndreiUtils
  mkdir <BUILD_FOLDER_NAME>
  cd <BUILD_FOLDER_NAME>
  cmake .. [-G "MinGW MakeFiles"] [-D...]
  make -j
  sudo make install
  ```
  Check the [CMAKE FLAGS](#cmake-flags) section to check which flags there are available. ``-G "MinGW Makefiles"`` is necessary if compiling on windows with mingw's gcc.

### CMAKE FLAGS
- ```-DWITH_ALL=On/Off``` (toggle to add/remove all utils)
- ```-DWITH_EIGEN=On/Off``` (toggle to add/remove eigen utils to the library)
  - ```-DEIGEN_VERSION="3.4"``` (optional argument: set the desired version of the Eigen library; default = "3.4")
- ```-DWITH_JSON=On/Off``` (toggle to add/remove json utils to library)
- ```-DWITH_OPENCV=On/Off``` (toggle to add/remove opencv utils to the library)
  - ```-DOPENCV_VERSION="4.5.1"``` (optional argument: set the desired version of the OpenCV library; default = "") 
- ```-DWITH_OPENMP=On/Off``` (toggle to add/remove openmp utils to the library)
- ```-DWITH_PTHREAD=On/Off``` (toggle to add/remove pthread utils to library)
- ```-DWITH_PYTHON=On/Off``` (toggle to add/remove python/pybind utils to library)
  - ```-DPYBIND11_PYTHON_VERSION="3.7"``` (optional argument: set the desired python version that will be used; default = 3.7)
- ```-DWITH_REALSENSE=On/Off``` (toggle to add/remove realsense utils to the library)
- ```-DWITH_PCL_OPTIMIZATION_FLAGS=On/Off``` (toggle to compile the eigen utils with PCL's own compiler optimization flags, for compatibility with a PCL-based caller; requires PCL to be installed; default = On)
- ```-DWITH_TESTS=On/Off``` (toggle to build the test executables; default = On)
- ```-DCMAKE_INSTALL_PREFIX="{Your Custom Installation Directory}"``` (optional argument: if desired to not install the library in ```/usr/local/```)

### Running Tests
With ```-DWITH_TESTS=On``` (the default), building also produces the test executables. From the build folder, run:
```
ctest --output-on-failure
```
This runs the GoogleTest-based test suite. Some of the built test executables aren't registered with CTest (e.g. ones that read from stdin or are plain manual/demo programs) and need to be run directly instead.

### Optional External Libraries
- Eigen: https://gitlab.com/libeigen/eigen (https://eigen.tuxfamily.org/index.php?title=Main_Page)
- OpenCV: https://github.com/opencv/opencv (https://opencv.org/)
- PyBind11: https://github.com/pybind/pybind11 (https://pybind11.readthedocs.io/en/stable/index.html)
- realsense: https://github.com/IntelRealSense/librealsense (https://www.intelrealsense.com/)

## Usage in Other (Cmake) Projects
Embed in your project's CMakeLists.txt
```
find_package(AndreiUtils REQUIRED COMPONENTS world)
message("AndreiUtils include dirs are ${AndreiUtils_INCLUDE}")
message("AndreiUtils libraries are ${AndreiUtils_LIBRARY}")

add_executable(TestAndreiUtils main.cpp)
target_link_libraries(TestAndreiUtils ${AndreiUtils_LIBRARY})
```
Check the [Library Components](#library-components) section for all available components of the library. <br> 
There's no need for including the include directories of ```AndreiUtils``` explicitly; by linking to ```${AndreiUtils_LIBRARY}``` the include directories are automatically set correctly. 

### Library Components
- ```core```: all utils that to not depend on external libraries
- ```json```: all utils that only use json (e.g. for (de-)serialization)
- ```openmp```: all utils that only use openmp (e.g. for parallelization)
- ```pthread```: all utils that only use pthread
- ```python```: all utils that only use python (e.g. for embedding Python into C++ code)
- ```realsense```: all utils that only use librealsense
- ```opencv```: all utils that only use OpenCV
- ```opencv_realsense```: all utils that only use OpenCV and librealsense
- ```eigen```: all utils that only use Eigen
- ```eigen_json```: all utils that only use Eigen and json
- ```eigen_opencv```: all utils that only use Eigen and OpenCV
- ```eigen_realsense```: all utils that only use Eigen and librealsense
- ```world```: all utils that were enabled in the compilation process of AndreiUtils

Some components include others: e.g. by requesting the "opencv_realsense" component, one is using the "core", "opencv", and "realsense" components as well.

## License
Licensed under the Apache License, Version 2.0 - see [LICENSE](LICENSE) for the full text.
