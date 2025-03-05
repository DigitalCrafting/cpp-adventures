set(EXE_SOURCES "generator_main.cc")
set(EXE_INCLUDES "./" "${CMAKE_SOURCE_DIR}/src/types_lib")

add_executable(0_test_data_generator ${EXE_SOURCES})
target_include_directories(0_test_data_generator PUBLIC ${EXE_INCLUDES})
target_link_libraries(0_test_data_generator PUBLIC
        ${LIBRARY_PROFILER_NAME}
        ${LIBRARY_HAVERSINE_NAME}
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            0_test_data_generator
            ON
            OFF
    )
endif()