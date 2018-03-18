cmake_minimum_required(VERSION 3.9)
set(GST_TITLE GltfSpoT)
project(${GST_TITLE})

set(CMAKE_C_STANDARD   14)
set(CMAKE_CXX_STANDARD 14)

# Version number
set(GST_VERSION_MAJOR 0)
set(GST_VERSION_MINOR 1)
set(GST_NAME gltfspot)

# TODO Add LOGSPOT

# Download the latest Niels Lohmann's JSON for Modern C++
file(DOWNLOAD
     https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/include/nlohmann/json.hpp)

# Includes
set(GST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${GST_INCLUDE_DIR})

# Sources
set(GST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(GST_SOURCES ${GST_SOURCE_DIR}/Gltf.cpp)
source_group(Source FILES ${GST_SOURCES})

# Library
add_library(${GST_NAME} ${GST_SOURCES})
set_target_properties(${GST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Test
set(GST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include(${GST_TEST_DIR}/Tests.cmake)

# Documentation
find_package(Doxygen REQUIRED)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
set(DOXYGEN_RECURSIVE NO)
doxygen_add_docs(doxygen
                 "${GST_INCLUDE_DIR};${GST_SOURCE_DIR}"
                 COMMENT "Generating documentation")