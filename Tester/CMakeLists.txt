
OPTIX_add_sample_executable(Tester
    Tester.cpp
    LabMaterials.cpp
    Labyrinth.cpp
    Box.cpp
    LabMaterials.h
    Labyrinth.h
    Box.h
    Settings.ini
    CUDAfiles/shaders.cu
    CUDAfiles/rectangleAA.cu
    CUDAfiles/box.cu
    )

file(COPY Settings.ini DESTINATION .)
