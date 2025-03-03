set(LIBRARY_SOURCES "haversine.cpp")
set(LIBRARY_HEADERS "haversine.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured/include")

add_library(${LIBRARY_HAVERSINE_NAME} STATIC
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS}
)

target_include_directories(${LIBRARY_HAVERSINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LIBRARY_HAVERSINE_NAME}
            ON
            OFF
    )
endif()