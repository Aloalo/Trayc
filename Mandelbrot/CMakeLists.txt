project(Mandelbrot)

# Define the source files.
set(Mandelbrot_SRCS
    Main.cpp
    MandelbrotRenderer.cpp
    MandelbrotRenderer.h
)

source_group(Mandelbrot FILES ${Mandelbrot_SRCS})

add_executable(Mandelbrot ${Mandelbrot_SRCS})

target_link_libraries(Mandelbrot
    Engine
    ${SDL2_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
)

copy_dlls()
file(COPY Shaders DESTINATION .)
