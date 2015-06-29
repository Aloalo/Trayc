project(Tester)

set(BIN_DIR ${Tester_SOURCE_DIR}/bin)

function(OPTIX_add_sample_executable target_name)
  # These calls will group PTX and CUDA files into their own directories in the Visual
  # Studio projects.
  source_group("PTX Files"  REGULAR_EXPRESSION ".+\\.ptx$")
  source_group("CUDA Files" REGULAR_EXPRESSION ".+\\.cu$")

  # Separate the sources from the CMake and CUDA options fed to the macro.  This code
  # comes from the CUDA_COMPILE_PTX macro found in FindCUDA.cmake.  We are copying the
  # code here, so that we can use our own name for the target.  target_name is used in the
  # creation of the output file names, and we want this to be unique for each target in
  # the SDK.
  CUDA_GET_SOURCES_AND_OPTIONS(source_files cmake_options options ${ARGN})
  message(STATUS "NVCC options" ${options})
  # Create the rules to build the PTX from the CUDA files.
  CUDA_WRAP_SRCS( "" PTX generated_files ${source_files} ${cmake_options}
    OPTIONS ${options})

  # Here is where we create the rule to make the executable.  We define a target name and
  # list all the source files used to create the target.  In addition we also pass along
  # the cmake_options parsed out of the arguments.
  add_executable(${target_name}
    ${source_files}
    ${generated_files}
    ${cmake_options}
    )

  # Most of the samples link against the sutil library and the optix library.  Here is the
  # rule that specifies this linkage.
  target_link_libraries( ${target_name}
    Engine
    Trayc
    ${SDL2_LIBRARY}
    ${ASSIMP_LIBRARY}
    ${GLEW_LIBRARY}
    ${OPENGL_LIBRARIES}
    optixu
    optix
    ${optix_rpath}
    ${ANTTWEAKBAR_LIBRARY}
    )
  if(USING_GNU_CXX)
    target_link_libraries( ${target_name} m ) # Explicitly link against math library (C samples don't do that by default)
  endif()
endfunction()

OPTION(CUDA_REMOVE_GLOBAL_MEMORY_SPACE_WARNING "Suppress the \"Advisory: Cannot tell what pointer points to, assuming global memory space\" warning nvcc makes." ON)

mark_as_advanced(CLEAR CUDA_64_BIT_DEVICE_CODE)

set(ASSIMP_DEBUG TRUE)
find_package(Assimp REQUIRED)

include_directories(
    ${ASSIMP_INCLUDE_DIR}
    ${CUDA_INCLUDE_DIRS}
    ${OptiX_INCLUDE}
    "${IL_INCLUDE_DIR}/.."
    ${ANTTWEAKBAR_INCLUDE_DIR}
    )

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
