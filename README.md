# AndreiUtils
https://bitbucket.org/AndreiCostinescu/AndreiUtils

## Building the library
- Clone the repository: <br>
  git clone https://bitbucket.org/andreicostinescu/AndreiUtils.git
- ```
  cd AndreiUtils
  mkdir <BUILD_FOLDER_NAME>
  cd <BUILD_FOLDER_NAME>
  cmake .. [-G "MinGW MakeFiles"] -D...
  make -j
  ```
  Check the [CMAKE FLAGS](#markdown-header-cmake-flags) section to check which flags there are available. ``-G "MinGW Makefiles"`` is necessary if compiling on windows with mingw's gcc.

### CMAKE FLAGS
- -DWITH_EIGEN=On/Off (toggle to add/remove eigen utils to the library)
- -DWITH_JSON=On/Off (toggle to add/remove json utils to library)
- -DWITH_OPENCV=On/Off (toggle to add/remove opencv utils to the library)
- -DWITH_REALSENSE=On/Off (toggle to add/remove realsense utils to the library)

### Optional external libraries
- Eigen: https://gitlab.com/libeigen/eigen (https://eigen.tuxfamily.org/index.php?title=Main_Page)
- OpenCV: https://github.com/opencv/opencv (https://opencv.org/)
- realsense: https://github.com/opencv/opencv (https://www.intelrealsense.com/)

## Usage in other (cmake) projects
Embed in your project's CMakeLists.txt
```
set(ANDREI_UTILS_PATH "" CACHE PATH "The (absolute!) path to the AndreiUtils repository directory")
if ("${ANDREI_UTILS_PATH}" STREQUAL "")
    message(FATAL_ERROR "No path to AndreiUtils directory provided")
endif ()
message("AndreiUtils path set to ${ANDREI_UTILS_PATH}")

set(ANDREI_UTILS_DIR "${ANDREI_UTILS_PATH}")
if (UNIX)
    set(ANDREI_UTILS_LIB_FORMAT ".a")
else (UNIX)
    set(ANDREI_UTILS_LIB_FORMAT ".a")
endif ()
set(ANDREI_UTILS_INCLUDE "${ANDREI_UTILS_DIR}/include" "${ANDREI_UTILS_DIR}/json")
set(ANDREI_UTILS_LIB "${ANDREI_UTILS_DIR}/<BUILD_FOLDER_NAME>/libAndreiUtils${ANDREI_UTILS_LIB_FORMAT}")

message("AndreiUtils include dirs set to ${ANDREI_UTILS_INCLUDE}")
message("AndreiUtils library path set to ${ANDREI_UTILS_LIB}")
include_directories(${ANDREI_UTILS_INCLUDE})

add_executable(TestAndreiUtils main.cpp)
target_link_libraries(TestAndreiUtils ${ANDREI_UTILS_LIB})
```
Be sure to replace <BUILD_FOLDER_NAME> with the actual name of the folder in your CMakeLists.txt.
Also remove every backslash ('\') from the above sample.
When using your project, be sure to use cmake with the path to the AndreiUtils repository
(e.g. ```cmake .. -DANDREI_UTILS_PATH="/home/andrei/Coding/AndreiUtils"``` or ```cmake .. -DANDREI_UTILS_PATH="C:/Users/Andrei/Coding/AndreiUtils"```)

