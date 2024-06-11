cmake_minimum_required(VERSION 3.20)

project(ipp2 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

find_package(MPI REQUIRED)
find_package(Boost REQUIRED COMPONENTS mpi)

if (WIN32)
    find_package(PThreads4W REQUIRED)
else ()
endif ()

set(MPI_PROGRAMS
    ch03/mpi_hello.cpp
    ch03/mpi_trap1.cpp
    ch03/mpi_trap2.cpp
    ch03/mpi_trap3.cpp
)

set(BOOST_MPI_PROGRAMS
    ch03/boost_mpi_hello.cpp
    ch03/boost_mpi_trap1.cpp
    ch03/boost_mpi_trap2.cpp
    ch03/boost_mpi_trap3.cpp
)

set(PTHREAD_PROGRAMS
    ch04/pth_hello.c
    ch04/pth_mat_vect.c
    ch04/pth_pi.c
    ch04/pth_pi_mutex.c
)

set(THREAD_PROGRAMS
    ch04/std_hello.cpp
    ch04/std_mat_vect.cpp
    ch04/std_pi.cpp
    ch04/std_pi_mutex.cpp
)

foreach (PROGRAM ${MPI_PROGRAMS})
    get_filename_component(PROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ${PROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ${PROGRAM})
    target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${MPI_INCLUDE_PATH})
    target_link_libraries(${TARGET_NAME} PRIVATE ${MPI_LIBRARIES})
endforeach ()

foreach (PROGRAM ${BOOST_MPI_PROGRAMS})
    get_filename_component(ROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ${ROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ${PROGRAM})
    target_link_libraries(${TARGET_NAME} PRIVATE Boost::mpi)
endforeach ()

foreach (PROGRAM ${PTHREAD_PROGRAMS})
    get_filename_component(ROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ${ROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ${PROGRAM})
    target_link_libraries(${TARGET_NAME} PRIVATE PThreads4W::PThreads4W)
endforeach ()

foreach (PROGRAM ${THREAD_PROGRAMS})
    get_filename_component(ROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ${ROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ${PROGRAM})
endforeach ()
