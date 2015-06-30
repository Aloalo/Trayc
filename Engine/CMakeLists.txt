project(Engine)

set(Engine_HDRS
    Engine.h
)

set(Engine_Core
    Core/Core.h
    Core/SDLHandler.h
    Core/Camera.h
    Core/CameraHandler.h
    Core/DefaultCameraHandler.h
    Core/InputHandler.h
    Core/InputObserver.h
    Core/Updateable.h
    Core/SDLHandler.cpp
    Core/Camera.cpp
    Core/CameraHandler.cpp
    Core/DefaultCameraHandler.cpp
    Core/InputHandler.cpp
    Core/InputObserver.cpp
    Core/Updateable.cpp
    Core/UpdateableHandler.h
    Core/UpdateableHandler.cpp
)

set(Engine_Geometry
    Geometry/Geometry.h
    Geometry/TriangleMesh.h
    Geometry/IndexContainer.h
    Geometry/Material.h
    Geometry/AABB.h
    Geometry/TriangleMesh.cpp
    Geometry/IndexContainer.cpp
    Geometry/Material.cpp
    Geometry/AABB.cpp
)

set(Engine_GL
    GL/Shader.h
    GL/VertexShader.h
    GL/GeometryShader.h
    GL/FragmentShader.h
    GL/Program.h
    GL/Shader.cpp
    GL/VertexShader.cpp
    GL/GeometryShader.cpp
    GL/FragmentShader.cpp
    GL/Program.cpp
)

set(Engine_Utils
    Utils/Singleton.h
    Utils/Setting.h
    Utils/StlExtensions.hpp
    Utils/InitialSettings.h
    Utils/NormalDrawer.h
    Utils/FPSCounter.h
    Utils/MathFunctions.h
    Utils/Utilities.h
    Utils/ErrorCheck.h
    Utils/InitialSettings.cpp
    Utils/NormalDrawer.cpp
    Utils/FPSCounter.cpp
    Utils/MathFunctions.cpp
    Utils/MathFunctions.tpp
    Utils/Utilities.cpp
    Utils/ErrorCheck.cpp
)

set(Engine_Engine
    Engine/Component.h
	Engine/GraphicsComponent.h
	Engine/PhysicsComponent.h
	Engine/InputComponent.h
	Engine/Entity.h
	Engine/Scene.h
    Engine/Component.cpp
	Engine/GraphicsComponent.cpp
	Engine/PhysicsComponent.cpp
	Engine/InputComponent.cpp
	Engine/Entity.cpp
	Engine/Scene.cpp
)

set(Engine_GL_SHADERS
    Shaders/NormalShader.vert
    Shaders/NormalShader.geom
    Shaders/NormalShader.frag
)

source_group(Common FILES ${Engine_Common})
source_group(Core FILES ${Engine_Core})
source_group(Geometry FILES ${Engine_Geometry})
source_group(GL FILES ${Engine_GL})
source_group(Utils FILES ${Engine_Utils})
source_group(Engine FILES ${Engine_Engine})

add_library(Engine STATIC
    ${Engine_HDRS}
    ${Engine_Core}
    ${Engine_GL}
    ${Engine_Geometry}
    ${Engine_Common}
    ${Engine_Utils}
	${Engine_Engine}
)

target_link_libraries(Engine 
    ${SDL2_LIBRARY} 
    ${GLEW_LIBRARY} 
    ${OPENGL_LIBRARIES}
    ${ANTTWEAKBAR_LIBRARY}
)

file(COPY ${Engine_GL_SHADERS} DESTINATION "../Resources/Shaders")
