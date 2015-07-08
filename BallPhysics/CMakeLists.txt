project(BallPhysics)

# Define the source files.
set(BallPhysics_SRCS
    Main.cpp
    WireCube.cpp
)

# Define the header files.
set(BallPhysics_HDRS
    WireCube.h
)

add_executable(BallPhysics ${BallPhysics_SRCS} ${BallPhysics_HDRS})

target_link_libraries(BallPhysics
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

file(COPY Shaders DESTINATION .)
file(COPY Settings.ini DESTINATION .)
