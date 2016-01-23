project(GameBlocks)

# Define the source files.
set(GameBlocks_SRCS
    Main.cpp
)

# Define the header files.
set(GameBlocks_HDRS
)

add_executable(GameBlocks ${GameBlocks_SRCS} ${GameBlocks_HDRS})

target_link_libraries(GameBlocks
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

copy_dlls()
