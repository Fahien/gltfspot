include(CTest)

# Download models for testing
macro(download_model MODEL_PATH)
	set(URL "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/${MODEL_PATH}")
	set(OUT "${CMAKE_CURRENT_SOURCE_DIR}/test/model/${MODEL_PATH}")
	message(STATUS "Downloading ${URL}")
	file(DOWNLOAD ${URL} ${OUT})
	if("${OUT}" MATCHES ".gltf$")
		list(APPEND GLTF_FILES ${OUT})
	endif()
endmacro()

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
# Box Vertex colors
download_model(BoxVertexColors/glTF/BoxVertexColors.gltf)
download_model(BoxVertexColors/glTF/buffer.bin)


function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${GST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})
	
	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
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
