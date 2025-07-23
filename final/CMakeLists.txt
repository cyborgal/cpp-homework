cmake_minimum_required(VERSION 3.10)
project(FocusTimeOverlay)

set(CMAKE_CXX_STANDARD 17)

# Add GLAD
include_directories(glad/include)

# Add ImGui
set(IMGUI_DIR imgui)
set(IMGUI_FILES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add include directories
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    glfw/include
)

# Add GLFW
add_subdirectory(glfw)

# Combine all source files
set(SRC_FILES
    src/main.cpp
    src/task.cpp
    src/taskmanager.cpp
    glad/src/glad.c
    ${IMGUI_FILES}
)

# Create final executable
add_executable(FocusTime ${SRC_FILES})

target_link_libraries(FocusTime glfw OpenGL32)
