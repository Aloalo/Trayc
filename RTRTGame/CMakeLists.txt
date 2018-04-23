project(RTRTGame)

# Define the source files.
set(RTRTGame_SRCS_HDRS
    Main.cpp
    GUIView.cpp
    GUIView.h
    RTRTObjects.cpp
    RTRTObjects.h
)

source_group(Src FILES ${RTRTGame_SRCS_HDRS})
add_executable(RTRTGame ${RTRTGame_SRCS_HDRS})

target_link_libraries(RTRTGame
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

copy_dlls()
