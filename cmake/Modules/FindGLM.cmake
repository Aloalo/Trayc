#
# Try to find GLM include path.
# Once done this will define
#
# GLM_FOUND
# GLM_INCLUDE_DIR
#

if(NOT GLM_FOUND)
    find_path(GLM_INCLUDE_DIR glm/glm.hpp
        PATHS
        "${CMAKE_SOURCE_DIR}/libs"
        )

    SET(GLM_FOUND "NO")
    if(GLM_INCLUDE_DIR)
        SET(GLM_FOUND "YES")
    endif(GLM_INCLUDE_DIR)
    
    MESSAGE("GLM_INCLUDE_DIR is ${GLM_INCLUDE_DIR}")
endif(NOT GLM_FOUND)
