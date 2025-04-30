find_package(OpenGL REQUIRED)

set(EXE_SOURCES "main.cc")
set(EXE_INCLUDES "../.." "${CMAKE_BINARY_DIR}/configured/include" "${CMAKE_BINARY_DIR}/src/glad/include")
set(LOCAL_NAME "2_shaders")

add_executable(${LOCAL_NAME} ${EXE_SOURCES})
target_include_directories(${LOCAL_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(${LOCAL_NAME} PRIVATE
        ${OPENGL_glu_LIBRARY}
        ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        glfw
        glad
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LOCAL_NAME}
            ON
            OFF
    )
endif()