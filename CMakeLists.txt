cmake_minimum_required(VERSION 3.16)

project(AndreiUtils)  # sets PROJECT_NAME variable
set(PROJECT_VERSION_STRING "1.0.0")
set(PROJECT_TARGET ${PROJECT_NAME})
set(PROJECT_INCLUDE_PREFIX ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif ()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASE)
if (${CMAKE_BUILD_TYPE_LOWERCASE} MATCHES release)
    SET(CMAKE_CXX_FLAGS "-O2")
endif ()

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

function(AndreiUtilsAddTarget targetName targetSources targetLinkDependencies)
    # message("In AndreiUtilsAddTarget: targetName = ${targetName}")
    # message("In AndreiUtilsAddTarget: targetSources = ${${targetSources}}")
    # message("In AndreiUtilsAddTarget: targetLinkDependencies = ${${targetLinkDependencies}}")
    set(targetCompileOption)
    if ((${ARGC} EQUAL 4) AND (${ARGV3}))
        set(targetCompileOptions ${ARGV3})
        # message("In AndreiUtilsAddTarget: targetCompileOptions = ${${targetCompileOptions}}")
    endif ()
    add_library(${targetName} ${${targetSources}})
    target_link_libraries(${targetName} ${${targetLinkDependencies}})
    # needed for cmake to correctly generate AndreiUtilsConfig.cmake to include the include directory of the libary :)
    target_include_directories(${targetName} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
    if (NOT "${${targetCompileOptions}}" STREQUAL "")
        target_compile_options(${targetName} PUBLIC ${${targetCompileOptions}})
    endif ()
    if (${USE_OPENMP})
        target_compile_definitions(${targetName} PRIVATE WITH_OPENMP)
    endif ()
    set(ALL_INSTALL_TARGETS ${targetName} ${ALL_INSTALL_TARGETS})
    set(ALL_INSTALL_TARGETS ${ALL_INSTALL_TARGETS} PARENT_SCOPE)
    # message(STATUS "All install targets = ${ALL_INSTALL_TARGETS}")
endfunction()

option(WITH_ALL "Build the code with all extensions" OFF)
option(WITH_EIGEN "Build the code with eigen" OFF)
option(WITH_JSON "Build the code with json" ON)
option(WITH_OPENCV "Build the code with opencv" OFF)
option(WITH_OPENMP "Build the code with openmp" OFF)
option(WITH_PTHREAD "Build the code with pthread" OFF)
option(WITH_PYTHON "Build the code with pybind" OFF)
option(WITH_REALSENSE "Build the code with librealsense" OFF)
option(WITH_PCL_OPTIMIZATION_FLAGS "Use optimization flags used by PCL to compile a compatible interface to EIGEN types" ON)

set(EIGEN_VERSION "3.4" CACHE STRING "The eigen version to use in the project")
set(OPENCV_VERSION "4" CACHE STRING "The opencv version to use in the project")
set(PYBIND11_PYTHON_VERSION "3.7" CACHE STRING "The python version to use in the project")
set(PCL_OPTIMIZATION_FLAGS)

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
message("Use python set to ${USE_PYTHON}")
message("Use realsense set to ${USE_REALSENSE}")

if (WIN32)
    set(ANDREI_UTILS_REQUIRED_LIBS wsock32 ws2_32 ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_GENERAL_REQUIRED_LIBS wsock32 ws2_32 ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
else (WIN32)
    set(ANDREI_UTILS_REQUIRED_LIBS pthread ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_GENERAL_REQUIRED_LIBS pthread ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
endif (WIN32)

if (USE_EIGEN)
    if ("${EIGEN_VERSION}" STREQUAL "")
        find_package(Eigen3 REQUIRED)
    else ()
        find_package(Eigen3 ${EIGEN_VERSION} REQUIRED)
    endif ()
    set(ANDREI_UTILS_REQUIRED_LIBS Eigen3::Eigen ${ANDREI_UTILS_REQUIRED_LIBS})
    set(ANDREI_UTILS_EIGEN_REQUIRED_LIBS Eigen3::Eigen ${ANDREI_UTILS_EIGEN_REQUIRED_LIBS})

    if (${WITH_PCL_OPTIMIZATION_FLAGS})
        find_package(PCL REQUIRED COMPONENTS common)
        get_target_property(OUTPUT pcl_common INTERFACE_COMPILE_OPTIONS)
        set(PCL_OPTIMIZATION_FLAGS ${OUTPUT})
        message(STATUS "PCL optimization flags = ${PCL_OPTIMIZATION_FLAGS}")
    endif ()
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

SET(UTILS_EXCEPTIONS src/exceptions/DebugRequestedException.cpp src/exceptions/QuitRequestedException.cpp)

SET(UTILS_GEOMETRY src/utilsGeometry.cpp)

SET(UTILS_IMAGES src/utilsImages.cpp)

SET(UTILS_NETWORK src/NetworkIncludes.cpp src/NetworkData.cpp)

# depends on utils.hpp
SET(UTILS_STRINGS src/utilsString.cpp)

SET(UTILS_THREAD src/utilsThread.cpp)

SET(UTILS_TIME src/utilsTime.cpp)

# depends on UTILS_STRINGS (utilsStrings.h)
SET(UTILS_FILES src/utilsFiles.cpp)

# depends on UTILS_FILES (utilsStrings.h)
set(UTILS_SERIALIZATION src/utilsBinarySerialization.cpp)

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
        src/classes/AnyType.cpp
        src/classes/Buffer.cpp
        src/classes/GaussianKernel.cpp
        src/classes/IndexIntervalSeries.cpp
        src/classes/MixedDataContainer.cpp
        src/classes/RandomNumberGenerator.cpp
        src/classes/Timer.cpp
        src/classes/UnionFind.cpp
        src/classes/UserInteraction.cpp
        ${UTILS_CLASSES_CAMERA}
        ${UTILS_CLASSES_GRAPH}
        )

SET(UTILS_EIGEN)
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
            src/utilsEigen.cpp src/utilsEigenGeometry.cpp src/utilsEigenLeastSquares.cpp src/utilsEigenSVD.cpp
            src/classes/motion/MotionDeviceIntrinsicParameters.cpp src/classes/motion/MotionDeviceCaptureParameters.cpp
            src/classes/trajectory/TrajectoryBase.cpp src/classes/trajectory/CartesianTrajectory.cpp
            src/classes/trajectory/CartesianTrajectoryFilter.cpp src/classes/trajectory/PoseDecoupledLowPassTrajectoryFilter.cpp
            src/classes/Symmetry.cpp
            src/enums/EigenLeastSquaresSolver.cpp src/enums/EigenSVDSolver.cpp)
endif ()

if (USE_EIGEN AND USE_OPENCV)
    # depends on eigen and opencv
    SET(UTILS_EIGEN_OPENCV src/utilsEigenOpenCV.cpp)
endif ()

if (USE_JSON)
    # depends on json
    SET(UTILS_JSON src/utilsJson.cpp src/classes/ConfigurationParameters.cpp ${UTILS_JSON})
endif ()

if (USE_OPENCV)
    # depends on opencv
    SET(UTILS_OPENCV src/utilsOpenCV.cpp)
endif ()

if (USE_OPENCV AND USE_REALSENSE)
    SET(UTILS_OPENCV_REALSENSE src/utilsOpenCVRealsense.cpp)
endif ()

if (USE_OPENMP)
    SET(UTILS_OPENMP src/utilsOpenMP.cpp)
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
        ${UTILS_EXCEPTIONS}
        ${UTILS_GEOMETRY}
        ${UTILS_IMAGES}

        ${UTILS_NETWORK}
        ${UTILS_STRINGS}
        ${UTILS_THREAD}
        ${UTILS_TIME}

        ${UTILS_FILES}
        ${UTILS_SERIALIZATION}

        ${UTILS_CAMERA}

        ${UTILS_CLASSES}
        )
set(EIGEN_UTILS ${UTILS_EIGEN})
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

if (USE_OPENMP)
    # OPENMP_UTILS
    AndreiUtilsAddTarget("${LIBRARY_NAME}_openmp" OPENMP_UTILS ANDREI_UTILS_OPENMP_REQUIRED_LIBS)
endif ()

# GENERAL_UTILS
set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_GENERAL_REQUIRED_LIBS})
if (${USE_OPENMP})
    set(LIBRARY_LINK_TARGETS ${LIBRARY_NAME}_openmp ${LIBRARY_LINK_TARGETS})
endif ()
AndreiUtilsAddTarget("${LIBRARY_NAME}_core" GENERAL_UTILS LIBRARY_LINK_TARGETS)

if (USE_JSON)
    # JSON_UTILS
    set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_JSON_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_json" JSON_UTILS LIBRARY_LINK_TARGETS)
endif ()

if (USE_PTHREAD)
    # PTHREAD_UTILS
    set(LIBRARY_LINK_TARGETS ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_pthread" PTHREAD_UTILS LIBRARY_LINK_TARGETS)
endif ()

if (USE_PYTHON)
    # PYTHON_UTILS
    set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_PYTHON_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_python" PYTHON_UTILS LIBRARY_LINK_TARGETS)
endif ()

if (USE_REALSENSE)
    # REALSENSE_UTILS
    set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_REALSENSE_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_realsense" REALSENSE_UTILS LIBRARY_LINK_TARGETS)
endif ()

if (USE_OPENCV)
    # OPENCV_UTILS
    set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_OPENCV_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_opencv" OPENCV_UTILS LIBRARY_LINK_TARGETS)

    if (USE_REALSENSE)
        # OPENCV_REALSENSE_UTILS
        set(LIBRARY_LINK_TARGETS ${LIBRARY_NAME}_realsense ${LIBRARY_NAME}_opencv)
        AndreiUtilsAddTarget("${LIBRARY_NAME}_opencv_realsense" OPENCV_REALSENSE_UTILS LIBRARY_LINK_TARGETS)
    endif ()
endif ()

if (USE_EIGEN)
    # EIGEN_UTILS
    set(LIBRARY_LINK_TARGETS ${ANDREI_UTILS_EIGEN_REQUIRED_LIBS} ${LIBRARY_NAME}_core)
    AndreiUtilsAddTarget("${LIBRARY_NAME}_eigen" EIGEN_UTILS LIBRARY_LINK_TARGETS PCL_OPTIMIZATION_FLAGS)

    if (USE_OPENCV)
        # EIGEN_OPENCV_UTILS
        set(LIBRARY_LINK_TARGETS ${LIBRARY_NAME}_eigen ${LIBRARY_NAME}_opencv)
        AndreiUtilsAddTarget("${LIBRARY_NAME}_eigen_opencv" EIGEN_OPENCV_UTILS LIBRARY_LINK_TARGETS PCL_OPTIMIZATION_FLAGS)
    endif ()

    if (USE_REALSENSE)
        # EIGEN_REALSENSE_UTILS
        set(LIBRARY_LINK_TARGETS ${LIBRARY_NAME}_eigen ${LIBRARY_NAME}_realsense)
        AndreiUtilsAddTarget("${LIBRARY_NAME}_eigen_realsense" EIGEN_REALSENSE_UTILS LIBRARY_LINK_TARGETS PCL_OPTIMIZATION_FLAGS)
    endif ()
endif ()

# UTILS
set(UTILS
        ${GENERAL_UTILS}
        ${EIGEN_UTILS}
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

# used when AndreiUtils is included as a submodule/subproject/subdirectory
append(${LIBRARY_NAME} UTILS_USE_PROJECT)
set(TARGET_LIBRARY ${UTILS_USE_PROJECT})
message("The target name is ${TARGET_LIBRARY}")

AndreiUtilsAddTarget("${TARGET_LIBRARY}" UTILS ANDREI_UTILS_REQUIRED_LIBS PCL_OPTIMIZATION_FLAGS)
set(ANDREI_UTILS_REQUIRED_LIBS ${TARGET_LIBRARY} ${ANDREI_UTILS_REQUIRED_LIBS})

if (${USE_OPENCV})
    message(${ANDREI_UTILS_REQUIRED_LIBS})
    set(EXECUTABLE TestAndreiUtils)
    add_executable(${EXECUTABLE} main.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()
endif ()

if (${USE_PYTHON})
    set(EXECUTABLE TestAndreiUtilsPython)
    add_executable(${EXECUTABLE} testPythonFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()
endif ()

set(EXECUTABLE TestAndreiUtilsJson)
add_executable(${EXECUTABLE} testJsonFunctions.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestAndreiUtilsMap)
add_executable(${EXECUTABLE} testMapFunctions.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

if (${USE_EIGEN})
    set(EXECUTABLE TestAndreiUtilsEigen)
    add_executable(${EXECUTABLE} testEigenFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()

    set(EXECUTABLE TestAndreiUtilsVector)
    add_executable(${EXECUTABLE} testVectorFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()

    set(EXECUTABLE TestAndreiUtilsDualQuaternion)
    add_executable(${EXECUTABLE} testDualQuaternionFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()

    set(EXECUTABLE TestAndreiUtilsSuperCube)
    add_executable(${EXECUTABLE} testSuperCubeFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()

    if (${USE_JSON})
        set(EXECUTABLE TestAndreiUtilsJsonEigen)
        add_executable(${EXECUTABLE} testJsonEigenFunctions.cpp)
        target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
        if (${USE_OPENMP})
            target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
        endif ()
    endif ()
endif ()

set(EXECUTABLE TestAndreiUtilsTime)
add_executable(${EXECUTABLE} testTimeFunctions.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestAndreiUtilsMove)
add_executable(${EXECUTABLE} testMoveFunctions.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

if (${USE_OPENMP})
    set(EXECUTABLE TestAndreiUtilsOpenMP)
    add_executable(${EXECUTABLE} testOpenMPFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

if (${USE_OPENCV})
    set(EXECUTABLE TestAndreiUtilsOpenCV)
    add_executable(${EXECUTABLE} testOpenCVFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()
endif ()

if (${USE_REALSENSE})
    set(EXECUTABLE TestAndreiUtilsRealsense)
    add_executable(${EXECUTABLE} testRealsenseFunctions.cpp)
    target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
    if (${USE_OPENMP})
        target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
    endif ()
endif ()

set(EXECUTABLE TestSmartPointers)
add_executable(${EXECUTABLE} testSmartPointers.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestDiamondInheritance)
add_executable(${EXECUTABLE} testDiamondInheritance.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestTypeTraits)
add_executable(${EXECUTABLE} testTypeTraits.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestVirtualInheritance)
add_executable(${EXECUTABLE} testVirtualInheritance.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

set(EXECUTABLE TestBinarySerialization)
add_executable(${EXECUTABLE} testSerialization.cpp)
target_link_libraries(${EXECUTABLE} ${ANDREI_UTILS_REQUIRED_LIBS})
if (${USE_OPENMP})
    target_compile_definitions(${EXECUTABLE} PRIVATE WITH_OPENMP)
endif ()

include(cmake/installConfig.cmake)
