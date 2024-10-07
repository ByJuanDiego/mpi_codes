cmake_minimum_required(VERSION 3.29)
project(mpi_test)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE MPI::MPI_CXX)
