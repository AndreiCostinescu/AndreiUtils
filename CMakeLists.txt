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
    else ()
        set(${to} "${${to}}_${what}")
    endif ()
endmacro()

option(WITH_ALL "Build the code with all extensions" OFF)
option(WITH_EIGEN "Build the code with eigen" OFF)
option(WITH_JSON "Build the code with json" ON)
option(WITH_OPENCV "Build the code with opencv" OFF)
option(WITH_REALSENSE "Build the code with librealsense" OFF)

set(OPENCV_VERSION "" CACHE STRING "The opencv version to use in the project")

# used only when using AndreiUtils as a submodule/subdirectory in the upper project
cumulative_or(USE_EIGEN WITH_EIGEN)
cumulative_or(USE_EIGEN WITH_ALL)
cumulative_or(USE_JSON WITH_JSON)
cumulative_or(USE_JSON WITH_ALL)
cumulative_or(USE_OPENCV WITH_OPENCV)
cumulative_or(USE_OPENCV WITH_ALL)
cumulative_or(USE_REALSENSE WITH_REALSENSE)
cumulative_or(USE_REALSENSE WITH_ALL)

message("Use eigen set to ${USE_EIGEN}")
message("Use json set to ${USE_JSON}")
message("Use opencv set to ${USE_OPENCV}")
message("Use realsense set to ${USE_REALSENSE}")

if (USE_EIGEN)
    find_package(Eigen3 REQUIRED)
    include_directories(${EIGEN3_INCLUDE_DIRS})
endif ()

if (USE_JSON)
    include_directories(json)
endif ()

if (USE_OPENCV)
    if ("${OPENCV_VERSION}" STREQUAL "")
        find_package(OpenCV REQUIRED)
    else ()
        find_package(OpenCV ${OPENCV_VERSION} REQUIRED)
    endif ()
    include_directories(${OpenCV_INCLUDE_DIRS})
endif ()

if (USE_REALSENSE)
    find_package(realsense2 REQUIRED)
endif ()

include_directories("include")

SET(UTILS_GENERAL src/utils.cpp)

SET(UTILS_TEMPLATE include/AndreiUtils/utils.hpp)

SET(UTILS_NETWORK src/NetworkIncludes.cpp src/NetworkData.cpp)

# depends on UTILS_TEMPLATE (utils.hpp)
SET(UTILS_STRINGS src/utilsString.cpp)

SET(UTILS_THREAD src/utilsThread.cpp)

SET(UTILS_TIME src/utilsTime.cpp)

# depends on UTILS_STRINGS (utilsStrings.h)
SET(UTILS_FILES src/utilsFiles.cpp)

SET(UTILS_CLASSES
        src/classes/Buffer.cpp
        src/classes/GaussianKernel.cpp
        src/classes/Timer.cpp
        include/AndreiUtils/classes/CircularArray.hpp
        include/AndreiUtils/classes/FifoQueue.hpp
        include/AndreiUtils/classes/ForkSharedVariable.hpp
        include/AndreiUtils/classes/PriorityQueue.hpp
        include/AndreiUtils/classes/SlidingWindow.hpp
        )

SET(UTILS_EIGEN)
SET(UTILS_EIGEN_OPENCV)
SET(UTILS_JSON)
SET(UTILS_OPENCV)
SET(UTILS_REALSENSE)

if (USE_EIGEN)
    # depends on eigen
    SET(UTILS_EIGEN src/utilsEigen.cpp src/utilsEigenGeometry.cpp include/AndreiUtils/utilsEigen.hpp)
    SET(UTILS_CLASSES include/AndreiUtils/classes/SlidingWindowEigen.hpp src/classes/CrossBilateralFilterEigen.cpp ${UTILS_CLASSES})
    SET(UTILS_JSON include/AndreiUtils/utilsJsonEigen.hpp ${UTILS_JSON})
endif ()

if (USE_EIGEN AND USE_OPENCV)
    # depends on eigen and opencv
    SET(UTILS_EIGEN_OPENCV src/utilsEigenOpenCV.cpp)
endif ()

if (USE_JSON)
    # depends on json
    SET(UTILS_JSON src/utilsJson.cpp ${UTILS_JSON})
endif ()

if (USE_OPENCV)
    # depends on opencv
    SET(UTILS_OPENCV src/utilsOpenCV.cpp)
    SET(UTILS_CLASSES include/AndreiUtils/classes/SlidingWindowOpenCV.hpp src/classes/CrossBilateralFilterOpenCV.cpp ${UTILS_CLASSES})
    SET(UTILS_JSON include/AndreiUtils/utilsJsonOpenCV.hpp ${UTILS_JSON})
endif ()

if (USE_REALSENSE)
    # depends on librealsense
    SET(UTILS_REALSENSE src/utilsRealsense.cpp)
endif ()

set(GENERAL_UTILS
        ${UTILS_GENERAL}
        ${UTILS_TEMPLATE}

        ${UTILS_NETWORK}
        ${UTILS_STRINGS}
        ${UTILS_THREAD}
        ${UTILS_TIME}

        ${UTILS_FILES}

        ${UTILS_CLASSES}
        )
set(EIGEN_UTILS ${UTILS_EIGEN})
set(EIGEN_OPENCV_UTILS ${UTILS_EIGEN_OPENCV})
set(JSON_UTILS ${UTILS_JSON})
set(OPENCV_UTILS ${UTILS_OPENCV})
set(REALSENSE_UTILS ${UTILS_REALSENSE})

set(LIBRARY_NAME "AndreiUtils")

# GENERAL_UTILS
set(LIB_NAME "${LIBRARY_NAME}_GENERAL")
add_library(${LIB_NAME} ${GENERAL_UTILS})

if (USE_JSON)
    # JSON_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_JSON")
    add_library(${LIB_NAME} ${JSON_UTILS})
endif ()

if (USE_EIGEN)
    # EIGEN_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_EIGEN")
    add_library(${LIB_NAME} ${EIGEN_UTILS})
endif ()

if (USE_OPENCV)
    # OPENCV_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_OPENCV")
    add_library(${LIB_NAME} ${OPENCV_UTILS})
endif ()

if (USE_REALSENSE)
    # REALSENSE_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_REALSENSE")
    add_library(${LIB_NAME} ${REALSENSE_UTILS})
endif ()

if (USE_EIGEN AND USE_OPENCV)
    # EIGEN_OPENCV_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_EIGEN_OPENCV")
    add_library(${LIB_NAME} ${EIGEN_OPENCV_UTILS})
endif ()

# UTILS
set(UTILS
        ${GENERAL_UTILS}
        ${EIGEN_UTILS}
        ${EIGEN_OPENCV_UTILS}
        ${JSON_UTILS}
        ${OPENCV_UTILS}
        ${REALSENSE_UTILS}
        )
message("Utils = ${UTILS}")

append(${LIBRARY_NAME} UTILS_USE_PROJECT)
set(TARGET_LIBRARY ${UTILS_USE_PROJECT})
message("The target name is ${TARGET_LIBRARY}")
add_library(${TARGET_LIBRARY} ${UTILS})
