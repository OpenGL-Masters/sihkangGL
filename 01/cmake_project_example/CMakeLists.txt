
# set minimum version of CMake
cmake_minimum_required(VERSION 3.14)

# set variable PROJECT_NAME as cmake_project_example and C++ version
set(PROJECT_NAME cmake_project_example)
set(CMAKE_CXX_STANDARD 17)

# project define. project() can set the project name, language version.
project(${PROJECT_NAME})

# add execuatble file by ${PROJECT_NAME} using src/main.cpp
add_executable(${PROJECT_NAME} src/main.cpp)