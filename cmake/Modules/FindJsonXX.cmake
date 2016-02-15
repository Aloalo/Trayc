#
# Try to find JsonXX include path.
# Once done this will define
#
# JXX_FOUND
# JXX_INCLUDE_DIR
# JXX_SRC_DIR
#

if(NOT JXX_FOUND)
    find_path(JXX_INCLUDE_DIR jsonxx.h
        PATHS
        "${CMAKE_SOURCE_DIR}/libs/jsonxx"
        )
        
    set(JXX_SRC_DIR "${CMAKE_SOURCE_DIR}/libs/jsonxx/jsonxx.cc")

    SET(JXX_FOUND "NO")
    if(JXX_INCLUDE_DIR)
        SET(JXX_FOUND "YES")
    endif(JXX_INCLUDE_DIR)
    
    MESSAGE("JXX_INCLUDE_DIR is ${JXX_INCLUDE_DIR}")
endif(NOT JXX_FOUND)
