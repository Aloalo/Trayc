project(Engine)

set(Engine_HDRS
    Engine.h
)

set(Engine_Common_SRCS
    Common/ErrorCheck.cpp
    Common/MathFunctions.cpp
    Common/MathFunctions.tpp
    Common/Utilities.cpp
)

set(Engine_Common_HDRS
    Common/Common.h
    Common/Settings.h
    Common/ErrorCheck.h
    Common/MathFunctions.h
    Common/Utilities.h
)
  
set(Engine_Core_SRCS
    Core/SDLHandler.cpp
    Core/Camera.cpp
    Core/CameraHandler.cpp
    Core/DefaultCameraHandler.cpp
    Core/EventHandler.cpp
    Core/EventListener.cpp
    Core/Updateable.cpp
)

set(Engine_Core_HDRS
    Core/Core.h
    Core/SDLHandler.h
    Core/Camera.h
    Core/CameraHandler.h
    Core/DefaultCameraHandler.h
    Core/EventHandler.h
    Core/EventListener.h
    Core/Updateable.h
)

set(Engine_Geometry_SRCS
    Geometry/TriangleMesh.cpp
    Geometry/IndexContainer.cpp
    Geometry/Scene.cpp
    Geometry/Material.cpp
)

set(Engine_Geometry_HDRS
    Geometry/Geometry.h
    Geometry/TriangleMesh.h
    Geometry/IndexContainer.h
    Geometry/Scene.h
    Geometry/Material.h
)

set(Engine_GL_SRCS
    GL/Shader.cpp
    GL/VertexShader.cpp
    GL/GeometryShader.cpp
    GL/FragmentShader.cpp
    GL/Program.cpp
)

set(Engine_GL_HDRS
    GL/Shader.h
    GL/VertexShader.h
    GL/GeometryShader.h
    GL/FragmentShader.h
    GL/Program.h
)

set(Engine_Utils_HDRS
    Utils/Singleton.h
    Utils/Setting.h
    Utils/InitialSettings.h
)

set(Engine_Utils_SRCS
    Utils/InitialSettings.cpp
)

source_group(Common FILES ${Engine_Common_SRCS} ${Engine_Common_HDRS})
source_group(Core FILES ${Engine_Core_SRCS} ${Engine_Core_HDRS})
source_group(Geometry FILES ${Engine_Geometry_SRCS} ${Engine_Geometry_HDRS})
source_group(GL FILES ${Engine_GL_SRCS} ${Engine_GL_HDRS})
source_group(Utils FILES ${Engine_Utils_HDRS}, ${Engine_Utils_SRCS})

add_library(Engine STATIC
    ${Engine_HDRS}
    ${Engine_Core_SRCS}
    ${Engine_Core_HDRS}
    ${Engine_GL_SRCS}
    ${Engine_GL_HDRS}
    ${Engine_Geometry_SRCS}
    ${Engine_Geometry_HDRS}
    ${Engine_Common_SRCS}
    ${Engine_Common_HDRS}
    ${Engine_Utils_SRCS}
    ${Engine_Utils_HDRS}
)

target_link_libraries(Engine 
    ${SDL2_LIBRARY} 
    ${GLEW_LIBRARY} 
    ${OPENGL_LIBRARIES}
    ${ANTTWEAKBAR_LIBRARY}
)
