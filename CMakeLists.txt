cmake_minimum_required(VERSION 3.20)

project(ipp2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(MPI REQUIRED)

file(GLOB MPI_PROGRAMS ch03/*.cpp)

foreach (MPI_PROGRAM ${MPI_PROGRAMS})
    get_filename_component(MPI_PROGRAM_NAME_WE ${MPI_PROGRAM} NAME_WE)
    set(TARGET_NAME ch03_${MPI_PROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ch03/${MPI_PROGRAM_NAME_WE}.cpp)
    target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${MPI_INCLUDE_PATH})
    target_link_libraries(${TARGET_NAME} ${MPI_LIBRARIES})
endforeach ()
