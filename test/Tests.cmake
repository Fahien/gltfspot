include(CTest)

# Download a box model for testing
file(DOWNLOAD
     https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/Box/glTF/Box.gltf
     ${CMAKE_CURRENT_SOURCE_DIR}/test/model/box/Box.gltf)
file(DOWNLOAD
     https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/Box/glTF/Box0.bin
     ${CMAKE_CURRENT_SOURCE_DIR}/test/model/box/Box0.bin)

function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${GST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})
	
	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${TEST_NAME} ${GST_NAME})
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME})
endfunction(make_test)

make_test(gst-test)
