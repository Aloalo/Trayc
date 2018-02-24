#
# Try to find CEGUI library and include path.
# Once done this will define
#
# ASSIMP_INCLUDE_DIR
# ASSIMP_LIBRARY_DEBUG
# ASSIMP_LIBRARY_RELEASE
# ASSIMP_DLLS_DEBUG
# ASSIMP_DLLS_RELEASE
# 

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	set(ASSIMP_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/assimp/x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32 bits
	set(ASSIMP_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/assimp/x86")
endif()

set(LIB_NAME "assimp")

message("ASSIMP_LIB_DIR is ${ASSIMP_LIB_DIR}")

find_path(ASSIMP_INCLUDE_DIR assimp/anim.h
    PATHS
    "${CMAKE_SOURCE_DIR}/libs/assimp/include"
)

message("ASSIMP_INCLUDE_DIR is ${ASSIMP_INCLUDE_DIR}")


find_library(ASSIMP_LIBRARY
  NAMES "${LIB_NAME}.lib"
  PATHS
   "${ASSIMP_LIB_DIR}"
)

file(GLOB ASSIMP_DLL
  "${ASSIMP_LIB_DIR}/assimp-vc140-mt.dll"
)

message("ASSIMP_DLL is ${ASSIMP_DLL}")
message("ASSIMP_LIBRARY is ${ASSIMP_LIBRARY}")

