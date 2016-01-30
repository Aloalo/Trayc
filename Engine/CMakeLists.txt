project(Engine)

set(Engine_HDRS
    Engine.h
)

set(Engine_Core
    Core/Core.h
    Core/ContextHandler.h
    Core/ContextHandler.cpp
    Core/Camera.h
    Core/CameraHandler.h
    Core/DefaultCameraHandler.h
    Core/InputHandler.h
    Core/InputObserver.h
    Core/Updateable.h
    Core/Camera.cpp
    Core/CameraHandler.cpp
    Core/DefaultCameraHandler.cpp
    Core/RotationalCameraHandler.cpp
    Core/RotationalCameraHandler.h
    Core/InputHandler.cpp
    Core/InputObserver.cpp
    Core/Updateable.cpp
    Core/UpdateableHandler.h
    Core/UpdateableHandler.cpp
)

set(Engine_Geometry
    Geometry/Geometry.h
    Geometry/GeometryMeshes.h
    Geometry/GeometryMeshes.cpp
    Geometry/TriangleMesh.h
    Geometry/Material.h
    Geometry/AABB.h
    Geometry/TriangleMesh.cpp
    Geometry/Material.cpp
    Geometry/AABB.cpp
    Geometry/Object3D.h
	Geometry/Object3D.cpp
    Geometry/Scene.h
    Geometry/Scene.cpp
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
    GL/BufferObject.cpp
    GL/BufferObject.h
    GL/FrameBuffer.cpp
    GL/FrameBuffer.h
    GL/VertexArray.cpp
    GL/VertexArray.h
    GL/VertexAttributeDef.cpp
    GL/VertexAttributeDef.h
    GL/VertexArrayInstanced.cpp
    GL/VertexArrayInstanced.h
    GL/UniformBuffer.cpp
    GL/UniformBuffer.h
    GL/Texture.cpp
    GL/Texture.h
)

set(Engine_Utils
    Utils/Singleton.h
    Utils/Setting.h
    Utils/StlExtensions.hpp
    Utils/VectorZip.hpp
    Utils/InitialSettings.h
    Utils/Profiler.h
    Utils/MathFunctions.h
    Utils/Utilities.h
    Utils/ErrorCheck.h
    Utils/BSpline.h
    Utils/InitialSettings.cpp
    Utils/Profiler.cpp
    Utils/MathFunctions.cpp
    Utils/MathFunctions.tpp
    Utils/Utilities.cpp
    Utils/ErrorCheck.cpp
    Utils/BSpline.cpp
)

set(Engine_Engine
	Engine/Game.h
	Engine/Game.cpp
	Engine/Renderable.h
	Engine/Renderable.cpp
    Engine/RenderableObject3D.h
	Engine/RenderableObject3D.cpp
	Engine/Renderer.h
	Engine/Renderer.cpp
    Engine/Light.h
	Engine/Light.cpp
    Engine/TextureCombiner.h
	Engine/TextureCombiner.cpp
    Engine/AssetLoader.h
	Engine/AssetLoader.cpp
    Engine/DebugDraw.h
	Engine/DebugDraw.cpp
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
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${OPENGL_LIBRARIES}
    optimized "${ASSIMP_LIBRARY_RELEASE}"
    debug "${ASSIMP_LIBRARY_DEBUG}"
)

file(COPY "Resources/" DESTINATION "../Resources")
