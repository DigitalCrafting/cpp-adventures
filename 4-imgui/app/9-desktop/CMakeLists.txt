set(EXE_SOURCES "main.cc" "render.cpp")
set(EXE_INCLUDES "../.." "render.hpp" "${CMAKE_BINARY_DIR}/configured/include")

set(LOCAL_EXE_NAME 9_Desktop)

add_executable(${LOCAL_EXE_NAME} ${EXE_SOURCES})
target_include_directories(${LOCAL_EXE_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(${LOCAL_EXE_NAME} PRIVATE
        fmt::fmt
        spdlog::spdlog
        cxxopts::cxxopts
        implot
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LOCAL_EXE_NAME}
            ON
            OFF
    )
endif()