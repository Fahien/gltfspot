include( ${CMAKE_CURRENT_SOURCE_DIR}/GltfFiles.cmake )

# Sources
set( TEST_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/test-gltf.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/handle.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
)
set( TEST_NAME test-${PROJECT_NAME} )
source_group( ${TEST_NAME} FILES ${TEST_SOURCES} )

# Executable
add_executable( ${TEST_NAME} ${TEST_SOURCES} )
target_include_directories( ${TEST_NAME} PRIVATE ${GST_INCLUDE_DIRS} )
target_link_libraries( ${TEST_NAME} ${PROJECT_NAME} )
target_compile_features( ${TEST_NAME} PUBLIC cxx_std_17 )


include( CTest )
include( Catch )
catch_discover_tests( ${TEST_NAME} )


# Tests
#foreach( GLTF ${GLTF_FILES} )
#	# Extract model name
#	string( FIND ${GLTF} "/" LAST_SLASH REVERSE )
#	string( FIND ${GLTF} "." LAST_DOT REVERSE )
#	math( EXPR LAST_SLASH "${LAST_SLASH} + 1" )
#	math( EXPR SUB_LENGTH "${LAST_DOT} - ${LAST_SLASH}" )
#	string( SUBSTRING ${GLTF} ${LAST_SLASH} ${SUB_LENGTH} PROJECT_NAME )
#	# Add test
#	add_test( test-gltf-${PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/test-gltf ${GLTF} )
#endforeach()
