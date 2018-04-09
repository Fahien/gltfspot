cmake_minimum_required(VERSION 3.9)

# MathSpoT
set(MATHSPOT mathspot)
if("${PROJECT_SOURCE_DIR}" STREQUAL "")
    set(PROJECT_SOURCE_DIR ".")
endif()
set(MATHSPOT_DIR ${PROJECT_SOURCE_DIR}/${MATHSPOT})
message(STATUS ${MATHSPOT_DIR})
if(NOT TARGET ${MATHSPOT})
    add_subdirectory(${MATHSPOT_DIR})
endif()
set(MATHSPOT_INCLUDE_DIR ${MATHSPOT_DIR}/include)

# GltfSpoT
set(GST_TITLE GltfSpoT)
project(${GST_TITLE})

set(CMAKE_C_STANDARD   14)
set(CMAKE_CXX_STANDARD 14)

# Version number
set(GST_VERSION_MAJOR 0)
set(GST_VERSION_MINOR 1)
set(GST_NAME gltfspot)

option(FORCE_DOWNLOAD FALSE)

# Niels Lohmann's JSON for Modern C++
set(OUT ${CMAKE_CURRENT_SOURCE_DIR}/include/nlohmann/json.hpp)
# Check whether the file has already been downloaded
set(FILE_DOES_NOT_EXIST FALSE)
if(NOT EXISTS ${OUT})
	set(FILE_DOES_NOT_EXIST TRUE)
endif()
if(${FORCE_DOWNLOAD} OR ${FILE_DOES_NOT_EXIST})
	set(URL "https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp")
	message(STATUS "Downloading ${URL}")
	file(DOWNLOAD ${URL} ${OUT})
endif()

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
target_link_libraries(${GST_NAME} ${MATHSPOT})
target_include_directories(${GST_NAME} PUBLIC ${MATHSPOT_INCLUDE_DIR})

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