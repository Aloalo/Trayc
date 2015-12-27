project(Mandelbrot)

# Define the source files.
set(Mandelbrot_SRCS
    Main.cpp
)

source_group(Mandelbrot FILES ${Mandelbrot_SRCS})

add_executable(Mandelbrot ${Mandelbrot_SRCS})

target_link_libraries(Mandelbrot
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

#file(COPY Shaders DESTINATION .)
