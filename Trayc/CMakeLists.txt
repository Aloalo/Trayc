project(Trayc)

set(Trayc_HDRS
    BufferDrawer.h
    Environment.h
    GameEngine.h
    OptixTracer.h
    Programs.h
    Utils.h
)

set(Trayc_SRCS
    BufferDrawer.cpp
    Environment.cpp
    GameEngine.cpp
    OptixTracer.cpp
    Programs.cpp
    Utils.cpp
)

set(Trayc_Handlers_HDRS
    Handlers/AccelHandler.h
    Handlers/MaterialHandler.h
    Handlers/OptixTextureHandler.h
)

set(Trayc_Handlers_SRCS
    Handlers/AccelHandler.cpp
    Handlers/MaterialHandler.cpp
    Handlers/OptixTextureHandler.cpp
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
    CUDAfiles/phong.h
    CUDAfiles/random.h
)

source_group(Handlers FILES ${Trayc_Handlers_HDRS} ${Trayc_Handlers_SRCS})
source_group(Shaders FILES ${Trayc_GL_SHADERS})
source_group(CUDAfiles FILES ${Trayc_CUDA_FILES})

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
