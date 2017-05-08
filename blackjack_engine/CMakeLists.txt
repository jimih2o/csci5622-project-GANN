cmake_minimum_required (VERSION 3.8.0)
project (BlackjackEngine)

SET(CMAKE_C_COMPILER /opt/gcc-4.9.2/bin/gcc)
SET(CMAKE_CXX_COMPILER /opt/gcc-4.9.2/bin/g++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# HDF5
#set (LIB_TYPE STATIC)
#string(TOLOWER ${LIB_TYPE} SEARCH_TYPE)
#find_package (HDF5 NAMES hdf5 COMPONENTS CXX ${SEARCH_TYPE})
#INCLUDE_DIRECTORIES (${HDF5_INCLUDE_DIR})
#set (LINK_LIBS ${LINK_LIBS} ${HDF5_CXX_${LIB_TYPE}_LIBRARY})


# OpenMP - required by FANN, not to be confused with OpenMPI
FIND_PACKAGE (OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

set(LINK_LIBS)


# OpenMPI
find_package (MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

# FANN
add_definitions(-DFANN_NO_DLL)
set (FANN_DIR ../fann/fann-master/src)
INCLUDE_DIRECTORIES (${FANN_DIR}/include)
INCLUDE_DIRECTORIES (${FANN_DIR}/)

INCLUDE_DIRECTORIES (.)

add_executable(BlackjackEngine fann_float_source.cpp main.cpp)

target_link_libraries (BlackjackEngine ${LINK_LIBS} ${MPI_LIBRARIES})
