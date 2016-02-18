project(EngineTester)

# Define the source files.
set(EngineTester_SRCS_HDRS
    Main.cpp
    DebugView.h
    DebugView.cpp
    LightHandler.h
    LightHandler.cpp
    GUIView.h
    GUIView.cpp
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
