# TO RUN FROM TERMINAL:
# 0.) Create Source and CMakeFile
# 1.) mkdir build
# 2.) cd build
# 3.) cmake ..   -  Generating the Build Files / Configure the Project
# DC: so it looks like we made the `build` dir into the 'CMake' dir, with project files one level above
# 4.) cmake --build .
# 5.) ./Executable

cmake_minimum_required(VERSION 3.22)

project(
        CppProjectTemplate
        VERSION 1.0.0
        LANGUAGES C CXX
)

add_executable(Executable main.cc)