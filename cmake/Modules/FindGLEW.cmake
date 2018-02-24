#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	set(GLEW_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/glew/lib/Release/x64")
	set(GLEW_BIN_DIR "${CMAKE_SOURCE_DIR}/libs/glew/bin/Release/x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32 bits
	set(GLEW_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/glew/lib/Release/Win32")
	set(GLEW_BIN_DIR "${CMAKE_SOURCE_DIR}/libs/glew/bin/Release/Win32")
endif()

message("GLEW_LIB_DIR is ${GLEW_LIB_DIR}")
message("GLEW_BIN_DIR is ${GLEW_BIN_DIR}")

find_path(GLEW_INCLUDE_PATH GL/glew.h
	PATHS
	"${CMAKE_SOURCE_DIR}/libs/glew/include"
)
	
find_path(GLEW_BIN_PATH glew32.dll
	PATHS
	"${GLEW_BIN_DIR}"
)
	
find_library(GLEW_LIBRARY
	NAMES glew GLEW glew32 glew32s
	PATHS
	"${GLEW_LIB_DIR}"
)


set(GLEW_FOUND "NO")
if(GLEW_INCLUDE_PATH AND GLEW_LIBRARY AND GLEW_BIN_PATH)
	set(GLEW_LIBRARIES ${GLEW_LIBRARY})
	set(GLEW_FOUND "YES")
endif(GLEW_INCLUDE_PATH AND GLEW_LIBRARY AND GLEW_BIN_PATH)

