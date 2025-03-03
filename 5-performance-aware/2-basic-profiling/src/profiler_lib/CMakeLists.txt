set(LIBRARY_SOURCES "profiler.cc")
set(LIBRARY_HEADERS "profiler.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured/include")

add_library(${LIBRARY_PROFILER_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

target_include_directories(${LIBRARY_PROFILER_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(${LIBRARY_PROFILER_NAME} PUBLIC
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LIBRARY_PROFILER_NAME}
            ON
            OFF
    )
endif()