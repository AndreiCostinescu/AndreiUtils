# AndreiUtils
https://bitbucket.org/AndreiCostinescu/AndreiUtils

## Building the Library
- Clone the repository: <br>
  ```git clone https://bitbucket.org/AndreiCostinescu/AndreiUtils.git```
- Build instructions: <br>
  ```
  cd AndreiUtils
  mkdir <BUILD_FOLDER_NAME>
  cd <BUILD_FOLDER_NAME>
  cmake .. [-G "MinGW MakeFiles"] [-D...]
  make -j
  sudo make install
  ```
  Check the [CMAKE FLAGS](#markdown-header-cmake-flags) section to check which flags there are available. ``-G "MinGW Makefiles"`` is necessary if compiling on windows with mingw's gcc.

### CMAKE FLAGS
- ```-DWITH_ALL=On/Off``` (toggle to add/remove all utils)
- ```-DWITH_EIGEN=On/Off``` (toggle to add/remove eigen utils to the library)
- ```-DWITH_JSON=On/Off``` (toggle to add/remove json utils to library)
- ```-DWITH_OPENCV=On/Off``` (toggle to add/remove opencv utils to the library)
  - ```-DOPENCV_VERSION="4.5.1"``` (optional argument: set the desired version of the OpenCV library; default = "") 
- ```-DWITH_OPENMP=On/Off``` (toggle to add/remove openmp utils to the library)
- ```-DWITH_PTHREAD=On/Off``` (toggle to add/remove pthread utils to library)
- ```-DWITH_PYBIND=On/Off``` (toggle to add/remove python/pybind utils to library)
  - ```-DPYBIND_PYTHON_VERSION="3.7"``` (optional argument: set the desired python version that will be used; default = 3.7)
- ```-DWITH_REALSENSE=On/Off``` (toggle to add/remove realsense utils to the library)
- ```-DCMAKE_INSTALL_DIR="{Your Custom Installation Directory}"``` (optional argument: if desired to not install the library in ```/usr/local/```)

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
Check the [Library Components](#markdown-header-library-components) section for all available components of the library. <br> 
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
- ```eigen_opencv```: all utils that only use Eigen and OpenCV
- ```eigen_realsense```: all utils that only use Eigen and librealsense
- ```world```: all utils that were enabled in the compilation process of AndreiUtils

Some components include others: e.g. by requesting the "opencv_realsense" component, one is using the "core", "opencv", and "realsense" components as well.