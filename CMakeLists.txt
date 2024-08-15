cmake_minimum_required(VERSION 3.20)

project(ipp2 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

find_package(MPI REQUIRED)
find_package(Boost REQUIRED COMPONENTS mpi)

if (WIN32)
    find_package(PThreads4W REQUIRED)
else ()
    find_package(Threads REQUIRED)
endif ()

set(MPI_PROGRAMS
    ch03/mpi_hello.c
    ch03/mpi_hello_any.c
    ch03/mpi_trap1.c
    ch03/mpi_trap2.c
)

set(BOOST_MPI_PROGRAMS
    ch03/boost_mpi_hello.cpp
    ch03/boost_mpi_hello_any.cpp
    ch03/boost_mpi_trap1.cpp
    ch03/boost_mpi_trap2.cpp
)

set(PTHREAD_PROGRAMS
    ch04/pth_hello.c
    ch04/pth_mat_vect.c
    ch04/pth_pi.c
    ch04/pth_pi_mutex.c
    ch04/pth_msg.c
    ch04/pth_msg_sem.c
    ch04/pth_barrier_bar.c
)

set(THREAD_PROGRAMS
    ch04/std_hello.cpp
    ch04/std_mat_vect.cpp
    ch04/std_pi.cpp
    ch04/std_pi_mutex.cpp
    ch04/std_pi_mutex_lock_guard.cpp
    ch04/std_msg.cpp
    ch04/std_msg_sem.cpp
    ch04/std_latch_bar.cpp
    ch04/std_barrier_bar.cpp
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
    if (WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE PThreads4W::PThreads4W)
    else ()
        target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads m)
    endif ()
endforeach ()

foreach (PROGRAM ${THREAD_PROGRAMS})
    get_filename_component(ROGRAM_NAME_WE ${PROGRAM} NAME_WE)
    set(TARGET_NAME ${ROGRAM_NAME_WE})
    add_executable(${TARGET_NAME} ${PROGRAM})
endforeach ()
