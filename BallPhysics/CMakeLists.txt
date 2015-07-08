project(BallPhysics)

# Define the source files.
set(BallPhysics_SRCS
    Main.cpp
    BallRenderer.h
    BallRenderer.cpp
    WireCube.cpp
    WireCube.h
)

# Define the header files.
set(BallPhysics_HDRS
)

add_executable(BallPhysics ${BallPhysics_SRCS})

target_link_libraries(BallPhysics
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

file(COPY Shaders DESTINATION .)
file(COPY Settings.ini DESTINATION .)
