project(EngineTester)

# Define the source files.
set(EngineTester_SRCS_HDRS
    Main.cpp
    DebugView.h
    DebugView.cpp
    SponzaDemo.h
    SponzaDemo.cpp
    HeadDemo.h
    HeadDemo.cpp
    ShadowsDemo.h
    ShadowsDemo.cpp
    GUIView.h
    GUIView.cpp
    PBRMaterialDemo.cpp
    PBRMaterialDemo.h
)

source_group(Src FILES ${EngineTester_SRCS_HDRS})
add_executable(EngineTester ${EngineTester_SRCS_HDRS})

target_link_libraries(EngineTester
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

copy_dlls()
