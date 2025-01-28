set(EXE_SOURCES "main.cc")
set(EXE_INCLUDES "../.." "${CMAKE_BINARY_DIR}/configured/include")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        fmt::fmt
        spdlog::spdlog
        cxxopts::cxxopts
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