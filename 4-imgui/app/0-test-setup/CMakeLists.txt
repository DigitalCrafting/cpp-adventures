set(EXE_SOURCES "main.cc")
set(EXE_INCLUDES "..")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        ${LIBRARY_NAME}
        ${OPENGL_glu_LIBRARY}
        ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        glfw
        glad
        imgui
        implot
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${EXECUTABLE_NAME}
            ON
            OFF
    )
endif()