# This module sets:
#   IL_LIBRARIES -   the name of the IL library. These include the full path to
#                    the core DevIL library. This one has to be linked into the
#                    application.
#   ILU_LIBRARIES -  the name of the ILU library. Again, the full path. This
#                    library is for filters and effects, not actual loading. It
#                    doesn't have to be linked if the functionality it provides
#                    is not used.
#   ILUT_LIBRARIES - the name of the ILUT library. Full path. This part of the
#                    library interfaces with OpenGL. It is not strictly needed
#                    in applications.
#   IL_INCLUDE_DIR - where to find the il.h, ilu.h and ilut.h files.
#   IL_FOUND -       this is set to TRUE if all the above variables were set.
#                    This will be set to false if ILU or ILUT are not found,
#                    even if they are not needed. In most systems, if one
#                    library is found all the others are as well. That's the
#                    way the DevIL developers release it.
#

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	set(DEVIL_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/DevIL/lib/x64/Release")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32 bits
	set(DEVIL_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/DevIL/lib/x86/Release")
endif()

message("DEVIL_LIB_DIR is ${DEVIL_LIB_DIR}")

find_path(IL_INCLUDE_DIR IL/il.h
    PATHS
    "${CMAKE_SOURCE_DIR}/libs/DevIL/include"
)

find_path(IL_BIN_DIR DevIL.dll
    PATHS
    "${DEVIL_LIB_DIR}"
)

find_path(ILU_BIN_DIR ILU.dll
    PATHS
    "${DEVIL_LIB_DIR}"
)

find_path(ILUT_BIN_DIR ILUT.dll
    PATHS
    "${DEVIL_LIB_DIR}"
)

find_library(IL_LIBRARIES
  NAMES IL DevIL
  PATHS
    "${DEVIL_LIB_DIR}"
)

find_library(ILUT_LIBRARIES
  NAMES ILUT
  PATHS
    "${DEVIL_LIB_DIR}"
)

find_library(ILU_LIBRARIES
  NAMES ILU
  PATHS
    "${DEVIL_LIB_DIR}"
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(IL DEFAULT_MSG
                                  IL_LIBRARIES ILU_LIBRARIES
                                  ILUT_LIBRARIES IL_INCLUDE_DIR)
