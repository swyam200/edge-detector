cmake_minimum_required(VERSION 3.4.1)
project(native-lib)

add_library(native-lib SHARED processor.cpp)

# Find OpenCV (assumes OpenCV Android SDK is placed under project-root/opencv)
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/../../../../opencv/sdk/native/jni)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(native-lib ${OpenCV_LIBS} log)
