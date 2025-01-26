set(EXE_SOURCES "main.cc")
set(EXE_INCLUDES "../..")

set(LOCAL_EXE_NAME FileManager)

add_executable(${LOCAL_EXE_NAME} ${EXE_SOURCES})
target_include_directories(${LOCAL_EXE_NAME} PUBLIC ${EXE_INCLUDES})
target_link_libraries(${LOCAL_EXE_NAME} PRIVATE
        ${LIBRARY_NAME}
        implot
)

if (ENABLE_WARNINGS)
    target_set_warnings(
            ${LOCAL_EXE_NAME}
            ON
            OFF
    )
endif()