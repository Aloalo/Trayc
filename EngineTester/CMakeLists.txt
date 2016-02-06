project(EngineTester)

# Define the source files.
set(EngineTester_SRCS
    Main.cpp
)

# Define the header files.
set(EngineTester_HDRS
)

add_executable(EngineTester ${EngineTester_SRCS} ${EngineTester_HDRS})

target_link_libraries(EngineTester
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

copy_dlls()
