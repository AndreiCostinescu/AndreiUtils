cmake_minimum_required(VERSION 3.17)
project(AndreiUtils)

set(CMAKE_CXX_STANDARD 11)

macro(cumulative_or var1 var2)
    if (NOT DEFINED ${var1})
        set(${var1} OFF)
    endif ()
    if (${var2})
        set(${var1} ON)
    endif ()
endmacro()

macro(append what to)
    if (NOT DEFINED ${to})
        set(${to} ${what})
    else()
        set(${to} "${${to}}_${what}")
    endif ()
endmacro()

option(WITH_EIGEN "Build the code with eigen" OFF)
option(WITH_JSON "Build the code with json" ON)
option(WITH_OPENCV "Build the code with opencv" OFF)
option(WITH_REALSENSE "Build the code with librealsense" OFF)

cumulative_or(USE_EIGEN WITH_EIGEN)
cumulative_or(USE_JSON WITH_JSON)
cumulative_or(USE_OPENCV WITH_OPENCV)
cumulative_or(USE_REALSENSE WITH_REALSENSE)

message("Use eigen set to ${USE_EIGEN}")
message("Use json set to ${USE_JSON}")
message("Use opencv set to ${USE_OPENCV}")
message("Use realsense set to ${USE_REALSENSE}")

if (UNIX)
    set(EXTERNAL_LIBS pthread ${EXTERNAL_LIBS})
else ()
    set(EXTERNAL_LIBS wsock32 ws2_32 ${EXTERNAL_LIBS})
endif ()

if (USE_EIGEN)
    find_package(Eigen3 REQUIRED)
    include_directories(${EIGEN3_INCLUDE_DIRS})
    set(EXTERNAL_LIBS Eigen3::Eigen ${EXTERNAL_LIBS})
endif ()

if (USE_JSON)
    include_directories(json)
endif ()

if (USE_OPENCV)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    set(EXTERNAL_LIBS ${OpenCV_LIBS} ${EXTERNAL_LIBS})
endif ()

if (USE_REALSENSE)
    find_package(realsense2 REQUIRED)
    set(EXTERNAL_LIBS ${realsense2_LIBRARY} ${EXTERNAL_LIBS})
endif ()

include_directories("include")

SET(UTILS_GENERAL src/utils.cpp)

SET(UTILS_TEMPLATE include/andrei_utils/utils.hpp)

SET(UTILS_NETWORK src/NetworkIncludes.cpp src/NetworkData.cpp)

# depends on UTILS_TEMPLATE (andrei_utils.hpp)
SET(UTILS_STRINGS src/utilsString.cpp)

SET(UTILS_THREAD src/utilsThread.cpp)

SET(UTILS_TIME src/utilsTime.cpp)

# depends on UTILS_STRINGS (utilsStrings.h)
SET(UTILS_FILES src/utilsFiles.cpp)

SET(UTILS_CLASSES src/classes/Buffer.cpp include/andrei_utils/classes/CircularArray.hpp include/andrei_utils/classes/FifoQueue.hpp include/andrei_utils/classes/ForkSharedVariable.hpp include/andrei_utils/classes/PriorityQueue.hpp)

SET(UTILS_EIGEN)
if (USE_EIGEN)
    # depends on eigen
    SET(UTILS_EIGEN src/utilsEigen.cpp)
endif ()

SET(UTILS_EIGEN_OPENCV)
if (USE_EIGEN AND USE_OPENCV)
    # depends on eigen and opencv
    SET(UTILS_EIGEN_OPENCV src/utilsEigenOpenCV.cpp)
endif ()

SET(UTILS_JSON)
if (USE_JSON)
    # depends on json
    SET(UTILS_JSON src/utilsJson.cpp json/andrei_utils/json.hpp)
endif ()

SET(UTILS_OPENCV)
if (USE_OPENCV)
    # depends on opencv
    SET(UTILS_OPENCV src/utilsOpenCV.cpp include/andrei_utils/utilsOpenCV.hpp)
endif ()

SET(UTILS_REALSENSE)
if (USE_REALSENSE)
    # depends on librealsense
    SET(UTILS_REALSENSE src/utilsRealsense.cpp)
endif ()

set(UTILS
        ${UTILS_GENERAL}
        ${UTILS_TEMPLATE}

        ${UTILS_NETWORK}
        ${UTILS_STRINGS}
        ${UTILS_THREAD}
        ${UTILS_TIME}

        ${UTILS_FILES}

        ${UTILS_CLASSES}

        ${UTILS_EIGEN}
        ${UTILS_EIGEN_OPENCV}
        ${UTILS_JSON}
        ${UTILS_OPENCV}
        ${UTILS_REALSENSE}
        )
message("Utils = ${UTILS}")

set(LIBRARY_NAME "AndreiUtils")
append(${LIBRARY_NAME} UTILS_USE_PROJECT)
set(TARGET_LIBRARY ${UTILS_USE_PROJECT})
message("The target name is ${TARGET_LIBRARY}")
add_library(TARGET_LIBRARY ${UTILS})
target_link_libraries(TARGET_LIBRARY PRIVATE ${EXTERNAL_LIBS})
message("External libs = ${EXTERNAL_LIBS}")
