cmake_minimum_required(VERSION 3.22)

project(
        CppProjectTemplate
        VERSION 1.0.0
        LANGUAGES C CXX
)

set(LIBRARY_NAME Library)
set(EXECUTABLE_NAME Executable)

add_library(${LIBRARY_NAME} my_lib.cc)
add_executable(${EXECUTABLE_NAME} main.cc)

target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})