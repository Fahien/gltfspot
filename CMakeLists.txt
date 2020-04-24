cmake_minimum_required( VERSION 3.15 )
project( gltfspot
	VERSION 0.3
	LANGUAGES C CXX )

# CMake modules
list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
include( AddMathSpot )
include( AddFileSpot )

option( FORCE_DOWNLOAD
	"Force downloading again gltf models"
	FALSE )

# Niels Lohmann's JSON for Modern C++
set( OUT ${CMAKE_CURRENT_SOURCE_DIR}/include/nlohmann/json.hpp )
# Check whether the file has already been downloaded
set( FILE_DOES_NOT_EXIST FALSE )
if( NOT EXISTS ${OUT} )
	set( FILE_DOES_NOT_EXIST TRUE )
endif()
if( ${FORCE_DOWNLOAD} OR ${FILE_DOES_NOT_EXIST} )
	set( URL "https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp" )
	message( STATUS "Downloading ${URL}" )
	file( DOWNLOAD ${URL} ${OUT} )
endif()

# Includes
set( GST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
set( GST_INCLUDE_DIRS ${GST_INCLUDE_DIR}
	${MATHSPOT_INCLUDE_DIRS}
	${FILESPOT_INCLUDE_DIRS} )

# Sources
set( GST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( GST_SOURCES
	${GST_SOURCE_DIR}/gltf.cc
	${GST_SOURCE_DIR}/mesh.cc
	${GST_SOURCE_DIR}/node.cc
	${GST_SOURCE_DIR}/bounds.cc
	${GST_SOURCE_DIR}/animation.cc
)
source_group( src FILES ${GST_SOURCES} )

find_package( Vulkan )

# Library
add_library( ${PROJECT_NAME} ${GST_SOURCES} )
target_link_libraries( ${PROJECT_NAME}
	${MATHSPOT_LIBRARIES}
	${FILESPOT_LIBRARIES}
	${Vulkan_LIBRARIES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${GST_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS} )
target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_17 )

# Test
set( GST_TEST_DIR ${PROJECT_SOURCE_DIR}/test )
include( ${GST_TEST_DIR}/tests.cmake )
