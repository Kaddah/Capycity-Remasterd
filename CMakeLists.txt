cmake_minimum_required(VERSION 3.24)
project(Capycity)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(Capycity main.cpp)
