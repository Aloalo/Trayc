project(Trayc)

set(Trayc_HDRS
    BufferDrawer.h
    SceneGraph.h
    Environment.h
    GameEngine.h
    OptixTracer.h
    Utils.h
)

set(Trayc_SRCS
    BufferDrawer.cpp
    SceneGraph.cpp
    Environment.cpp
    GameEngine.cpp
    OptixTracer.cpp
    Utils.cpp
)

set(Trayc_Handlers_HDRS
    Handlers/AccelHandler.h
    Handlers/MaterialHandler.h
    Handlers/OptixTextureHandler.h
    Handlers/ProgramHandler.h
)

set(Trayc_Handlers_SRCS
    Handlers/AccelHandler.cpp
    Handlers/MaterialHandler.cpp
    Handlers/OptixTextureHandler.cpp
    Handlers/ProgramHandler.cpp
)

set(Trayc_GL_SHADERS
    Shaders/fxaa.frag
    Shaders/fxaa.vert
)

set(Trayc_CUDA_FILES
    CUDAfiles/context_shaders.cu
    CUDAfiles/material_shaders.cu
    CUDAfiles/triangle_mesh.cu
    CUDAfiles/helper.h
    CUDAfiles/lights.h
    CUDAfiles/common.h
    CUDAfiles/phong.h
    CUDAfiles/random.h
    CUDAfiles/sphere.cu
    CUDAfiles/rectangleAA.cu
    CUDAfiles/box.cu
    CUDAfiles/light.cu
)

source_group(Handlers FILES ${Trayc_Handlers_HDRS} ${Trayc_Handlers_SRCS})
source_group(Shaders FILES ${Trayc_GL_SHADERS})
source_group(CUDAfiles FILES ${Trayc_CUDA_FILES})

find_package(CUDA 2.3 REQUIRED)
find_package(OptiX REQUIRED)
find_package(DevIL REQUIRED)

include_directories(
    ${CUDA_INCLUDE_DIRS}
    "${OptiX_INCLUDE}"
    "${IL_INCLUDE_DIR}/.."
)

include(Macros)
include(CompilerInfo)
include(ConfigCompilerFlags)

set(USE_FAST_MATH TRUE)
if(NOT PASSED_FIRST_CONFIGURE)
  set(flag "--use_fast_math")
  list(FIND CUDA_NVCC_FLAGS ${flag} index)
  if(index EQUAL -1)
    list(APPEND CUDA_NVCC_FLAGS ${flag})
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} CACHE LIST "Semi-colon delimit multiple arguments." FORCE)
  endif()

  if (CUDA_VERSION VERSION_LESS "3.0")
    set(flag "--keep")
    list(FIND CUDA_NVCC_FLAGS ${flag} index)
    if(index EQUAL -1)
      list(APPEND CUDA_NVCC_FLAGS ${flag})
      set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} CACHE LIST "Semi-colon delimit multiple arguments." FORCE)
    endif()
  endif()

  if( APPLE )
    # Undef'ing __BLOCKS__ for OSX builds.  This is due to a name clash between OSX 10.6
    # C headers and CUDA headers
    set(flag "-U__BLOCKS__")
    list(FIND CUDA_NVCC_FLAGS ${flag} index)
    if(index EQUAL -1)
      list(APPEND CUDA_NVCC_FLAGS ${flag})
      set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} CACHE LIST "Semi-colon delimit multiple arguments." FORCE)
    endif()
  endif()
endif(NOT PASSED_FIRST_CONFIGURE)

CUDA_COMPILE_PTX(ptx_files ${Trayc_CUDA_FILES})

add_library(Trayc STATIC
    ${Trayc_HDRS}
    ${Trayc_SRCS}
    ${Trayc_Handlers_HDRS}
    ${Trayc_Handlers_SRCS}
    ${Trayc_CUDA_FILES}
    ${ptx_files}
)

target_link_libraries(Trayc 
    ${SDL2_LIBRARY} 
    ${GLEW_LIBRARY} 
    ${OPENGL_LIBRARIES}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    optix
    optixu
)

file(COPY ${Trayc_GL_SHADERS} DESTINATION "../Resources/Shaders")
