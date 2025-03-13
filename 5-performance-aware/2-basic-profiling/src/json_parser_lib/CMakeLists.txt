set(LIBRARY_SOURCES "base_json_lookup_parser.cpp")
set(LIBRARY_HEADERS "base_json_lookup_parser.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_SOURCE_DIR}/src/commons_lib")

add_library(${LIBRARY_PARSER_NAME} STATIC
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS}
)

target_include_directories(${LIBRARY_PARSER_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(${LIBRARY_PARSER_NAME} PUBLIC ${LIBRARY_HAVERSINE_NAME})

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LIBRARY_PARSER_NAME}
            ON
            OFF
    )
endif()