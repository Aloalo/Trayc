
OPTIX_add_sample_executable(Tester
    Tester.cpp
    LabMaterials.cpp
    Labyrinth.cpp
    Box.cpp
    TweakBarHandler.cpp
    TweakBarHandler.h
    LabMaterials.h
    Labyrinth.h
    Box.h
    CUDAfiles/shaders.cu
    )

file(COPY Settings.ini DESTINATION .)
