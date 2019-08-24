CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (Prototype3D)

set (P3D_VERSION_MAJOR 0)
set (P3D_VERSION_MINOR 1)
set (P3D_VERSION_PATCH 0)

set (CMAKE_CXX_STANDARD 11)

IF(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /WX /arch:AVX /Ox /Ob2 /Ot /Gt /Zc:implicitNoexcept-")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /WX /arch:AVX /Od /Ob0 /Zc:implicitNoexcept- /DP3D_DEBUG")
ENDIF(MSVC)

# Configuration GUI goes here


# =====================================================================================

#                                     Global includes

# =====================================================================================

SET(HEADERS    
    p3d/common/Uuid.h   

    p3d/renderer/d3d11/d3d11_ConstConvert.h
    p3d/renderer/d3d11/d3d11_PixelShader.h
    p3d/renderer/d3d11/d3d11_Renderer.h
    p3d/renderer/d3d11/d3d11_Texture1dArray.h
    p3d/renderer/d3d11/d3d11_Texture2dArray.h
    p3d/renderer/d3d11/d3d11_Texture3d.h
    p3d/renderer/d3d11/d3d11_Utility.h
    p3d/renderer/d3d11/d3d11_VertexShader.h
    
    p3d/renderer/dx/dx_ComPtr.h
    p3d/renderer/dx/dx_ConstConvert.h
    
    p3d/renderer/GPUResourceProviderI.h
    p3d/renderer/MaterialI.h
    p3d/renderer/MeshI.h
    p3d/renderer/PixelShaderI.h
    p3d/renderer/RendererI.h
    p3d/renderer/Texture1dArrayI.h
    p3d/renderer/Texture2dArrayI.h
    p3d/renderer/Texture3dI.h
    p3d/renderer/VertexShaderI.h

    p3d/scene/NullSpacePartitioner.h
    p3d/scene/SpacePartitionerI.h
    p3d/scene/ResourceBank.h
    p3d/scene/SceneI.h
    p3d/scene/Scene.h

    p3d/util/util_GlfwWindowManager.h
    p3d/util/util_DefaultSampleRunner.h
    p3d/util/util_DDSTextureLoader.h
    p3d/util/util_SceneImporter.h
    p3d/util/util_SceneGraphI.h
    
    p3d/Constants.h
    p3d/ResourceDescriptions.h
    p3d/ResourceHandles.h
    p3d/assert.h
    )

SET(SOURCES 
    p3d/renderer/d3d11/d3d11_ConstConvert.cpp
    p3d/renderer/d3d11/d3d11_PixelShader.cpp
    p3d/renderer/d3d11/d3d11_Renderer.cpp
    p3d/renderer/d3d11/d3d11_Texture1dArray.cpp
    p3d/renderer/d3d11/d3d11_Texture2dArray.cpp
    p3d/renderer/d3d11/d3d11_Texture3d.cpp
    p3d/renderer/d3d11/d3d11_Utility.cpp
    p3d/renderer/d3d11/d3d11_VertexShader.cpp

    p3d/renderer/dx/dx_ConstConvert.cpp

    p3d/scene/Scene.cpp

    p3d/util/util_GlfwWindowManager.cpp
    p3d/util/util_DefaultSampleRunner.cpp
    p3d/util/util_DDSTextureLoader.cpp
    p3d/util/util_SceneImporter.cpp

    p3d/Constants.cpp
    )


# GLFW
SET(P3D_GLFW_INCLUDE_PATH
    "" CACHE PATH "P3D GLFW include path")

IF(P3D_GLFW_INCLUDE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D GLFW include path must be specified")
ENDIF(P3D_GLFW_INCLUDE_PATH STREQUAL "")

SET(P3D_GLFW_LIBRARY_DEBUG_PATH
    "" CACHE PATH "P3D GLFW library debug path")

IF(P3D_GLFW_LIBRARY_DEBUG_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D GLFW library debug path must be specified")
ENDIF(P3D_GLFW_LIBRARY_DEBUG_PATH STREQUAL "")

SET(P3D_GLFW_LIBRARY_RELEASE_PATH
    "" CACHE PATH "P3D GLFW library release path")

IF(P3D_GLFW_LIBRARY_RELEASE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D GLFW library release path must be specified")
ENDIF(P3D_GLFW_LIBRARY_RELEASE_PATH STREQUAL "")



# D3D11
SET(P3D_D3D11_INCLUDE_PATH
"" CACHE PATH "P3D D3D11 include path")

IF(P3D_D3D11_INCLUDE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D D3D11 include path must be specified")
ENDIF(P3D_D3D11_INCLUDE_PATH STREQUAL "")

SET(P3D_D3D11_LIBRARY_DEBUG_PATH
    "" CACHE PATH "P3D D3D11 library debug path")

IF(P3D_D3D11_LIBRARY_DEBUG_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D D3D11 library debug path must be specified")
ENDIF(P3D_D3D11_LIBRARY_DEBUG_PATH STREQUAL "")

SET(P3D_D3D11_LIBRARY_RELEASE_PATH
    "" CACHE PATH "P3D D3D11 library release path")

IF(P3D_D3D11_LIBRARY_RELEASE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D D3D11 library release path must be specified")
ENDIF(P3D_D3D11_LIBRARY_RELEASE_PATH STREQUAL "")



# GLM
SET(P3D_GLM_INCLUDE_PATH
    "" CACHE PATH "P3D GLM include path")

IF(P3D_GLM_INCLUDE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D GLM include path must be specified")
ENDIF(P3D_GLM_INCLUDE_PATH STREQUAL "")

# Assimp
SET(P3D_ASSIMP_INCLUDE_PATH
	"" CACHE PATH "P3D Assimp include path")

IF(P3D_ASSIMP_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "P3D Assimp include path must be specified")
ENDIF(P3D_ASSIMP_INCLUDE_PATH STREQUAL "")

SET(P3D_ASSIMP_LIBRARY_DEBUG_PATH
	"" CACHE PATH "P3D Assimp library debug path")

IF(P3D_ASSIMP_LIBRARY_DEBUG_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "P3D Assimp library debug path must be specified")
ENDIF(P3D_ASSIMP_LIBRARY_DEBUG_PATH STREQUAL "")

SET(P3D_ASSIMP_LIBRARY_RELEASE_PATH
	"" CACHE PATH "P3D Assimp library release path")

IF(P3D_ASSIMP_LIBRARY_RELEASE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "P3D Assimp library release path must be specified")
ENDIF(P3D_ASSIMP_LIBRARY_RELEASE_PATH STREQUAL "")


# Cereal
SET(P3D_CEREAL_INCLUDE_PATH
	"" CACHE PATH "P3D Cereal include path")

IF(P3D_CEREAL_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "P3D Cereal include path must be specified")
ENDIF(P3D_CEREAL_INCLUDE_PATH STREQUAL "")


SET(HEADERS
        ${HEADERS}
        #${NLE_IMGUI_INCLUDE_PATH}/imgui.h
        #${NLE_IMGUI_INCLUDE_PATH}/imconfig.h
        #${NLE_IMGUI_INCLUDE_PATH}/imgui_internal.h
        #${NLE_IMGUI_INCLUDE_PATH}/stb_rect_pack.h
        #${NLE_IMGUI_INCLUDE_PATH}/stb_textedit.h
        #${NLE_IMGUI_INCLUDE_PATH}/stb_truetype.h
        #p3d/device/d3d11/DDSTextureLoader.h
        #${P3D_DIRECT_X_TEX_INCLUDE_PATH}/WICTextureLoader/WICTextureLoader.h
    )

    SET(SOURCES 
        ${SOURCES}
        #${NLE_IMGUI_INCLUDE_PATH}/imgui.cpp
        #${NLE_IMGUI_INCLUDE_PATH}/imgui_demo.cpp
        #${NLE_IMGUI_INCLUDE_PATH}/imgui_draw.cpp
        #p3d/device/d3d11/DDSTextureLoader.cpp
        #${P3D_DIRECT_X_TEX_INCLUDE_PATH}/WICTextureLoader/WICTextureLoader.cpp
    )


INCLUDE_DIRECTORIES(
    ${P3D_GLFW_INCLUDE_PATH}
    ${P3D_D3D11_INCLUDE_PATH}
    ${P3D_GLM_INCLUDE_PATH}
    ${P3D_ASSIMP_INCLUDE_PATH}
	${P3D_CEREAL_INCLUDE_PATH}
    )

ADD_DEFINITIONS(
    -DP3D_ASSERT_ENABLE_DEBUG_BREAK
    -DP3D_ASSERT_ENABLE_ERROR_PRINT
    )

LINK_DIRECTORIES(
    ${P3D_GLFW_LIBRARY_DEBUG_PATH}
    ${P3D_GLFW_LIBRARY_RELEASE_PATH}
    ${P3D_D3D11_LIBRARY_DEBUG_PATH}
    ${P3D_D3D11_LIBRARY_RELEASE_PATH}
    ${P3D_ASSIMP_LIBRARY_DEBUG_PATH}
	${P3D_ASSIMP_LIBRARY_RELEASE_PATH}
    )

add_library (P3D STATIC
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

    debug assimp-vc140-mt
	optimized assimp-vc140-mt
    )

set_target_properties(P3D PROPERTIES DEBUG_POSTFIX _d)
