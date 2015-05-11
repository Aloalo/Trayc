project(FunctionDrawer)

# Define the source files.
set(FunctionDrawer_SRCS
    Main.cpp
    GUIHandler.cpp
    UserSettings.cpp
    TwoVariableFunction.cpp
    FunctionRasterizer.cpp
    FunctionTracer.cpp
    FunctionDrawer.cpp
)

# Define the header files.
set(FunctionDrawer_HDRS
    GUIHandler.h
    UserSettings.h
    TwoVariableFunction.h
    FunctionRasterizer.h
    FunctionTracer.h
    FunctionDrawer.h
)

add_executable(FunctionDrawer ${FunctionDrawer_SRCS} ${FunctionDrawer_HDRS})

target_link_libraries(FunctionDrawer
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${ANTTWEAKBAR_LIBRARY}
)

file(COPY Shaders DESTINATION .)
file(COPY Settings.ini DESTINATION .)
file(COPY Function.txt DESTINATION .)
