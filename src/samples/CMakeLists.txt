CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (Prototype3D_Samples)

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

# P3D
SET(P3D_INCLUDE_PATH
    "" CACHE PATH "P3D include path")

IF(P3D_INCLUDE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D include path must be specified")
ENDIF(P3D_INCLUDE_PATH STREQUAL "")

SET(P3D_LIBRARY_DEBUG_PATH
    "" CACHE PATH "P3D library debug path")

IF(P3D_LIBRARY_DEBUG_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D library debug path must be specified")
ENDIF(P3D_LIBRARY_DEBUG_PATH STREQUAL "")

SET(P3D_LIBRARY_RELEASE_PATH
    "" CACHE PATH "P3D library release path")

IF(P3D_LIBRARY_RELEASE_PATH STREQUAL "")
    MESSAGE(SEND_ERROR "P3D library release path must be specified")
ENDIF(P3D_LIBRARY_RELEASE_PATH STREQUAL "")


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


INCLUDE_DIRECTORIES(
    ${P3D_GLFW_INCLUDE_PATH}
    ${P3D_D3D11_INCLUDE_PATH}
    ${P3D_GLM_INCLUDE_PATH}
    ${P3D_INCLUDE_PATH}
    ${P3D_ASSIMP_INCLUDE_PATH}
	${P3D_CEREAL_INCLUDE_PATH}
    )

ADD_DEFINITIONS(
    -DP3D_ASSERT_ENABLE_ERROR_PRINT
    )

LINK_DIRECTORIES(
    ${P3D_GLFW_LIBRARY_DEBUG_PATH}
    ${P3D_GLFW_LIBRARY_RELEASE_PATH}
    ${P3D_D3D11_LIBRARY_DEBUG_PATH}
    ${P3D_D3D11_LIBRARY_RELEASE_PATH}
    ${P3D_LIBRARY_DEBUG_PATH}
    ${P3D_LIBRARY_RELEASE_PATH}
    ${P3D_ASSIMP_LIBRARY_DEBUG_PATH}
	${P3D_ASSIMP_LIBRARY_RELEASE_PATH}
    )

set(LIBS 
    debug glfw3
    optimized glfw3

    debug d3d11.lib
    optimized d3d11.lib
    debug d3dcompiler.lib
    optimized d3dcompiler.lib

    debug p3d_d.lib
    optimized p3d.lib
    
    debug assimp-vc140-mt
	optimized assimp-vc140-mt
)


add_executable (Sample1
  Sample1/CreateWindow.cpp
)

add_executable (Sample2
  Sample2/ImportScene.cpp
)

add_executable (Sample3
  Sample3/DrawTriangle.cpp
)

add_executable (Sample4
  Sample4/Sponza.cpp
)

add_executable (Sample5
  Sample5/CreateScene.cpp
)

TARGET_LINK_LIBRARIES(Sample1 ${LIBS})
TARGET_LINK_LIBRARIES(Sample2 ${LIBS})
TARGET_LINK_LIBRARIES(Sample3 ${LIBS})
TARGET_LINK_LIBRARIES(Sample4 ${LIBS})
TARGET_LINK_LIBRARIES(Sample5 ${LIBS})

set_target_properties(
    Sample1
    Sample2
    Sample3
    Sample4
    Sample5

    PROPERTIES DEBUG_POSTFIX _d
)