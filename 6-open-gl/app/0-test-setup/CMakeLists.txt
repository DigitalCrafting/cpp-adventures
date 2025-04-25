set(EXE_SOURCES "main.cc")
set(EXE_INCLUDES "../.." "${CMAKE_BINARY_DIR}/configured/include")

add_executable(0_${EXECUTABLE_NAME} ${EXE_SOURCES})
target_include_directories(0_${EXECUTABLE_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(0_${EXECUTABLE_NAME} PRIVATE
        fmt::fmt
        spdlog::spdlog
        cxxopts::cxxopts
        imgui
        implot
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            0_${EXECUTABLE_NAME}
            ON
            OFF
    )
endif()