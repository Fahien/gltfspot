option(FORCE_DOWNLOAD FALSE)

# Download models for testing
macro(download_model MODEL_PATH)
	set(OUT "${GLTF_OUT}/${MODEL_PATH}")
	if("${OUT}" MATCHES ".gltf$")
		list(APPEND GLTF_FILES ${OUT})
	endif()
	# Check whether the file has already been downloaded
	set(FILE_DOES_NOT_EXIST TRUE)
	if(EXISTS ${OUT})
		set(FILE_DOES_NOT_EXIST FALSE)
	endif()
	if(${FORCE_DOWNLOAD} OR ${FILE_DOES_NOT_EXIST})
		set(URL "https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/master/2.0/${MODEL_PATH}")
		message(STATUS "Downloading ${URL}")
		file(DOWNLOAD ${URL} ${OUT})
	endif()
endmacro()

if(NOT GLTF_OUT)
	set(GLTF_OUT ${CMAKE_CURRENT_SOURCE_DIR}/test/model)
endif()

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
# Duck
download_model(Duck/glTF/Duck.gltf)
download_model(Duck/glTF/Duck0.bin)
download_model(Duck/glTF/DuckCM.png)
