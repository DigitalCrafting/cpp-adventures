find_package(OpenGL REQUIRED)

set(IMGUI_SOURCES
        "./imgui/imgui.cpp"
        "./imgui/imgui_draw.cpp"
        "./imgui/imgui_tables.cpp"
        "./imgui/imgui_widgets.cpp"
        "./imgui/backends/imgui_impl_glfw.cpp"
        "./imgui/backends/imgui_impl_opengl3.cpp"
        "./imgui/misc/cpp/imgui_stdlib.cpp"
)
set(IMGUI_HEADERS
        "./imgui/imgui.h"
        "./imgui/imconfig.h"
        "./imgui/imstb_rectpack.h"
        "./imgui/imstb_textedit.h"
        "./imgui/imstb_truetype.h"
        "./imgui/backends/imgui_impl_glfw.h"
        "./imgui/backends/imgui_impl_opengl3.h"
        "./imgui/backends/imgui_impl_opengl3_loader.h"
        "./imgui/misc/cpp/imgui_stdlib.h"
)

add_library(imgui STATIC
        ${IMGUI_SOURCES}
        ${IMGUI_HEADERS}
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

set(IMPLOT_SOURCES
        "./implot/implot.cpp"
        "./implot/implot_items.cpp"
)

set(IMPLOT_HEADERS
        "./implot/implot.h"
        "./implot/implot_internal.h"
)

add_library(implot STATIC
    ${IMPLOT_SOURCES}
    ${IMPLOT_HEADERS}
)

target_link_libraries(implot PUBLIC
        ${OPENGL_glu_LIBRARY}
        ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        glfw
        glad
        imgui
)

target_include_directories(implot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/implot
)