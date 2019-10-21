cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD   14)
set(CMAKE_CXX_STANDARD 14)

# GltfSpot
set(GST_TITLE GltfSpot)
project(${GST_TITLE} C CXX)

# Version number
set(GST_VERSION_MAJOR 0)
set(GST_VERSION_MINOR 2)
set(GST_NAME gltfspot)

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(AddMathSpot)
include(AddFileSpot)

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
set(GST_INCLUDE_DIRS ${GST_INCLUDE_DIR}
	${MATHSPOT_INCLUDE_DIRS}
	${FILESPOT_INCLUDE_DIRS})

# Sources
set(GST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(GST_SOURCES
	${GST_SOURCE_DIR}/Gltf.cpp
	${GST_SOURCE_DIR}/Mesh.cpp
	${GST_SOURCE_DIR}/Node.cpp
	${GST_SOURCE_DIR}/Bounds.cpp
)
source_group(Source FILES ${GST_SOURCES})

# Library
add_library(${GST_NAME} ${GST_SOURCES})
target_link_libraries(${GST_NAME}
	${MATHSPOT_LIBRARIES}
	${FILESPOT_LIBRARIES})
target_include_directories(${GST_NAME} PUBLIC ${GST_INCLUDE_DIRS})

# Test
set(GST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include(${GST_TEST_DIR}/Tests.cmake)
