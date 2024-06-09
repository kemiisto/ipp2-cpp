cmake_minimum_required(VERSION 3.20)

project(ipp2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(MPI REQUIRED)
find_package(Boost REQUIRED COMPONENTS mpi)

set(MPI_PROGRAMS
    ch03/mpi_hello.cpp
    ch03/mpi_trap1.cpp
    ch03/mpi_trap2.cpp
    ch03/mpi_trap3.cpp)

set(BOOST_MPI_PROGRAMS
    ch03/boost_mpi_hello.cpp
    ch03/boost_mpi_trap1.cpp
    ch03/boost_mpi_trap2.cpp
    ch03/boost_mpi_trap3.cpp
)

foreach (PROGRAM ${MPI_PROGRAMS})
    get_filename_component(PROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ch03_${PROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ch03/${PROGRAM_NAME_WE}.cpp)
    target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${MPI_INCLUDE_PATH})
    target_link_libraries(${TARGET_NAME} PRIVATE ${MPI_LIBRARIES})
endforeach ()

foreach (PROGRAM ${BOOST_MPI_PROGRAMS})
    get_filename_component(ROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ch03_${ROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ch03/${ROGRAM_NAME_WE}.cpp)
    target_link_libraries(${TARGET_NAME} PRIVATE Boost::mpi)
endforeach ()
