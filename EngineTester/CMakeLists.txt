project(EngineTester)

find_package(CEGUI REQUIRED)

include_directories(
    ${CEGUI_INCLUDE_DIR}
)

# Define the source files.
set(EngineTester_SRCS_HDRS
    Main.cpp
    DebugView.h
    DebugView.cpp
    GUIView.h
    GUIView.cpp
    GUI.h
    GUI.cpp
)

source_group(Src FILES ${EngineTester_SRCS_HDRS})
add_executable(EngineTester ${EngineTester_SRCS_HDRS})

target_link_libraries(EngineTester
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    optimized "${CEGUI_LIBRARIES_RELEASE}"
    debug "${CEGUI_LIBRARIES_DEBUG}"
)

copy_dlls()
file(COPY ${CEGUI_DLLS_DEBUG} DESTINATION ./Debug)
file(COPY ${CEGUI_DLLS_RELEASE} DESTINATION ./Release)
