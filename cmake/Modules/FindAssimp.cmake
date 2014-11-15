# Based on the FindPhysFS.cmake scipt
# - Try to find Assimp
# Once done this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directory
#  ASSIMP_LIBRARY - Link these to use Assimp

set(_assimp_INCLUDE_SEARCH_DIRS_SYSTEM
    C:/assimp/include
    C:/assimp
    "$ENV{ProgramFiles}/assimp/include"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/include
  )

set(_assimp_LIB_SEARCH_DIRS_SYSTEM
    C:/assimp/lib
    C:/assimp
    "$ENV{ProgramFiles}/assimp/lib"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/lib
  )

FIND_PATH(ASSIMP_INCLUDE_DIR assimp/ai_assert.h
	$ENV{ASSIMP_HOME}/include
	$ENV{ASSIMP_HOME}
	$ENV{ASSIMP_HOME}/..
	~/Library/Frameworks/AssImp.framework/Headers
	/Library/Frameworks/AssImp.framework/Headers
	/usr/local/include/assimp
	/usr/local/include
	/usr/include/assimp
	/usr/include
	/sw/include/assimp # Fink
	/sw/include
	/opt/local/include/assimp # DarwinPorts
	/opt/local/include
	/opt/csw/include/assimp # Blastwave
	/opt/csw/include
	/opt/include/assimp
	/opt/include
	${_assimp_INCLUDE_SEARCH_DIRS_SYSTEM}
	)

FIND_LIBRARY(ASSIMP_LIBRARY
	NAMES assimp
	PATHS
	$ENV{ASSIMP_HOME}/lib32
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	${_assimp_LIB_SEARCH_DIRS_SYSTEM}
	)

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)
	SET(ASSIMP_FOUND "YES")
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)

if(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARY AND ASSIMP_INCLUDE_DIR))
	message(FATAL_ERROR "Could not find assimp")
ENDIF(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARY AND ASSIMP_INCLUDE_DIR))

mark_as_advanced(ASSIMP_LIBRARY_DEBUG ASSIMP_LIBRARY_RELEASE ASSIMP_INCLUDE_DIR)