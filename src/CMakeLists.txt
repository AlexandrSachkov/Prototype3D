CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (Prototype3D)

set (P3D_VERSION_MAJOR 0)
set (P3D_VERSION_MINOR 1)
set (P3D_VERSION_PATCH 1)


IF(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /WX /arch:AVX /Ox /Ob2 /Ot /Gt /Zc:implicitNoexcept-")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /WX /arch:AVX /Od /Ob0 /Zc:implicitNoexcept-")
ENDIF(MSVC)

# Configuration GUI goes here


# =====================================================================================

#                                     Global includes

# =====================================================================================

SET(HEADERS 
	#NL_Allocator.h
	#NL_AssetImporter.h
	#NL_CerealSerializer.h
	#NL_IEditorUiManager.h
	#NL_FileIOManager.h
	#NL_IFileIOManager.h
	#NL_Game.h
	#NL_GameManager.h
	#NL_IGameManager.h
	#NL_GameObject.h
	#NL_GameObjectComponent.h
	#NL_ImguiGameEditor.h
	#NL_ImguiSceneEditor.h
	#NL_ImguiSceneViewer.h
	#NL_ImguiScriptEditor.h
	#NL_ImguiScriptViewer.h
	#NL_ImguiObjectEditor.h
	#NL_LuaBindings.h
	#NL_LuaCustomTypes.h
	#NL_MemoryManager.h
	#NL_MemoryPool.h
	#NL_Octree.h
	#NL_OperationBuffer.h
	#NL_Queue.h
	#NL_RenderingEngine.h
	#NL_IRenderingEngine.h
	#NL_RenderingComponent.h
	#NL_Resource.h
	#NL_ResourceCache.h
	#NL_IResourceCache.h
	#NL_ResourceManager.h
	#NL_IResourceManager.h
	#NL_Scene.h		
	#NL_SceneGraph.h
	#NL_ScriptExecutor.h
	#NL_ScriptingComponent.h
	#NL_ScriptingContext.h
	#NL_ScriptingEngine.h
	#NL_IScriptingEngine.h	
	#NL_Serializer.h
	#NL_ISerializer.h
	#NL_SharedArray.h
	#NL_SharedData.h
	#NL_SharedEntry.h
	#NL_ISharedEntity.h
	#NL_ISystem.h
	#NL_TextureLoader.h
	#NL_ThreadLocal.h
	#NL_UiComponent.h
	#NL_Uuid.h
	#NL_ResourceManager.cpp
	#NL_Thread.h
	#NL_IWindowManager.h
    
    internal/p3d_RenderingResources.h
    internal/NL_D3D11Utility.h
    internal/NL_D3D11Utility.inl 
    
    util/internal/p3d_CharBuffer.h
    util/internal/p3d_CircularBuffer.h
    
    util/p3d_ConsoleQueue.h
    util/p3d_GlfwWindowManager.h
    util/p3d_ImguiRenderer.h
    util/p3d_ImguiUiManager.h
    util/p3d_Timer.h	
    
    p3d_RenderingEngine.h
	)

SET(SOURCES 
	#NL_AssetImporter.cpp
	#NL_FileIOManager.cpp
	#NL_Game.cpp
	#NL_GameManager.cpp
	#NL_GameObject.cpp
	#NL_GameObjectComponent.cpp
	#NL_ImguiGameEditor.cpp
	#NL_ImguiSceneEditor.cpp
	#NL_ImguiSceneViewer.cpp
	#NL_ImguiScriptEditor.cpp
	#NL_ImguiScriptViewer.cpp
	#NL_ImguiObjectEditor.cpp
	#NL_LuaBindings.cpp
	#NL_MemoryManager.cpp
	#NL_Octree.cpp
	#NL_RenderingComponent.cpp
	#NL_RenderingEngine.cpp
	#NL_ResourceCache.cpp
	#NL_Scene.cpp	
	#NL_SceneGraph.cpp
	#NL_ScriptExecutor.cpp
	#NL_ScriptingComponent.cpp
	#NL_ScriptingContext.cpp
	#NL_ScriptingEngine.cpp
	#NL_SharedData.cpp
	#NL_TextureLoader.cpp
	#NL_UiComponent.cpp
    
    internal/NL_D3D11Utility.cpp
    
    util/p3d_ConsoleQueue.cpp
    util/p3d_GlfwWindowManager.cpp
    util/p3d_ImguiRenderer.cpp
    util/p3d_ImguiUiManager.cpp
    
    p3d_RenderingEngine.cpp
    p3dTest.cpp
	)

SET (SHADERS
    internal/shaders/Forward_VS.hlsl
	internal/shaders/Forward_PS.hlsl
)

# TBB

SET(NLE_TBB_INCLUDE_PATH
	"" CACHE PATH "TBB include path")

IF(NLE_TBB_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "TBB include path must be specified")
ENDIF(NLE_TBB_INCLUDE_PATH STREQUAL "")

SET(NLE_TBB_LIBRARY_DEBUG_PATH
	"" CACHE PATH "TBB library debug path")

IF(NLE_TBB_LIBRARY_DEBUG_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "TBB library debug path must be specified")
ENDIF(NLE_TBB_LIBRARY_DEBUG_PATH STREQUAL "")

SET(NLE_TBB_LIBRARY_RELEASE_PATH
	"" CACHE PATH "TBB library release path")

IF(NLE_TBB_LIBRARY_RELEASE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "TBB library release path must be specified")
ENDIF(NLE_TBB_LIBRARY_RELEASE_PATH STREQUAL "")

# Lua
SET(NLE_LUA_INCLUDE_PATH
	"" CACHE PATH "NLE Lua include path")

IF(NLE_LUA_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Lua include path must be specified")
ENDIF(NLE_LUA_INCLUDE_PATH STREQUAL "")

SET(NLE_LUA_LIBRARY_DEBUG_PATH
	"" CACHE PATH "NLE Lua library debug path")

IF(NLE_LUA_LIBRARY_DEBUG_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Lua library debug path must be specified")
ENDIF(NLE_LUA_LIBRARY_DEBUG_PATH STREQUAL "")

SET(NLE_LUA_LIBRARY_RELEASE_PATH
	"" CACHE PATH "NLE Lua library release path")

IF(NLE_LUA_LIBRARY_RELEASE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Lua library release path must be specified")
ENDIF(NLE_LUA_LIBRARY_RELEASE_PATH STREQUAL "")


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



# Assimp
SET(NLE_ASSIMP_INCLUDE_PATH
	"" CACHE PATH "NLE Assimp include path")

IF(NLE_ASSIMP_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Assimp include path must be specified")
ENDIF(NLE_ASSIMP_INCLUDE_PATH STREQUAL "")

SET(NLE_ASSIMP_LIBRARY_DEBUG_PATH
	"" CACHE PATH "NLE Assimp library debug path")

IF(NLE_ASSIMP_LIBRARY_DEBUG_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Assimp library debug path must be specified")
ENDIF(NLE_ASSIMP_LIBRARY_DEBUG_PATH STREQUAL "")

SET(NLE_ASSIMP_LIBRARY_RELEASE_PATH
	"" CACHE PATH "NLE Assimp library release path")

IF(NLE_ASSIMP_LIBRARY_RELEASE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Assimp library release path must be specified")
ENDIF(NLE_ASSIMP_LIBRARY_RELEASE_PATH STREQUAL "")


# Cereal
SET(NLE_CEREAL_INCLUDE_PATH
	"" CACHE PATH "NLE Cereal include path")

IF(NLE_CEREAL_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE Cereal include path must be specified")
ENDIF(NLE_CEREAL_INCLUDE_PATH STREQUAL "")



#lua-intf
SET(NLE_LUA_INTF_INCLUDE_PATH
	"" CACHE PATH "NLE LUA_INTF include path")

IF(NLE_LUA_INTF_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE LUA_INTF include path must be specified")
ENDIF(NLE_LUA_INTF_INCLUDE_PATH STREQUAL "")


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


#imgui
SET(NLE_IMGUI_INCLUDE_PATH
	"" CACHE PATH "NLE IMGUI include path")

IF(NLE_IMGUI_INCLUDE_PATH STREQUAL "")
	MESSAGE(SEND_ERROR "NLE IMGUI include path must be specified")
ENDIF(NLE_IMGUI_INCLUDE_PATH STREQUAL "")

SET(HEADERS
		${HEADERS}
		${NLE_IMGUI_INCLUDE_PATH}/imgui.h
		${NLE_IMGUI_INCLUDE_PATH}/imconfig.h
		${NLE_IMGUI_INCLUDE_PATH}/imgui_internal.h
		${NLE_IMGUI_INCLUDE_PATH}/stb_rect_pack.h
		${NLE_IMGUI_INCLUDE_PATH}/stb_textedit.h
		${NLE_IMGUI_INCLUDE_PATH}/stb_truetype.h
	)

	SET(SOURCES 
		${SOURCES}
		${NLE_IMGUI_INCLUDE_PATH}/imgui.cpp
		${NLE_IMGUI_INCLUDE_PATH}/imgui_demo.cpp
		${NLE_IMGUI_INCLUDE_PATH}/imgui_draw.cpp
	)


INCLUDE_DIRECTORIES(
    ${NLE_TBB_INCLUDE_PATH}
	${NLE_LUA_INCLUDE_PATH}
	${NLE_GLFW_INCLUDE_PATH}
	${NLE_ASSIMP_INCLUDE_PATH}
	${NLE_CEREAL_INCLUDE_PATH}
	${NLE_LUA_INTF_INCLUDE_PATH}
	${NLE_IMGUI_INCLUDE_PATH}
    ${NLE_D3D11_INCLUDE_PATH}
	)

ADD_DEFINITIONS(
	-D_CRT_SECURE_NO_WARNINGS
	-D_SCL_SECURE_NO_WARNINGS
	-D_WIN32_WINNT=0x0601
	-DWINVER=0x0601
	)

LINK_DIRECTORIES(
    ${NLE_TBB_LIBRARY_DEBUG_PATH}
	${NLE_TBB_LIBRARY_RELEASE_PATH}
	${NLE_LUA_LIBRARY_DEBUG_PATH}
	${NLE_LUA_LIBRARY_RELEASE_PATH}
	${NLE_GLFW_LIBRARY_DEBUG_PATH}
	${NLE_GLFW_LIBRARY_RELEASE_PATH}
	${NLE_ASSIMP_LIBRARY_DEBUG_PATH}
	${NLE_ASSIMP_LIBRARY_RELEASE_PATH}
    ${NLE_D3D11_LIBRARY_DEBUG_PATH}
	${NLE_D3D11_LIBRARY_RELEASE_PATH}
	)

add_executable (P3D
  ${HEADERS}
  ${SOURCES}
  ${SHADERS}
)

TARGET_LINK_LIBRARIES(P3D 
    debug tbb_debug
	optimized tbb
	debug tbb_preview_debug
	optimized tbb_preview
	debug tbbmalloc_debug
	optimized tbbmalloc
	debug tbbmalloc_proxy_debug
	optimized tbbmalloc_proxy
	debug tbbproxy_debug
	optimized tbbproxy
    
	debug lua51
	optimized lua51
	
	debug glfw3
	optimized glfw3

	debug assimpd
	optimized assimp
    
    debug d3d11.lib
	optimized d3d11.lib
	debug d3dcompiler.lib
	optimized d3dcompiler.lib
	)

set_target_properties(P3D PROPERTIES DEBUG_POSTFIX _d)