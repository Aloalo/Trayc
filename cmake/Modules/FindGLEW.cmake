#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

IF(WIN32)
	FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h
        PATHS
		"${CMAKE_SOURCE_DIR}/libs/glew/include"
        )
        
    FIND_PATH(GLEW_BIN_PATH glew32.dll
        PATHS
		"${CMAKE_SOURCE_DIR}/libs/glew/bin/Release/Win32"
        )
        
    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew GLEW glew32 glew32s
        PATHS
        "${CMAKE_SOURCE_DIR}/libs/glew/lib/Release/Win32"
    )
ELSE (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${GLEW_ROOT_DIR}/include
		DOC "The directory where GL/glew.h resides")

	FIND_LIBRARY( GLEW_LIBRARY
		NAMES GLEW glew
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${GLEW_ROOT_DIR}/lib
        $ENV{GLEW_HOME}/lib
		DOC "The GLEW library")
ENDIF(WIN32)

SET(GLEW_FOUND "NO")
IF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY AND GLEW_BIN_PATH)
	SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
	SET(GLEW_FOUND "YES")
ENDIF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY AND GLEW_BIN_PATH)

