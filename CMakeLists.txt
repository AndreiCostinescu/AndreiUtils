cmake_minimum_required(VERSION 3.16)

project(AndreiUtils)  # sets PROJECT_NAME variable
set(PROJECT_VERSION_STRING "1.0.0")
set(PROJECT_TARGET ${PROJECT_NAME})
set(PROJECT_INCLUDE_PREFIX ${PROJECT_NAME})

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
option(WITH_OPENMP "Build the code with openmp" OFF)
option(WITH_PTHREAD "Build the code with pthread" OFF)
option(WITH_PYTHON "Build the code with pybind" OFF)
option(WITH_REALSENSE "Build the code with librealsense" OFF)

set(OPENCV_VERSION "" CACHE STRING "The opencv version to use in the project")
set(PYBIND11_PYTHON_VERSION "3.7" CACHE STRING "The python version to use in the project")

# used only when using AndreiUtils as a submodule/subdirectory in the upper project
cumulative_or(USE_EIGEN WITH_EIGEN)
cumulative_or(USE_EIGEN WITH_ALL)
cumulative_or(USE_JSON WITH_JSON)
cumulative_or(USE_JSON WITH_ALL)
cumulative_or(USE_OPENCV WITH_OPENCV)
cumulative_or(USE_OPENCV WITH_ALL)
cumulative_or(USE_OPENMP WITH_OPENMP)
cumulative_or(USE_OPENMP WITH_ALL)
cumulative_or(USE_PTHREAD WITH_PTHREAD)
cumulative_or(USE_PTHREAD WITH_ALL)
cumulative_or(USE_PYTHON WITH_PYTHON)
cumulative_or(USE_PYTHON WITH_ALL)
cumulative_or(USE_REALSENSE WITH_REALSENSE)
cumulative_or(USE_REALSENSE WITH_ALL)

message("Use eigen set to ${USE_EIGEN}")
message("Use json set to ${USE_JSON}")
message("Use opencv set to ${USE_OPENCV}")
message("Use openmp set to ${USE_OPENMP}")
message("Use pthread set to ${USE_PTHREAD}")
message("Use python set to ${USE_REALSENSE}")
message("Use realsense set to ${USE_REALSENSE}")

if (WIN32)
    set(ANDREI_UTILS_REQUIRED_LIBS wsock32 ws2_32 ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_GENERAL_REQUIRED_LIBS wsock32 ws2_32 ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
else (WIN32)
    set(ANDREI_UTILS_REQUIRED_LIBS pthread ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_GENERAL_REQUIRED_LIBS pthread ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
endif (WIN32)

if (USE_EIGEN)
    find_package(Eigen3 REQUIRED)
    set(ANDREI_UTILS_REQUIRED_LIBS Eigen3::Eigen ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_EIGEN_REQUIRED_LIBS Eigen3::Eigen ${ANDREI_UTILS_EIGEN_REQUIRED_LIBS})
endif ()

if (USE_JSON)
    include_directories(json)
endif ()

if (USE_OPENCV)
    if ("${OPENCV_VERSION}" STREQUAL "")
        find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui calib3d)
    else ()
        find_package(OpenCV ${OPENCV_VERSION} REQUIRED COMPONENTS core imgproc highgui calib3d)
    endif ()
    set(ANDREI_UTILS_REQUIRED_LIBS ${OpenCV_LIBS} ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_OPENCV_REQUIRED_LIBS ${OpenCV_LIBS} ${ANDREI_UTILS_OPENCV_REQUIRED_LIBS})
endif ()

if (USE_OPENMP)
    find_package(OpenMP REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    set(ANDREI_UTILS_REQUIRED_LIBS ${OpenMP_CXX_LIBRARIES} ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_OPENMP_REQUIRED_LIBS ${OpenMP_CXX_LIBRARIES} ${ANDREI_UTILS_OPENMP_REQUIRED_LIBS})
endif ()

if (USE_PYTHON)
    if ((DEFINED PYBIND11_PYTHON_VERSION) AND (${PYBIND11_PYTHON_VERSION}))
        find_package(Python3 ${PYBIND11_PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
        message("Found package python with version ${PYBIND11_PYTHON_VERSION}")
    endif ()
    find_package(pybind11 REQUIRED)
    set(PYBIND11_LIB pybind11::embed)
    set(ANDREI_UTILS_REQUIRED_LIBS ${PYBIND11_LIB} ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_PYTHON_REQUIRED_LIBS ${PYBIND11_LIB} ${ANDREI_UTILS_PYTHON_REQUIRED_LIBS})
endif ()

if (USE_REALSENSE)
    find_package(realsense2 REQUIRED)
    set(ANDREI_UTILS_REQUIRED_LIBS ${realsense2_LIBRARY} ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_REALSENSE_REQUIRED_LIBS ${realsense2_LIBRARY} ${ANDREI_UTILS_REALSENSE_REQUIRED_LIBS})
endif ()

include_directories("include")

SET(UTILS_GENERAL src/utils.cpp)

SET(UTILS_ENUMS src/enums/StandardTypes.cpp src/enums/ImageDistortionModel.cpp src/enums/TimeUnit.cpp src/enums/InvalidValuesHandlingMode.cpp)

SET(UTILS_ERROR src/utilsError.cpp)

SET(UTILS_GEOMETRY src/utilsGeometry.cpp)

SET(UTILS_IMAGES src/utilsImages.cpp)

SET(UTILS_NETWORK src/NetworkIncludes.cpp src/NetworkData.cpp)

# depends on utils.hpp
SET(UTILS_STRINGS src/utilsString.cpp)

SET(UTILS_THREAD src/utilsThread.cpp)

SET(UTILS_TIME src/utilsTime.cpp)

# depends on UTILS_STRINGS (utilsStrings.h)
SET(UTILS_FILES src/utilsFiles.cpp)

SET(UTILS_CAMERA src/utilsCamera.cpp)

set(UTILS_CLASSES_CAMERA
        src/classes/camera/ImageParameters.cpp
        src/classes/camera/CameraIntrinsicParameters.cpp
        src/classes/camera/ImageCaptureParameters.cpp
        src/classes/camera/ImageCaptureParametersWithIntrinsics.cpp
        )

set(UTILS_CLASSES_GRAPH
        src/classes/graph/NodeData.cpp
        src/classes/graph/EdgeData.cpp
        )

SET(UTILS_CLASSES
        src/classes/Buffer.cpp
        src/classes/GaussianKernel.cpp
        src/classes/MixedDataContainer.cpp
        src/classes/Timer.cpp
        src/classes/UnionFind.cpp
        ${UTILS_CLASSES_CAMERA}
        ${UTILS_CLASSES_GRAPH}
        )

SET(UTILS_EIGEN)
SET(UTILS_EIGEN_JSON)
SET(UTILS_EIGEN_OPENCV)
SET(UTILS_EIGEN_REALSENSE)
SET(UTILS_JSON)
SET(UTILS_OPENCV)
SET(UTILS_OPENCV_REALSENSE)
SET(UTILS_OPENMP)
SET(UTILS_PTHREAD)
SET(UTILS_PYTHON)
SET(UTILS_REALSENSE)

if (USE_EIGEN)
    # depends on eigen
    SET(UTILS_EIGEN
            src/utilsEigen.cpp src/utilsEigenGeometry.cpp src/utilsEigenLeastSquares.cpp
            src/classes/motion/MotionDeviceIntrinsicParameters.cpp src/classes/motion/MotionDeviceCaptureParameters.cpp
            src/classes/Symmetry.cpp src/enums/EigenLeastSquaresSolver.cpp)
endif ()

if (USE_EIGEN AND USE_JSON)
    # depends on eigen and json
    SET(UTILS_EIGEN_JSON src/utilsJsonEigen.cpp)
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
endif ()

if (USE_OPENCV AND USE_REALSENSE)
    SET(UTILS_OPENCV_REALSENSE src/utilsOpenCVRealsense.cpp)
endif ()

if (USE_OPENMP)
    SET(UTILS_OPENMP include/AndreiUtils/utilsOpenMP.hpp src/utilsOpenMP.cpp)
endif ()

if (USE_PTHREAD)
    SET(UTILS_PTHREAD src/utilsPThread.cpp)
endif ()

if (USE_PYTHON)
    SET(UTILS_PYTHON src/classes/PythonInterface.cpp)
endif ()

if (USE_REALSENSE)
    # depends on librealsense
    SET(UTILS_REALSENSE src/utilsRealsense.cpp)
endif ()

if (USE_EIGEN AND USE_REALSENSE)
    # depends on eigen and realsense
    SET(UTILS_EIGEN_REALSENSE src/utilsRealsenseEigen.cpp)
endif ()

set(GENERAL_UTILS
        ${UTILS_GENERAL}
        ${UTILS_ENUMS}
        ${UTILS_ERROR}
        ${UTILS_GEOMETRY}
        ${UTILS_IMAGES}

        ${UTILS_NETWORK}
        ${UTILS_STRINGS}
        ${UTILS_THREAD}
        ${UTILS_TIME}

        ${UTILS_FILES}

        ${UTILS_CAMERA}

        ${UTILS_CLASSES}
        )
set(EIGEN_UTILS ${UTILS_EIGEN})
set(EIGEN_JSON_UTILS ${UTILS_EIGEN_JSON})
set(EIGEN_OPENCV_UTILS ${UTILS_EIGEN_OPENCV})
set(EIGEN_REALSENSE_UTILS ${UTILS_EIGEN_REALSENSE})
set(JSON_UTILS ${UTILS_JSON})
set(OPENCV_UTILS ${UTILS_OPENCV})
set(OPENCV_REALSENSE_UTILS ${UTILS_OPENCV_REALSENSE})
set(OPENMP_UTILS ${UTILS_OPENMP})
set(PTHREAD_UTILS ${UTILS_PTHREAD})
set(PYTHON_UTILS ${UTILS_PYTHON})
set(REALSENSE_UTILS ${UTILS_REALSENSE})

set(LIBRARY_NAME ${PROJECT_NAME})

# GENERAL_UTILS
set(LIB_NAME "${LIBRARY_NAME}_core")
add_library(${LIB_NAME} ${GENERAL_UTILS})
target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})

if (USE_JSON)
    # JSON_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_json")
    add_library(${LIB_NAME} ${JSON_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_JSON_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
endif ()

if (USE_OPENMP)
    # OPENMP_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_openmp")
    add_library(${LIB_NAME} ${OPENMP_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_OPENMP_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
endif ()

if (USE_PTHREAD)
    # PTHREAD_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_pthread")
    add_library(${LIB_NAME} ${PTHREAD_UTILS})
    target_link_libraries(${LIB_NAME} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
endif ()

if (USE_PYTHON)
    # PYTHON_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_python")
    add_library(${LIB_NAME} ${PYTHON_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_PYTHON_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
endif ()

if (USE_REALSENSE)
    # REALSENSE_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_realsense")
    add_library(${LIB_NAME} ${REALSENSE_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_REALSENSE_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
endif ()

if (USE_OPENCV)
    # OPENCV_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_opencv")
    add_library(${LIB_NAME} ${OPENCV_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_OPENCV_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})

    if (USE_REALSENSE)
        # OPENCV_REALSENSE_UTILS
        set(LIB_NAME "${LIBRARY_NAME}_opencv_realsense")
        add_library(${LIB_NAME} ${OPENCV_REALSENSE_UTILS})
        target_link_libraries(${LIB_NAME} ${LIBRARY_NAME}_realsense ${LIBRARY_NAME}_opencv)
        target_include_directories(${LIB_NAME} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
                )
        set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
    endif ()
endif ()

if (USE_EIGEN)
    # EIGEN_UTILS
    set(LIB_NAME "${LIBRARY_NAME}_eigen")
    add_library(${LIB_NAME} ${EIGEN_UTILS})
    target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_EIGEN_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    target_include_directories(${LIB_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})

    if (USE_JSON)
        # EIGEN_JSON_UTILS
        set(LIB_NAME "${LIBRARY_NAME}_eigen_json")
        add_library(${LIB_NAME} ${EIGEN_JSON_UTILS})
        target_link_libraries(${LIB_NAME} ${LIBRARY_NAME}_eigen ${LIBRARY_NAME}_json)
        target_include_directories(${LIB_NAME} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
                )
        set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
    endif ()

    if (USE_OPENCV)
        # EIGEN_OPENCV_UTILS
        set(LIB_NAME "${LIBRARY_NAME}_eigen_opencv")
        add_library(${LIB_NAME} ${EIGEN_OPENCV_UTILS})
        target_link_libraries(${LIB_NAME} ${LIBRARY_NAME}_eigen ${LIBRARY_NAME}_opencv)
        target_include_directories(${LIB_NAME} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
                )
        set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
    endif ()

    if (USE_REALSENSE)
        # EIGEN_REALSENSE_UTILS
        set(LIB_NAME "${LIBRARY_NAME}_eigen_realsense")
        add_library(${LIB_NAME} ${EIGEN_REALSENSE_UTILS})
        target_link_libraries(${LIB_NAME} ${LIBRARY_NAME}_eigen ${LIBRARY_NAME}_realsense)
        target_include_directories(${LIB_NAME} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
                )
        set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
    endif ()
endif ()

# UTILS
set(UTILS
        ${GENERAL_UTILS}
        ${EIGEN_UTILS}
        ${EIGEN_JSON_UTILS}
        ${EIGEN_OPENCV_UTILS}
        ${EIGEN_REALSENSE_UTILS}
        ${JSON_UTILS}
        ${OPENCV_UTILS}
        ${OPENCV_REALSENSE_UTILS}
        ${OPENMP_UTILS}
        ${PTHREAD_UTILS}
        ${PYTHON_UTILS}
        ${REALSENSE_UTILS}
        )
message("Utils = ${UTILS}")

append(${LIBRARY_NAME} UTILS_USE_PROJECT)
set(TARGET_LIBRARY ${UTILS_USE_PROJECT})
message("The target name is ${TARGET_LIBRARY}")

set(LIB_NAME ${TARGET_LIBRARY})
add_library(${LIB_NAME} ${UTILS})
target_link_libraries(${LIB_NAME} ${ANDREI_UTILS_REQUIRED_LIBS})
# needed for cmake to correctly generate AndreiUtilsConfig.cmake to include the include directory of the libary :)
target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})
set(ANDREI_UTILS_REQUIRED_LIBS ${LIB_NAME} ${ANDREI_UTILS_REQUIRED_LIBS})

message(${ANDREI_UTILS_REQUIRED_LIBS})
add_executable(TestAndreiUtils main.cpp)
target_link_libraries(TestAndreiUtils ${ANDREI_UTILS_REQUIRED_LIBS})

include(cmake/installConfig.cmake)
