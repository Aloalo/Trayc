project(BallPhysics)

# Define the source files.
set(BallPhysics_SRCS
    Main.cpp
    BallRenderer.h
    BallRenderer.cpp
    BallPhysics.h
    BallPhysics.cpp
    Ball.h
    Ball.cpp
    WireCube.cpp
    WireCube.h
)

set(BallPhysics_Solvers
    Solvers/SlowSolver.cpp
    Solvers/SlowSolver.h
    Solvers/PhysicsSolver.h
    Solvers/PhysicsSolver.cpp
    Solvers/VectorFields.cpp
    Solvers/VectorFields.h
)

source_group(BallPhysics FILES ${BallPhysics_SRCS})
source_group(Solvers FILES ${BallPhysics_Solvers})

add_executable(BallPhysics ${BallPhysics_SRCS} ${BallPhysics_Solvers})

target_link_libraries(BallPhysics
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

file(COPY Shaders DESTINATION .)
file(COPY Settings.ini DESTINATION .)
