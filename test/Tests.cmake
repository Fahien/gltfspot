include(CTest)

# Download models for testing
function(download_model MODEL_PATH)
	set(URL "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/${MODEL_PATH}")
	file(DOWNLOAD
	     ${URL}
	    "${CMAKE_CURRENT_SOURCE_DIR}/test/model/${MODEL_PATH}")
	message(STATUS "Downloading ${URL}")
endfunction()


# Box
download_model(Box/glTF/Box.gltf)
download_model(Box/glTF/Box0.bin)
# Box Interleaved
download_model(BoxInterleaved/glTF/BoxInterleaved.gltf)
download_model(BoxInterleaved/glTF/BoxInterleaved.bin)
# Box Textured
download_model(BoxTextured/glTF/BoxTextured.gltf)
download_model(BoxTextured/glTF/BoxTextured0.bin)
download_model(BoxTextured/glTF/CesiumLogoFlat.png)


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
