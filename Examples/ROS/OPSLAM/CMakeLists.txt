cmake_minimum_required(VERSION 3.0.2)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)


IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3  -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")


# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++11.")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    add_definitions(-DCOMPILEDWITHC0X)
    message(STATUS "Using flag -std=c++0x.")
else ()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()


#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_EXTENSIONS OFF)
#add_definitions(-DCOMPILENDWITHC11)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake_modules)

find_package(OpenCV 4.5.0 QUIET)
if (NOT OpenCV_FOUND)
    find_package(OpenCV 2.4.3 QUIET)
    if (NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
    endif ()
endif ()




message(${OpenCV_VERSION})

find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  cv_bridge
  image_transport
  message_filters
  roscpp
  sensor_msgs
  std_msgs
)

#find_package(Eigen3 3.1.0 REQUIRED)
find_package(Pangolin REQUIRED)
find_package(PCL 1.9 REQUIRED)
find_package(Boost COMPONENTS system)


include_directories("/usr/include/eigen3")
include_directories(
        ${PROJECT_SOURCE_DIR}/../../../
        ${PROJECT_SOURCE_DIR}/../../../include
        ${PROJECT_SOURCE_DIR}/../../../g2oAddition
        #${EIGEN3_INCLUDE_DIR}
        ${Pangolin_INCLUDE_DIRS}
        ${PCL_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}

)

#include_directories(../include/peac)
add_definitions(${PCL_DEFINITIONS})
link_directories(${PCL_LIBRARY_DIRS})



set(LIBS
        ${OpenCV_LIBS}
        ${EIGEN3_LIBS}
        ${Pangolin_LIBRARIES}
        ${PROJECT_SOURCE_DIR}/../../../Thirdparty/DBoW2/lib/libDBoW2.so
        ${PROJECT_SOURCE_DIR}/../../../Thirdparty/g2o/lib/libg2o.so
        ${PROJECT_SOURCE_DIR}/../../../lib/libPlanarSLAM.so
        ${PCL_LIBRARIES}
        ${Boost_LIBRARIES}
)

# Build examples
add_executable(Planar_ros src/main_ros.cc)
target_link_libraries(Planar_ros ${LIBS} ${catkin_LIBRARIES})











