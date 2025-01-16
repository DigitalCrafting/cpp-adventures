add_executable(${EXECUTABLE_NAME} main.cc)

target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_BINARY_DIR}/configured/")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME})
