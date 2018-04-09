# Download models for testing
macro(download_model MODEL_PATH)
	set(OUT "${OUT_DIR}/${MODEL_PATH}")
	if("${OUT}" MATCHES ".gltf$")
		list(APPEND GLTF_FILES ${OUT})
	endif()
	# Check whether the file has already been downloaded
	if(NOT EXISTS ${OUT})
		set(FILE_DOES_NOT_EXIST TRUE)
	endif()
	if(${FORCE_DOWNLOAD} OR ${FILE_DOES_NOT_EXIST})
		set(URL "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/${MODEL_PATH}")
		message(STATUS "Downloading ${URL}")
		file(DOWNLOAD ${URL} ${OUT})
	endif()
endmacro()

set(OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/model)

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
