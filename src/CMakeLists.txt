CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (Prototype3D)

set (P3D_VERSION_MAJOR 0)
set (P3D_VERSION_MINOR 1)
set (P3D_VERSION_PATCH 0)


IF(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /WX /arch:AVX /Ox /Ob2 /Ot /Gt /Zc:implicitNoexcept-")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /WX /arch:AVX /Od /Ob0 /Zc:implicitNoexcept-")
ENDIF(MSVC)

# Configuration GUI goes here


# =====================================================================================

#                                     Global includes

# =====================================================================================

SET(HEADERS       
    util/p3d_GlfwWindowManager.h
    util/p3d_Timer.h	
	)

SET(SOURCES 
    util/p3d_GlfwWindowManager.cpp

    p3dTest.cpp
	)


# GLFW
SET(NLE_GLFW_INCLUDE_PATH
	"" CACHE PATH "NLE GLFW include path")

IF(NLE_GLFW_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE GLFW include path must be specified")
ENDIF(NLE_GLFW_INCLUDE_PATH STREQUAL "")

SET(NLE_GLFW_LIBRARY_DEBUG_PATH
	"" CACHE PATH "NLE GLFW library debug path")

IF(NLE_GLFW_LIBRARY_DEBUG_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE GLFW library debug path must be specified")
ENDIF(NLE_GLFW_LIBRARY_DEBUG_PATH STREQUAL "")

SET(NLE_GLFW_LIBRARY_RELEASE_PATH
	"" CACHE PATH "NLE GLFW library release path")

IF(NLE_GLFW_LIBRARY_RELEASE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE GLFW library release path must be specified")
ENDIF(NLE_GLFW_LIBRARY_RELEASE_PATH STREQUAL "")



# D3D11
SET(NLE_D3D11_INCLUDE_PATH
"" CACHE PATH "NLE D3D11 include path")

IF(NLE_D3D11_INCLUDE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "NLE D3D11 include path must be specified")
ENDIF(NLE_D3D11_INCLUDE_PATH STREQUAL "")

SET(NLE_D3D11_LIBRARY_DEBUG_PATH
    "" CACHE PATH "NLE D3D11 library debug path")

IF(NLE_D3D11_LIBRARY_DEBUG_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "NLE D3D11 library debug path must be specified")
ENDIF(NLE_D3D11_LIBRARY_DEBUG_PATH STREQUAL "")

SET(NLE_D3D11_LIBRARY_RELEASE_PATH
    "" CACHE PATH "NLE D3D11 library release path")

IF(NLE_D3D11_LIBRARY_RELEASE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "NLE D3D11 library release path must be specified")
ENDIF(NLE_D3D11_LIBRARY_RELEASE_PATH STREQUAL "")




SET(HEADERS
		${HEADERS}
		#${NLE_IMGUI_INCLUDE_PATH}/imgui.h
		#${NLE_IMGUI_INCLUDE_PATH}/imconfig.h
		#${NLE_IMGUI_INCLUDE_PATH}/imgui_internal.h
		#${NLE_IMGUI_INCLUDE_PATH}/stb_rect_pack.h
		#${NLE_IMGUI_INCLUDE_PATH}/stb_textedit.h
		#${NLE_IMGUI_INCLUDE_PATH}/stb_truetype.h
		#${P3D_DIRECT_X_TEX_INCLUDE_PATH}/DDSTextureLoader/DDSTextureLoader.h
		#${P3D_DIRECT_X_TEX_INCLUDE_PATH}/WICTextureLoader/WICTextureLoader.h
	)

	SET(SOURCES 
		${SOURCES}
		#${NLE_IMGUI_INCLUDE_PATH}/imgui.cpp
		#${NLE_IMGUI_INCLUDE_PATH}/imgui_demo.cpp
		#${NLE_IMGUI_INCLUDE_PATH}/imgui_draw.cpp
		#${P3D_DIRECT_X_TEX_INCLUDE_PATH}/DDSTextureLoader/DDSTextureLoader.cpp
		#${P3D_DIRECT_X_TEX_INCLUDE_PATH}/WICTextureLoader/WICTextureLoader.cpp
	)


INCLUDE_DIRECTORIES(
	${NLE_GLFW_INCLUDE_PATH}
    ${NLE_D3D11_INCLUDE_PATH}
	)

#ADD_DEFINITIONS(

#	)

LINK_DIRECTORIES(
	${NLE_GLFW_LIBRARY_DEBUG_PATH}
	${NLE_GLFW_LIBRARY_RELEASE_PATH}
    ${NLE_D3D11_LIBRARY_DEBUG_PATH}
	${NLE_D3D11_LIBRARY_RELEASE_PATH}
	)

add_executable (P3D
  ${HEADERS}
  ${SOURCES}
)

TARGET_LINK_LIBRARIES(P3D 
	debug glfw3
	optimized glfw3

    debug d3d11.lib
	optimized d3d11.lib
	debug d3dcompiler.lib
	optimized d3dcompiler.lib
	)

set_target_properties(P3D PROPERTIES DEBUG_POSTFIX _d)