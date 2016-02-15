#
# Try to find Easyloggingpp include path.
# Once done this will define
#
# ELPP_FOUND
# ELPP_INCLUDE_DIR
#

if(NOT ELPP_FOUND)
    find_path(ELPP_INCLUDE_DIR easylogging++.h
        PATHS
        "${CMAKE_SOURCE_DIR}/libs/easylogging"
        )

    SET(ELPP_FOUND "NO")
    if(ELPP_INCLUDE_DIR)
        SET(ELPP_FOUND "YES")
    endif(ELPP_INCLUDE_DIR)
    
    MESSAGE("ELPP_INCLUDE_DIR is ${ELPP_INCLUDE_DIR}")
endif(NOT ELPP_FOUND)
