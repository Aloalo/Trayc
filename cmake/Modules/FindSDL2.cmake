#
# Try to find SDL2 library and include path.
# Once done this will define
#
# SDL2_INCLUDE_DIR
# SDL2_BIN_DIR
# SDL2_FOUND
# 

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32 bits
	set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x86")
endif()

message("SDL2_LIB_DIR is ${SDL2_LIB_DIR}")

find_path(SDL2_INCLUDE_DIR SDL.h
  PATHS
  "${CMAKE_SOURCE_DIR}/libs/SDL2/include"
)

find_path(SDL2_BIN_DIR SDL2.dll
  PATHS
  "${SDL2_LIB_DIR}"
)

find_library(SDL2_LIBRARY_TEMP
  NAMES SDL2
  PATHS
  "${SDL2_LIB_DIR}"
)

find_library(SDL2MAIN_LIBRARY
	NAMES SDL2main
	PATHS
	"${SDL2_LIB_DIR}"
)


# SDL2 may require threads on your system.
# The Apple build may not need an explicit flag because one of the
# frameworks may already provide it.
# But for non-OSX systems, I will use the CMake Threads package.
IF(NOT APPLE)
  FIND_PACKAGE(Threads)
ENDIF(NOT APPLE)

SET(SDL2_FOUND "NO")
IF(SDL2_LIBRARY_TEMP)
	# For SDL2main
	IF(SDL2MAIN_LIBRARY)
		SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
	ENDIF(SDL2MAIN_LIBRARY)

  # For threads, as mentioned Apple doesn't need this.
  # In fact, there seems to be a problem if I used the Threads package
  # and try using this line, so I'm just skipping it entirely for OS X.
  IF(NOT APPLE)
    SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
  ENDIF(NOT APPLE)

  # Set the final string here so the GUI reflects the final state.
  SET(SDL2_LIBRARY ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
  # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
  SET(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")

  SET(SDL2_FOUND "YES")
ENDIF(SDL2_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2
                                  REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
