include(CTest)

include(${CMAKE_CURRENT_SOURCE_DIR}/test/GltfFiles.cmake)


function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${GST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})
	
	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	target_include_directories(${TEST_NAME} PRIVATE ${GST_INCLUDE_DIRS})
	target_link_libraries(${TEST_NAME} ${GST_NAME})

	# Tests
	foreach(GLTF ${GLTF_FILES})
		# Extract model name
		string(FIND ${GLTF} "/" LAST_SLASH REVERSE)
		string(FIND ${GLTF} "." LAST_DOT REVERSE)
		math(EXPR LAST_SLASH "${LAST_SLASH} + 1")
		math(EXPR SUB_LENGTH "${LAST_DOT} - ${LAST_SLASH}")
		string(SUBSTRING ${GLTF} ${LAST_SLASH} ${SUB_LENGTH} GLTF_NAME)
		# Add test
		add_test(${TEST_NAME}-${GLTF_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME} ${GLTF})
	endforeach()
endfunction(make_test)

make_test(gst-test)
